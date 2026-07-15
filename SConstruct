import os
import subprocess
import sys
import sysconfig
import platform
import shlex
import numpy as np

import SCons.Errors

SCons.Warnings.warningAsException(True)

# capnp's kj library warns when $PWD is stale (doesn't match the real cwd); keep them in sync
os.environ.pop('PWD', None)

Decider('MD5-timestamp')

SetOption('num_jobs', max(1, int(os.cpu_count()/2)))

AddOption('--asan', action='store_true', help='turn on ASAN')
AddOption('--ubsan', action='store_true', help='turn on UBSan')
AddOption('--mutation', action='store_true', help='generate mutation-ready code')
AddOption('--ccflags', action='store', type='string', default='', help='pass arbitrary flags over the command line')
AddOption('--minimal',
          action='store_false',
          dest='extras',
          default=os.path.exists(File('#.gitattributes').abspath), # minimal by default on release branch (where there's no LFS)
          help='the minimum build to run openpilot. no tests, tools, etc.')
AddOption('--verbose', action='store_true', help='show full compiler/linker command lines instead of short build lines')

# Detect platform
arch = subprocess.check_output(["uname", "-m"], encoding='utf8').rstrip()
if platform.system() == "Darwin":
  arch = "Darwin"
  brew_prefix = subprocess.check_output(['brew', '--prefix'], encoding='utf8').strip()
elif arch == "aarch64" and os.path.isfile('/TICI'):
  arch = "larch64"
  try:
    from openpilot.system.hardware import HARDWARE
    HARDWARE.set_power_save(False)
    os.sched_setaffinity(0, range(8))
  except Exception:
    # host tuning needs real device sysfs; the prebuilt arm64 container fakes /TICI and has none
    pass
assert arch in [
  "larch64",  # linux tici arm64
  "aarch64",  # linux pc arm64
  "x86_64",   # linux pc x64
  "Darwin",   # macOS arm64 (x86 not supported)
]

env = Environment(
  ENV={
    "PATH": os.environ['PATH'],
    "PYTHONPATH": Dir("#").abspath + ':' + Dir(f"#third_party/acados").abspath,
    "ACADOS_SOURCE_DIR": Dir("#third_party/acados").abspath,
    "ACADOS_PYTHON_INTERFACE_PATH": Dir("#third_party/acados/acados_template").abspath,
    "TERA_PATH": Dir("#").abspath + f"/third_party/acados/{arch}/t_renderer"
  },
  CC='clang',
  CXX='clang++',
  CCFLAGS=[
    "-g",
    "-fPIC",
    "-O2",
    "-Wunused",
    "-Werror",
    "-Wshadow",
    "-Wno-unknown-warning-option",
    "-Wno-inconsistent-missing-override",
    "-Wno-c99-designator",
    "-Wno-reorder-init-list",
    "-Wno-vla-cxx-extension",
  ],
  CFLAGS=["-std=gnu11"],
  CXXFLAGS=["-std=c++1z"],
  CPPPATH=[
    "#",
    "#msgq",
    "#third_party",
    "#third_party/json11",
    "#third_party/linux/include",
    "#third_party/acados/include",
    "#third_party/acados/include/blasfeo/include",
    "#third_party/acados/include/hpipm/include",
    "#third_party/catch2/include",
    "#third_party/libyuv/include",
  ],
  LIBPATH=[
    "#common",
    "#msgq_repo",
    "#third_party",
    "#selfdrive/pandad",
    "#rednose/helpers",
    f"#third_party/libyuv/{arch}/lib",
    f"#third_party/acados/{arch}/lib",
  ],
  RPATH=[],
  CYTHONCFILESUFFIX=".cpp",
  COMPILATIONDB_USE_ABSPATH=True,
  REDNOSE_ROOT="#",
  tools=["default", "cython", "compilation_db", "rednose_filter"],
  toolpath=["#site_scons/site_tools", "#rednose_repo/site_scons/site_tools"],
)

# Arch-specific flags and paths
if arch == "larch64":
  env.Append(CPPPATH=[
    "#third_party/opencl/include",
    "/usr/include/aarch64-linux-gnu",
  ])
  env.Append(LIBPATH=[
    "/usr/local/lib",
    "/usr/lib/aarch64-linux-gnu",
    "/system/vendor/lib64",
  ])
  arch_flags = ["-D__TICI__", "-mcpu=cortex-a57", "-DQCOM2"]
  env.Append(CCFLAGS=arch_flags)
  env.Append(CXXFLAGS=arch_flags)
elif arch == "Darwin":
  env.Append(LIBPATH=[
    f"{brew_prefix}/lib",
    f"{brew_prefix}/opt/openssl@3.0/lib",
    f"{brew_prefix}/opt/llvm/lib/c++",
    "/System/Library/Frameworks/OpenGL.framework/Libraries",
  ])
  env.Append(CCFLAGS=["-DGL_SILENCE_DEPRECATION"])
  env.Append(CXXFLAGS=["-DGL_SILENCE_DEPRECATION"])
  env.Append(CPPPATH=[
    f"{brew_prefix}/include",
    f"{brew_prefix}/opt/openssl@3.0/include",
  ])
  # the same static libs are pulled in by multiple deps; harmless, quiet the noise
  env.Append(LINKFLAGS=["-Wl,-no_warn_duplicate_libraries"])
  # spi.cc is device-only, so it archives with no symbols on host. 'ar rcS' skips the
  # symbol table (no warning) and ranlib rebuilds it quietly with -no_warning_for_no_symbols.
  env.Replace(ARFLAGS="rcS")
  env.Append(RANLIBFLAGS=["-no_warning_for_no_symbols"])
else:
  env.Append(LIBPATH=[
    "/usr/lib",
    "/usr/local/lib",
  ])

# Sanitizers and extra CCFLAGS from CLI
if GetOption('asan'):
  env.Append(CCFLAGS=["-fsanitize=address", "-fno-omit-frame-pointer"])
  env.Append(LINKFLAGS=["-fsanitize=address"])
elif GetOption('ubsan'):
  env.Append(CCFLAGS=["-fsanitize=undefined"])
  env.Append(LINKFLAGS=["-fsanitize=undefined"])

_extra_cc = shlex.split(GetOption('ccflags') or '')
if _extra_cc:
  env.Append(CCFLAGS=_extra_cc)

# no --as-needed on mac linker
if arch != "Darwin":
  env.Append(LINKFLAGS=["-Wl,--as-needed", "-Wl,--no-undefined"])

# pretty build output (short colored lines; pass --verbose for full commands)
env.Tool('pretty')

# progress output
node_interval = 5
node_count = 0
def progress_function(node):
  global node_count
  node_count += node_interval
  sys.stderr.write("progress: %d\n" % node_count)
if os.environ.get('SCONS_PROGRESS'):
  Progress(progress_function, interval=node_interval)

# ********** Cython build environment **********
py_include = sysconfig.get_paths()['include']
envCython = env.Clone()
envCython["CPPPATH"] += [py_include, np.get_include()]
envCython["CCFLAGS"] += ["-Wno-#warnings", "-Wno-shadow", "-Wno-deprecated-declarations"]
envCython["CCFLAGS"].remove("-Werror")

envCython["LIBS"] = []
if arch == "Darwin":
  envCython["LINKFLAGS"] = env["LINKFLAGS"] + ["-bundle", "-undefined", "dynamic_lookup"]
else:
  envCython["LINKFLAGS"] = ["-pthread", "-shared"]

np_version = SCons.Script.Value(np.__version__)
Export('envCython', 'np_version')

Export('env', 'arch')

# Setup cache dir
default_cache_dir = os.environ.get('SCONS_CACHE_DIR') or ('/data/scons_cache' if arch == "larch64" else '/tmp/scons_cache')
cache_dir = ARGUMENTS.get('cache_dir', default_cache_dir)
CacheDir(cache_dir)
Clean(["."], cache_dir)

# ********** start building stuff **********

# Build common module
SConscript(['common/SConscript'])
Import('_common')
common = [_common, 'json11', 'zmq']
Export('common')

# Build messaging (cereal + msgq + socketmaster + their dependencies)
# Enable swaglog include in submodules
env_swaglog = env.Clone()
env_swaglog['CXXFLAGS'].append('-DSWAGLOG="\\"common/swaglog.h\\""')
SConscript(['msgq_repo/SConscript'], exports={'env': env_swaglog})
SConscript(['opendbc_repo/SConscript'], exports={'env': env_swaglog})

SConscript(['cereal/SConscript'])

Import('socketmaster', 'msgq')
messaging = [socketmaster, msgq, 'capnp', 'kj',]
Export('messaging')


# Build other submodules
SConscript(['panda/SConscript'])

# Build rednose library
SConscript(['rednose/SConscript'])

# Build system services
SConscript([
  'system/loggerd/SConscript',
  'system/proprietary_runtime/SConscript',
])

if arch == "larch64":
  SConscript(['system/camerad/SConscript'])

# Build openpilot
SConscript(['third_party/SConscript'])

SConscript(['selfdrive/SConscript'])

SConscript(['iqpilot/SConscript'])

if Dir('#tools/cabana/').exists() and GetOption('extras'):
  SConscript(['tools/replay/SConscript'])
  if arch != "larch64":
    SConscript(['tools/cabana/SConscript'])
    if Dir('#tools/jotpluggler/').exists():
      SConscript(['tools/jotpluggler/SConscript'])


env.CompilationDatabase('compile_commands.json')
