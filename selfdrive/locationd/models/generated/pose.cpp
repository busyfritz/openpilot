#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.13.2                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2613128383328169093) {
   out_2613128383328169093[0] = delta_x[0] + nom_x[0];
   out_2613128383328169093[1] = delta_x[1] + nom_x[1];
   out_2613128383328169093[2] = delta_x[2] + nom_x[2];
   out_2613128383328169093[3] = delta_x[3] + nom_x[3];
   out_2613128383328169093[4] = delta_x[4] + nom_x[4];
   out_2613128383328169093[5] = delta_x[5] + nom_x[5];
   out_2613128383328169093[6] = delta_x[6] + nom_x[6];
   out_2613128383328169093[7] = delta_x[7] + nom_x[7];
   out_2613128383328169093[8] = delta_x[8] + nom_x[8];
   out_2613128383328169093[9] = delta_x[9] + nom_x[9];
   out_2613128383328169093[10] = delta_x[10] + nom_x[10];
   out_2613128383328169093[11] = delta_x[11] + nom_x[11];
   out_2613128383328169093[12] = delta_x[12] + nom_x[12];
   out_2613128383328169093[13] = delta_x[13] + nom_x[13];
   out_2613128383328169093[14] = delta_x[14] + nom_x[14];
   out_2613128383328169093[15] = delta_x[15] + nom_x[15];
   out_2613128383328169093[16] = delta_x[16] + nom_x[16];
   out_2613128383328169093[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2805699213723397762) {
   out_2805699213723397762[0] = -nom_x[0] + true_x[0];
   out_2805699213723397762[1] = -nom_x[1] + true_x[1];
   out_2805699213723397762[2] = -nom_x[2] + true_x[2];
   out_2805699213723397762[3] = -nom_x[3] + true_x[3];
   out_2805699213723397762[4] = -nom_x[4] + true_x[4];
   out_2805699213723397762[5] = -nom_x[5] + true_x[5];
   out_2805699213723397762[6] = -nom_x[6] + true_x[6];
   out_2805699213723397762[7] = -nom_x[7] + true_x[7];
   out_2805699213723397762[8] = -nom_x[8] + true_x[8];
   out_2805699213723397762[9] = -nom_x[9] + true_x[9];
   out_2805699213723397762[10] = -nom_x[10] + true_x[10];
   out_2805699213723397762[11] = -nom_x[11] + true_x[11];
   out_2805699213723397762[12] = -nom_x[12] + true_x[12];
   out_2805699213723397762[13] = -nom_x[13] + true_x[13];
   out_2805699213723397762[14] = -nom_x[14] + true_x[14];
   out_2805699213723397762[15] = -nom_x[15] + true_x[15];
   out_2805699213723397762[16] = -nom_x[16] + true_x[16];
   out_2805699213723397762[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4356096610437586896) {
   out_4356096610437586896[0] = 1.0;
   out_4356096610437586896[1] = 0.0;
   out_4356096610437586896[2] = 0.0;
   out_4356096610437586896[3] = 0.0;
   out_4356096610437586896[4] = 0.0;
   out_4356096610437586896[5] = 0.0;
   out_4356096610437586896[6] = 0.0;
   out_4356096610437586896[7] = 0.0;
   out_4356096610437586896[8] = 0.0;
   out_4356096610437586896[9] = 0.0;
   out_4356096610437586896[10] = 0.0;
   out_4356096610437586896[11] = 0.0;
   out_4356096610437586896[12] = 0.0;
   out_4356096610437586896[13] = 0.0;
   out_4356096610437586896[14] = 0.0;
   out_4356096610437586896[15] = 0.0;
   out_4356096610437586896[16] = 0.0;
   out_4356096610437586896[17] = 0.0;
   out_4356096610437586896[18] = 0.0;
   out_4356096610437586896[19] = 1.0;
   out_4356096610437586896[20] = 0.0;
   out_4356096610437586896[21] = 0.0;
   out_4356096610437586896[22] = 0.0;
   out_4356096610437586896[23] = 0.0;
   out_4356096610437586896[24] = 0.0;
   out_4356096610437586896[25] = 0.0;
   out_4356096610437586896[26] = 0.0;
   out_4356096610437586896[27] = 0.0;
   out_4356096610437586896[28] = 0.0;
   out_4356096610437586896[29] = 0.0;
   out_4356096610437586896[30] = 0.0;
   out_4356096610437586896[31] = 0.0;
   out_4356096610437586896[32] = 0.0;
   out_4356096610437586896[33] = 0.0;
   out_4356096610437586896[34] = 0.0;
   out_4356096610437586896[35] = 0.0;
   out_4356096610437586896[36] = 0.0;
   out_4356096610437586896[37] = 0.0;
   out_4356096610437586896[38] = 1.0;
   out_4356096610437586896[39] = 0.0;
   out_4356096610437586896[40] = 0.0;
   out_4356096610437586896[41] = 0.0;
   out_4356096610437586896[42] = 0.0;
   out_4356096610437586896[43] = 0.0;
   out_4356096610437586896[44] = 0.0;
   out_4356096610437586896[45] = 0.0;
   out_4356096610437586896[46] = 0.0;
   out_4356096610437586896[47] = 0.0;
   out_4356096610437586896[48] = 0.0;
   out_4356096610437586896[49] = 0.0;
   out_4356096610437586896[50] = 0.0;
   out_4356096610437586896[51] = 0.0;
   out_4356096610437586896[52] = 0.0;
   out_4356096610437586896[53] = 0.0;
   out_4356096610437586896[54] = 0.0;
   out_4356096610437586896[55] = 0.0;
   out_4356096610437586896[56] = 0.0;
   out_4356096610437586896[57] = 1.0;
   out_4356096610437586896[58] = 0.0;
   out_4356096610437586896[59] = 0.0;
   out_4356096610437586896[60] = 0.0;
   out_4356096610437586896[61] = 0.0;
   out_4356096610437586896[62] = 0.0;
   out_4356096610437586896[63] = 0.0;
   out_4356096610437586896[64] = 0.0;
   out_4356096610437586896[65] = 0.0;
   out_4356096610437586896[66] = 0.0;
   out_4356096610437586896[67] = 0.0;
   out_4356096610437586896[68] = 0.0;
   out_4356096610437586896[69] = 0.0;
   out_4356096610437586896[70] = 0.0;
   out_4356096610437586896[71] = 0.0;
   out_4356096610437586896[72] = 0.0;
   out_4356096610437586896[73] = 0.0;
   out_4356096610437586896[74] = 0.0;
   out_4356096610437586896[75] = 0.0;
   out_4356096610437586896[76] = 1.0;
   out_4356096610437586896[77] = 0.0;
   out_4356096610437586896[78] = 0.0;
   out_4356096610437586896[79] = 0.0;
   out_4356096610437586896[80] = 0.0;
   out_4356096610437586896[81] = 0.0;
   out_4356096610437586896[82] = 0.0;
   out_4356096610437586896[83] = 0.0;
   out_4356096610437586896[84] = 0.0;
   out_4356096610437586896[85] = 0.0;
   out_4356096610437586896[86] = 0.0;
   out_4356096610437586896[87] = 0.0;
   out_4356096610437586896[88] = 0.0;
   out_4356096610437586896[89] = 0.0;
   out_4356096610437586896[90] = 0.0;
   out_4356096610437586896[91] = 0.0;
   out_4356096610437586896[92] = 0.0;
   out_4356096610437586896[93] = 0.0;
   out_4356096610437586896[94] = 0.0;
   out_4356096610437586896[95] = 1.0;
   out_4356096610437586896[96] = 0.0;
   out_4356096610437586896[97] = 0.0;
   out_4356096610437586896[98] = 0.0;
   out_4356096610437586896[99] = 0.0;
   out_4356096610437586896[100] = 0.0;
   out_4356096610437586896[101] = 0.0;
   out_4356096610437586896[102] = 0.0;
   out_4356096610437586896[103] = 0.0;
   out_4356096610437586896[104] = 0.0;
   out_4356096610437586896[105] = 0.0;
   out_4356096610437586896[106] = 0.0;
   out_4356096610437586896[107] = 0.0;
   out_4356096610437586896[108] = 0.0;
   out_4356096610437586896[109] = 0.0;
   out_4356096610437586896[110] = 0.0;
   out_4356096610437586896[111] = 0.0;
   out_4356096610437586896[112] = 0.0;
   out_4356096610437586896[113] = 0.0;
   out_4356096610437586896[114] = 1.0;
   out_4356096610437586896[115] = 0.0;
   out_4356096610437586896[116] = 0.0;
   out_4356096610437586896[117] = 0.0;
   out_4356096610437586896[118] = 0.0;
   out_4356096610437586896[119] = 0.0;
   out_4356096610437586896[120] = 0.0;
   out_4356096610437586896[121] = 0.0;
   out_4356096610437586896[122] = 0.0;
   out_4356096610437586896[123] = 0.0;
   out_4356096610437586896[124] = 0.0;
   out_4356096610437586896[125] = 0.0;
   out_4356096610437586896[126] = 0.0;
   out_4356096610437586896[127] = 0.0;
   out_4356096610437586896[128] = 0.0;
   out_4356096610437586896[129] = 0.0;
   out_4356096610437586896[130] = 0.0;
   out_4356096610437586896[131] = 0.0;
   out_4356096610437586896[132] = 0.0;
   out_4356096610437586896[133] = 1.0;
   out_4356096610437586896[134] = 0.0;
   out_4356096610437586896[135] = 0.0;
   out_4356096610437586896[136] = 0.0;
   out_4356096610437586896[137] = 0.0;
   out_4356096610437586896[138] = 0.0;
   out_4356096610437586896[139] = 0.0;
   out_4356096610437586896[140] = 0.0;
   out_4356096610437586896[141] = 0.0;
   out_4356096610437586896[142] = 0.0;
   out_4356096610437586896[143] = 0.0;
   out_4356096610437586896[144] = 0.0;
   out_4356096610437586896[145] = 0.0;
   out_4356096610437586896[146] = 0.0;
   out_4356096610437586896[147] = 0.0;
   out_4356096610437586896[148] = 0.0;
   out_4356096610437586896[149] = 0.0;
   out_4356096610437586896[150] = 0.0;
   out_4356096610437586896[151] = 0.0;
   out_4356096610437586896[152] = 1.0;
   out_4356096610437586896[153] = 0.0;
   out_4356096610437586896[154] = 0.0;
   out_4356096610437586896[155] = 0.0;
   out_4356096610437586896[156] = 0.0;
   out_4356096610437586896[157] = 0.0;
   out_4356096610437586896[158] = 0.0;
   out_4356096610437586896[159] = 0.0;
   out_4356096610437586896[160] = 0.0;
   out_4356096610437586896[161] = 0.0;
   out_4356096610437586896[162] = 0.0;
   out_4356096610437586896[163] = 0.0;
   out_4356096610437586896[164] = 0.0;
   out_4356096610437586896[165] = 0.0;
   out_4356096610437586896[166] = 0.0;
   out_4356096610437586896[167] = 0.0;
   out_4356096610437586896[168] = 0.0;
   out_4356096610437586896[169] = 0.0;
   out_4356096610437586896[170] = 0.0;
   out_4356096610437586896[171] = 1.0;
   out_4356096610437586896[172] = 0.0;
   out_4356096610437586896[173] = 0.0;
   out_4356096610437586896[174] = 0.0;
   out_4356096610437586896[175] = 0.0;
   out_4356096610437586896[176] = 0.0;
   out_4356096610437586896[177] = 0.0;
   out_4356096610437586896[178] = 0.0;
   out_4356096610437586896[179] = 0.0;
   out_4356096610437586896[180] = 0.0;
   out_4356096610437586896[181] = 0.0;
   out_4356096610437586896[182] = 0.0;
   out_4356096610437586896[183] = 0.0;
   out_4356096610437586896[184] = 0.0;
   out_4356096610437586896[185] = 0.0;
   out_4356096610437586896[186] = 0.0;
   out_4356096610437586896[187] = 0.0;
   out_4356096610437586896[188] = 0.0;
   out_4356096610437586896[189] = 0.0;
   out_4356096610437586896[190] = 1.0;
   out_4356096610437586896[191] = 0.0;
   out_4356096610437586896[192] = 0.0;
   out_4356096610437586896[193] = 0.0;
   out_4356096610437586896[194] = 0.0;
   out_4356096610437586896[195] = 0.0;
   out_4356096610437586896[196] = 0.0;
   out_4356096610437586896[197] = 0.0;
   out_4356096610437586896[198] = 0.0;
   out_4356096610437586896[199] = 0.0;
   out_4356096610437586896[200] = 0.0;
   out_4356096610437586896[201] = 0.0;
   out_4356096610437586896[202] = 0.0;
   out_4356096610437586896[203] = 0.0;
   out_4356096610437586896[204] = 0.0;
   out_4356096610437586896[205] = 0.0;
   out_4356096610437586896[206] = 0.0;
   out_4356096610437586896[207] = 0.0;
   out_4356096610437586896[208] = 0.0;
   out_4356096610437586896[209] = 1.0;
   out_4356096610437586896[210] = 0.0;
   out_4356096610437586896[211] = 0.0;
   out_4356096610437586896[212] = 0.0;
   out_4356096610437586896[213] = 0.0;
   out_4356096610437586896[214] = 0.0;
   out_4356096610437586896[215] = 0.0;
   out_4356096610437586896[216] = 0.0;
   out_4356096610437586896[217] = 0.0;
   out_4356096610437586896[218] = 0.0;
   out_4356096610437586896[219] = 0.0;
   out_4356096610437586896[220] = 0.0;
   out_4356096610437586896[221] = 0.0;
   out_4356096610437586896[222] = 0.0;
   out_4356096610437586896[223] = 0.0;
   out_4356096610437586896[224] = 0.0;
   out_4356096610437586896[225] = 0.0;
   out_4356096610437586896[226] = 0.0;
   out_4356096610437586896[227] = 0.0;
   out_4356096610437586896[228] = 1.0;
   out_4356096610437586896[229] = 0.0;
   out_4356096610437586896[230] = 0.0;
   out_4356096610437586896[231] = 0.0;
   out_4356096610437586896[232] = 0.0;
   out_4356096610437586896[233] = 0.0;
   out_4356096610437586896[234] = 0.0;
   out_4356096610437586896[235] = 0.0;
   out_4356096610437586896[236] = 0.0;
   out_4356096610437586896[237] = 0.0;
   out_4356096610437586896[238] = 0.0;
   out_4356096610437586896[239] = 0.0;
   out_4356096610437586896[240] = 0.0;
   out_4356096610437586896[241] = 0.0;
   out_4356096610437586896[242] = 0.0;
   out_4356096610437586896[243] = 0.0;
   out_4356096610437586896[244] = 0.0;
   out_4356096610437586896[245] = 0.0;
   out_4356096610437586896[246] = 0.0;
   out_4356096610437586896[247] = 1.0;
   out_4356096610437586896[248] = 0.0;
   out_4356096610437586896[249] = 0.0;
   out_4356096610437586896[250] = 0.0;
   out_4356096610437586896[251] = 0.0;
   out_4356096610437586896[252] = 0.0;
   out_4356096610437586896[253] = 0.0;
   out_4356096610437586896[254] = 0.0;
   out_4356096610437586896[255] = 0.0;
   out_4356096610437586896[256] = 0.0;
   out_4356096610437586896[257] = 0.0;
   out_4356096610437586896[258] = 0.0;
   out_4356096610437586896[259] = 0.0;
   out_4356096610437586896[260] = 0.0;
   out_4356096610437586896[261] = 0.0;
   out_4356096610437586896[262] = 0.0;
   out_4356096610437586896[263] = 0.0;
   out_4356096610437586896[264] = 0.0;
   out_4356096610437586896[265] = 0.0;
   out_4356096610437586896[266] = 1.0;
   out_4356096610437586896[267] = 0.0;
   out_4356096610437586896[268] = 0.0;
   out_4356096610437586896[269] = 0.0;
   out_4356096610437586896[270] = 0.0;
   out_4356096610437586896[271] = 0.0;
   out_4356096610437586896[272] = 0.0;
   out_4356096610437586896[273] = 0.0;
   out_4356096610437586896[274] = 0.0;
   out_4356096610437586896[275] = 0.0;
   out_4356096610437586896[276] = 0.0;
   out_4356096610437586896[277] = 0.0;
   out_4356096610437586896[278] = 0.0;
   out_4356096610437586896[279] = 0.0;
   out_4356096610437586896[280] = 0.0;
   out_4356096610437586896[281] = 0.0;
   out_4356096610437586896[282] = 0.0;
   out_4356096610437586896[283] = 0.0;
   out_4356096610437586896[284] = 0.0;
   out_4356096610437586896[285] = 1.0;
   out_4356096610437586896[286] = 0.0;
   out_4356096610437586896[287] = 0.0;
   out_4356096610437586896[288] = 0.0;
   out_4356096610437586896[289] = 0.0;
   out_4356096610437586896[290] = 0.0;
   out_4356096610437586896[291] = 0.0;
   out_4356096610437586896[292] = 0.0;
   out_4356096610437586896[293] = 0.0;
   out_4356096610437586896[294] = 0.0;
   out_4356096610437586896[295] = 0.0;
   out_4356096610437586896[296] = 0.0;
   out_4356096610437586896[297] = 0.0;
   out_4356096610437586896[298] = 0.0;
   out_4356096610437586896[299] = 0.0;
   out_4356096610437586896[300] = 0.0;
   out_4356096610437586896[301] = 0.0;
   out_4356096610437586896[302] = 0.0;
   out_4356096610437586896[303] = 0.0;
   out_4356096610437586896[304] = 1.0;
   out_4356096610437586896[305] = 0.0;
   out_4356096610437586896[306] = 0.0;
   out_4356096610437586896[307] = 0.0;
   out_4356096610437586896[308] = 0.0;
   out_4356096610437586896[309] = 0.0;
   out_4356096610437586896[310] = 0.0;
   out_4356096610437586896[311] = 0.0;
   out_4356096610437586896[312] = 0.0;
   out_4356096610437586896[313] = 0.0;
   out_4356096610437586896[314] = 0.0;
   out_4356096610437586896[315] = 0.0;
   out_4356096610437586896[316] = 0.0;
   out_4356096610437586896[317] = 0.0;
   out_4356096610437586896[318] = 0.0;
   out_4356096610437586896[319] = 0.0;
   out_4356096610437586896[320] = 0.0;
   out_4356096610437586896[321] = 0.0;
   out_4356096610437586896[322] = 0.0;
   out_4356096610437586896[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_838605051751895038) {
   out_838605051751895038[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_838605051751895038[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_838605051751895038[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_838605051751895038[3] = dt*state[12] + state[3];
   out_838605051751895038[4] = dt*state[13] + state[4];
   out_838605051751895038[5] = dt*state[14] + state[5];
   out_838605051751895038[6] = state[6];
   out_838605051751895038[7] = state[7];
   out_838605051751895038[8] = state[8];
   out_838605051751895038[9] = state[9];
   out_838605051751895038[10] = state[10];
   out_838605051751895038[11] = state[11];
   out_838605051751895038[12] = state[12];
   out_838605051751895038[13] = state[13];
   out_838605051751895038[14] = state[14];
   out_838605051751895038[15] = state[15];
   out_838605051751895038[16] = state[16];
   out_838605051751895038[17] = state[17];
}
void F_fun(double *state, double dt, double *out_4469587873875989476) {
   out_4469587873875989476[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4469587873875989476[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4469587873875989476[2] = 0;
   out_4469587873875989476[3] = 0;
   out_4469587873875989476[4] = 0;
   out_4469587873875989476[5] = 0;
   out_4469587873875989476[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4469587873875989476[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4469587873875989476[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4469587873875989476[9] = 0;
   out_4469587873875989476[10] = 0;
   out_4469587873875989476[11] = 0;
   out_4469587873875989476[12] = 0;
   out_4469587873875989476[13] = 0;
   out_4469587873875989476[14] = 0;
   out_4469587873875989476[15] = 0;
   out_4469587873875989476[16] = 0;
   out_4469587873875989476[17] = 0;
   out_4469587873875989476[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4469587873875989476[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4469587873875989476[20] = 0;
   out_4469587873875989476[21] = 0;
   out_4469587873875989476[22] = 0;
   out_4469587873875989476[23] = 0;
   out_4469587873875989476[24] = 0;
   out_4469587873875989476[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4469587873875989476[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4469587873875989476[27] = 0;
   out_4469587873875989476[28] = 0;
   out_4469587873875989476[29] = 0;
   out_4469587873875989476[30] = 0;
   out_4469587873875989476[31] = 0;
   out_4469587873875989476[32] = 0;
   out_4469587873875989476[33] = 0;
   out_4469587873875989476[34] = 0;
   out_4469587873875989476[35] = 0;
   out_4469587873875989476[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4469587873875989476[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4469587873875989476[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4469587873875989476[39] = 0;
   out_4469587873875989476[40] = 0;
   out_4469587873875989476[41] = 0;
   out_4469587873875989476[42] = 0;
   out_4469587873875989476[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4469587873875989476[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4469587873875989476[45] = 0;
   out_4469587873875989476[46] = 0;
   out_4469587873875989476[47] = 0;
   out_4469587873875989476[48] = 0;
   out_4469587873875989476[49] = 0;
   out_4469587873875989476[50] = 0;
   out_4469587873875989476[51] = 0;
   out_4469587873875989476[52] = 0;
   out_4469587873875989476[53] = 0;
   out_4469587873875989476[54] = 0;
   out_4469587873875989476[55] = 0;
   out_4469587873875989476[56] = 0;
   out_4469587873875989476[57] = 1;
   out_4469587873875989476[58] = 0;
   out_4469587873875989476[59] = 0;
   out_4469587873875989476[60] = 0;
   out_4469587873875989476[61] = 0;
   out_4469587873875989476[62] = 0;
   out_4469587873875989476[63] = 0;
   out_4469587873875989476[64] = 0;
   out_4469587873875989476[65] = 0;
   out_4469587873875989476[66] = dt;
   out_4469587873875989476[67] = 0;
   out_4469587873875989476[68] = 0;
   out_4469587873875989476[69] = 0;
   out_4469587873875989476[70] = 0;
   out_4469587873875989476[71] = 0;
   out_4469587873875989476[72] = 0;
   out_4469587873875989476[73] = 0;
   out_4469587873875989476[74] = 0;
   out_4469587873875989476[75] = 0;
   out_4469587873875989476[76] = 1;
   out_4469587873875989476[77] = 0;
   out_4469587873875989476[78] = 0;
   out_4469587873875989476[79] = 0;
   out_4469587873875989476[80] = 0;
   out_4469587873875989476[81] = 0;
   out_4469587873875989476[82] = 0;
   out_4469587873875989476[83] = 0;
   out_4469587873875989476[84] = 0;
   out_4469587873875989476[85] = dt;
   out_4469587873875989476[86] = 0;
   out_4469587873875989476[87] = 0;
   out_4469587873875989476[88] = 0;
   out_4469587873875989476[89] = 0;
   out_4469587873875989476[90] = 0;
   out_4469587873875989476[91] = 0;
   out_4469587873875989476[92] = 0;
   out_4469587873875989476[93] = 0;
   out_4469587873875989476[94] = 0;
   out_4469587873875989476[95] = 1;
   out_4469587873875989476[96] = 0;
   out_4469587873875989476[97] = 0;
   out_4469587873875989476[98] = 0;
   out_4469587873875989476[99] = 0;
   out_4469587873875989476[100] = 0;
   out_4469587873875989476[101] = 0;
   out_4469587873875989476[102] = 0;
   out_4469587873875989476[103] = 0;
   out_4469587873875989476[104] = dt;
   out_4469587873875989476[105] = 0;
   out_4469587873875989476[106] = 0;
   out_4469587873875989476[107] = 0;
   out_4469587873875989476[108] = 0;
   out_4469587873875989476[109] = 0;
   out_4469587873875989476[110] = 0;
   out_4469587873875989476[111] = 0;
   out_4469587873875989476[112] = 0;
   out_4469587873875989476[113] = 0;
   out_4469587873875989476[114] = 1;
   out_4469587873875989476[115] = 0;
   out_4469587873875989476[116] = 0;
   out_4469587873875989476[117] = 0;
   out_4469587873875989476[118] = 0;
   out_4469587873875989476[119] = 0;
   out_4469587873875989476[120] = 0;
   out_4469587873875989476[121] = 0;
   out_4469587873875989476[122] = 0;
   out_4469587873875989476[123] = 0;
   out_4469587873875989476[124] = 0;
   out_4469587873875989476[125] = 0;
   out_4469587873875989476[126] = 0;
   out_4469587873875989476[127] = 0;
   out_4469587873875989476[128] = 0;
   out_4469587873875989476[129] = 0;
   out_4469587873875989476[130] = 0;
   out_4469587873875989476[131] = 0;
   out_4469587873875989476[132] = 0;
   out_4469587873875989476[133] = 1;
   out_4469587873875989476[134] = 0;
   out_4469587873875989476[135] = 0;
   out_4469587873875989476[136] = 0;
   out_4469587873875989476[137] = 0;
   out_4469587873875989476[138] = 0;
   out_4469587873875989476[139] = 0;
   out_4469587873875989476[140] = 0;
   out_4469587873875989476[141] = 0;
   out_4469587873875989476[142] = 0;
   out_4469587873875989476[143] = 0;
   out_4469587873875989476[144] = 0;
   out_4469587873875989476[145] = 0;
   out_4469587873875989476[146] = 0;
   out_4469587873875989476[147] = 0;
   out_4469587873875989476[148] = 0;
   out_4469587873875989476[149] = 0;
   out_4469587873875989476[150] = 0;
   out_4469587873875989476[151] = 0;
   out_4469587873875989476[152] = 1;
   out_4469587873875989476[153] = 0;
   out_4469587873875989476[154] = 0;
   out_4469587873875989476[155] = 0;
   out_4469587873875989476[156] = 0;
   out_4469587873875989476[157] = 0;
   out_4469587873875989476[158] = 0;
   out_4469587873875989476[159] = 0;
   out_4469587873875989476[160] = 0;
   out_4469587873875989476[161] = 0;
   out_4469587873875989476[162] = 0;
   out_4469587873875989476[163] = 0;
   out_4469587873875989476[164] = 0;
   out_4469587873875989476[165] = 0;
   out_4469587873875989476[166] = 0;
   out_4469587873875989476[167] = 0;
   out_4469587873875989476[168] = 0;
   out_4469587873875989476[169] = 0;
   out_4469587873875989476[170] = 0;
   out_4469587873875989476[171] = 1;
   out_4469587873875989476[172] = 0;
   out_4469587873875989476[173] = 0;
   out_4469587873875989476[174] = 0;
   out_4469587873875989476[175] = 0;
   out_4469587873875989476[176] = 0;
   out_4469587873875989476[177] = 0;
   out_4469587873875989476[178] = 0;
   out_4469587873875989476[179] = 0;
   out_4469587873875989476[180] = 0;
   out_4469587873875989476[181] = 0;
   out_4469587873875989476[182] = 0;
   out_4469587873875989476[183] = 0;
   out_4469587873875989476[184] = 0;
   out_4469587873875989476[185] = 0;
   out_4469587873875989476[186] = 0;
   out_4469587873875989476[187] = 0;
   out_4469587873875989476[188] = 0;
   out_4469587873875989476[189] = 0;
   out_4469587873875989476[190] = 1;
   out_4469587873875989476[191] = 0;
   out_4469587873875989476[192] = 0;
   out_4469587873875989476[193] = 0;
   out_4469587873875989476[194] = 0;
   out_4469587873875989476[195] = 0;
   out_4469587873875989476[196] = 0;
   out_4469587873875989476[197] = 0;
   out_4469587873875989476[198] = 0;
   out_4469587873875989476[199] = 0;
   out_4469587873875989476[200] = 0;
   out_4469587873875989476[201] = 0;
   out_4469587873875989476[202] = 0;
   out_4469587873875989476[203] = 0;
   out_4469587873875989476[204] = 0;
   out_4469587873875989476[205] = 0;
   out_4469587873875989476[206] = 0;
   out_4469587873875989476[207] = 0;
   out_4469587873875989476[208] = 0;
   out_4469587873875989476[209] = 1;
   out_4469587873875989476[210] = 0;
   out_4469587873875989476[211] = 0;
   out_4469587873875989476[212] = 0;
   out_4469587873875989476[213] = 0;
   out_4469587873875989476[214] = 0;
   out_4469587873875989476[215] = 0;
   out_4469587873875989476[216] = 0;
   out_4469587873875989476[217] = 0;
   out_4469587873875989476[218] = 0;
   out_4469587873875989476[219] = 0;
   out_4469587873875989476[220] = 0;
   out_4469587873875989476[221] = 0;
   out_4469587873875989476[222] = 0;
   out_4469587873875989476[223] = 0;
   out_4469587873875989476[224] = 0;
   out_4469587873875989476[225] = 0;
   out_4469587873875989476[226] = 0;
   out_4469587873875989476[227] = 0;
   out_4469587873875989476[228] = 1;
   out_4469587873875989476[229] = 0;
   out_4469587873875989476[230] = 0;
   out_4469587873875989476[231] = 0;
   out_4469587873875989476[232] = 0;
   out_4469587873875989476[233] = 0;
   out_4469587873875989476[234] = 0;
   out_4469587873875989476[235] = 0;
   out_4469587873875989476[236] = 0;
   out_4469587873875989476[237] = 0;
   out_4469587873875989476[238] = 0;
   out_4469587873875989476[239] = 0;
   out_4469587873875989476[240] = 0;
   out_4469587873875989476[241] = 0;
   out_4469587873875989476[242] = 0;
   out_4469587873875989476[243] = 0;
   out_4469587873875989476[244] = 0;
   out_4469587873875989476[245] = 0;
   out_4469587873875989476[246] = 0;
   out_4469587873875989476[247] = 1;
   out_4469587873875989476[248] = 0;
   out_4469587873875989476[249] = 0;
   out_4469587873875989476[250] = 0;
   out_4469587873875989476[251] = 0;
   out_4469587873875989476[252] = 0;
   out_4469587873875989476[253] = 0;
   out_4469587873875989476[254] = 0;
   out_4469587873875989476[255] = 0;
   out_4469587873875989476[256] = 0;
   out_4469587873875989476[257] = 0;
   out_4469587873875989476[258] = 0;
   out_4469587873875989476[259] = 0;
   out_4469587873875989476[260] = 0;
   out_4469587873875989476[261] = 0;
   out_4469587873875989476[262] = 0;
   out_4469587873875989476[263] = 0;
   out_4469587873875989476[264] = 0;
   out_4469587873875989476[265] = 0;
   out_4469587873875989476[266] = 1;
   out_4469587873875989476[267] = 0;
   out_4469587873875989476[268] = 0;
   out_4469587873875989476[269] = 0;
   out_4469587873875989476[270] = 0;
   out_4469587873875989476[271] = 0;
   out_4469587873875989476[272] = 0;
   out_4469587873875989476[273] = 0;
   out_4469587873875989476[274] = 0;
   out_4469587873875989476[275] = 0;
   out_4469587873875989476[276] = 0;
   out_4469587873875989476[277] = 0;
   out_4469587873875989476[278] = 0;
   out_4469587873875989476[279] = 0;
   out_4469587873875989476[280] = 0;
   out_4469587873875989476[281] = 0;
   out_4469587873875989476[282] = 0;
   out_4469587873875989476[283] = 0;
   out_4469587873875989476[284] = 0;
   out_4469587873875989476[285] = 1;
   out_4469587873875989476[286] = 0;
   out_4469587873875989476[287] = 0;
   out_4469587873875989476[288] = 0;
   out_4469587873875989476[289] = 0;
   out_4469587873875989476[290] = 0;
   out_4469587873875989476[291] = 0;
   out_4469587873875989476[292] = 0;
   out_4469587873875989476[293] = 0;
   out_4469587873875989476[294] = 0;
   out_4469587873875989476[295] = 0;
   out_4469587873875989476[296] = 0;
   out_4469587873875989476[297] = 0;
   out_4469587873875989476[298] = 0;
   out_4469587873875989476[299] = 0;
   out_4469587873875989476[300] = 0;
   out_4469587873875989476[301] = 0;
   out_4469587873875989476[302] = 0;
   out_4469587873875989476[303] = 0;
   out_4469587873875989476[304] = 1;
   out_4469587873875989476[305] = 0;
   out_4469587873875989476[306] = 0;
   out_4469587873875989476[307] = 0;
   out_4469587873875989476[308] = 0;
   out_4469587873875989476[309] = 0;
   out_4469587873875989476[310] = 0;
   out_4469587873875989476[311] = 0;
   out_4469587873875989476[312] = 0;
   out_4469587873875989476[313] = 0;
   out_4469587873875989476[314] = 0;
   out_4469587873875989476[315] = 0;
   out_4469587873875989476[316] = 0;
   out_4469587873875989476[317] = 0;
   out_4469587873875989476[318] = 0;
   out_4469587873875989476[319] = 0;
   out_4469587873875989476[320] = 0;
   out_4469587873875989476[321] = 0;
   out_4469587873875989476[322] = 0;
   out_4469587873875989476[323] = 1;
}
void h_4(double *state, double *unused, double *out_3292015623268015671) {
   out_3292015623268015671[0] = state[6] + state[9];
   out_3292015623268015671[1] = state[7] + state[10];
   out_3292015623268015671[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_8242565656520005999) {
   out_8242565656520005999[0] = 0;
   out_8242565656520005999[1] = 0;
   out_8242565656520005999[2] = 0;
   out_8242565656520005999[3] = 0;
   out_8242565656520005999[4] = 0;
   out_8242565656520005999[5] = 0;
   out_8242565656520005999[6] = 1;
   out_8242565656520005999[7] = 0;
   out_8242565656520005999[8] = 0;
   out_8242565656520005999[9] = 1;
   out_8242565656520005999[10] = 0;
   out_8242565656520005999[11] = 0;
   out_8242565656520005999[12] = 0;
   out_8242565656520005999[13] = 0;
   out_8242565656520005999[14] = 0;
   out_8242565656520005999[15] = 0;
   out_8242565656520005999[16] = 0;
   out_8242565656520005999[17] = 0;
   out_8242565656520005999[18] = 0;
   out_8242565656520005999[19] = 0;
   out_8242565656520005999[20] = 0;
   out_8242565656520005999[21] = 0;
   out_8242565656520005999[22] = 0;
   out_8242565656520005999[23] = 0;
   out_8242565656520005999[24] = 0;
   out_8242565656520005999[25] = 1;
   out_8242565656520005999[26] = 0;
   out_8242565656520005999[27] = 0;
   out_8242565656520005999[28] = 1;
   out_8242565656520005999[29] = 0;
   out_8242565656520005999[30] = 0;
   out_8242565656520005999[31] = 0;
   out_8242565656520005999[32] = 0;
   out_8242565656520005999[33] = 0;
   out_8242565656520005999[34] = 0;
   out_8242565656520005999[35] = 0;
   out_8242565656520005999[36] = 0;
   out_8242565656520005999[37] = 0;
   out_8242565656520005999[38] = 0;
   out_8242565656520005999[39] = 0;
   out_8242565656520005999[40] = 0;
   out_8242565656520005999[41] = 0;
   out_8242565656520005999[42] = 0;
   out_8242565656520005999[43] = 0;
   out_8242565656520005999[44] = 1;
   out_8242565656520005999[45] = 0;
   out_8242565656520005999[46] = 0;
   out_8242565656520005999[47] = 1;
   out_8242565656520005999[48] = 0;
   out_8242565656520005999[49] = 0;
   out_8242565656520005999[50] = 0;
   out_8242565656520005999[51] = 0;
   out_8242565656520005999[52] = 0;
   out_8242565656520005999[53] = 0;
}
void h_10(double *state, double *unused, double *out_3738960453134783073) {
   out_3738960453134783073[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_3738960453134783073[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_3738960453134783073[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_1772545011172825092) {
   out_1772545011172825092[0] = 0;
   out_1772545011172825092[1] = 9.8100000000000005*cos(state[1]);
   out_1772545011172825092[2] = 0;
   out_1772545011172825092[3] = 0;
   out_1772545011172825092[4] = -state[8];
   out_1772545011172825092[5] = state[7];
   out_1772545011172825092[6] = 0;
   out_1772545011172825092[7] = state[5];
   out_1772545011172825092[8] = -state[4];
   out_1772545011172825092[9] = 0;
   out_1772545011172825092[10] = 0;
   out_1772545011172825092[11] = 0;
   out_1772545011172825092[12] = 1;
   out_1772545011172825092[13] = 0;
   out_1772545011172825092[14] = 0;
   out_1772545011172825092[15] = 1;
   out_1772545011172825092[16] = 0;
   out_1772545011172825092[17] = 0;
   out_1772545011172825092[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_1772545011172825092[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_1772545011172825092[20] = 0;
   out_1772545011172825092[21] = state[8];
   out_1772545011172825092[22] = 0;
   out_1772545011172825092[23] = -state[6];
   out_1772545011172825092[24] = -state[5];
   out_1772545011172825092[25] = 0;
   out_1772545011172825092[26] = state[3];
   out_1772545011172825092[27] = 0;
   out_1772545011172825092[28] = 0;
   out_1772545011172825092[29] = 0;
   out_1772545011172825092[30] = 0;
   out_1772545011172825092[31] = 1;
   out_1772545011172825092[32] = 0;
   out_1772545011172825092[33] = 0;
   out_1772545011172825092[34] = 1;
   out_1772545011172825092[35] = 0;
   out_1772545011172825092[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_1772545011172825092[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_1772545011172825092[38] = 0;
   out_1772545011172825092[39] = -state[7];
   out_1772545011172825092[40] = state[6];
   out_1772545011172825092[41] = 0;
   out_1772545011172825092[42] = state[4];
   out_1772545011172825092[43] = -state[3];
   out_1772545011172825092[44] = 0;
   out_1772545011172825092[45] = 0;
   out_1772545011172825092[46] = 0;
   out_1772545011172825092[47] = 0;
   out_1772545011172825092[48] = 0;
   out_1772545011172825092[49] = 0;
   out_1772545011172825092[50] = 1;
   out_1772545011172825092[51] = 0;
   out_1772545011172825092[52] = 0;
   out_1772545011172825092[53] = 1;
}
void h_13(double *state, double *unused, double *out_2179608341068223281) {
   out_2179608341068223281[0] = state[3];
   out_2179608341068223281[1] = state[4];
   out_2179608341068223281[2] = state[5];
}
void H_13(double *state, double *unused, double *out_6991904591857212816) {
   out_6991904591857212816[0] = 0;
   out_6991904591857212816[1] = 0;
   out_6991904591857212816[2] = 0;
   out_6991904591857212816[3] = 1;
   out_6991904591857212816[4] = 0;
   out_6991904591857212816[5] = 0;
   out_6991904591857212816[6] = 0;
   out_6991904591857212816[7] = 0;
   out_6991904591857212816[8] = 0;
   out_6991904591857212816[9] = 0;
   out_6991904591857212816[10] = 0;
   out_6991904591857212816[11] = 0;
   out_6991904591857212816[12] = 0;
   out_6991904591857212816[13] = 0;
   out_6991904591857212816[14] = 0;
   out_6991904591857212816[15] = 0;
   out_6991904591857212816[16] = 0;
   out_6991904591857212816[17] = 0;
   out_6991904591857212816[18] = 0;
   out_6991904591857212816[19] = 0;
   out_6991904591857212816[20] = 0;
   out_6991904591857212816[21] = 0;
   out_6991904591857212816[22] = 1;
   out_6991904591857212816[23] = 0;
   out_6991904591857212816[24] = 0;
   out_6991904591857212816[25] = 0;
   out_6991904591857212816[26] = 0;
   out_6991904591857212816[27] = 0;
   out_6991904591857212816[28] = 0;
   out_6991904591857212816[29] = 0;
   out_6991904591857212816[30] = 0;
   out_6991904591857212816[31] = 0;
   out_6991904591857212816[32] = 0;
   out_6991904591857212816[33] = 0;
   out_6991904591857212816[34] = 0;
   out_6991904591857212816[35] = 0;
   out_6991904591857212816[36] = 0;
   out_6991904591857212816[37] = 0;
   out_6991904591857212816[38] = 0;
   out_6991904591857212816[39] = 0;
   out_6991904591857212816[40] = 0;
   out_6991904591857212816[41] = 1;
   out_6991904591857212816[42] = 0;
   out_6991904591857212816[43] = 0;
   out_6991904591857212816[44] = 0;
   out_6991904591857212816[45] = 0;
   out_6991904591857212816[46] = 0;
   out_6991904591857212816[47] = 0;
   out_6991904591857212816[48] = 0;
   out_6991904591857212816[49] = 0;
   out_6991904591857212816[50] = 0;
   out_6991904591857212816[51] = 0;
   out_6991904591857212816[52] = 0;
   out_6991904591857212816[53] = 0;
}
void h_14(double *state, double *unused, double *out_3603750172356241210) {
   out_3603750172356241210[0] = state[6];
   out_3603750172356241210[1] = state[7];
   out_3603750172356241210[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6240937560850061088) {
   out_6240937560850061088[0] = 0;
   out_6240937560850061088[1] = 0;
   out_6240937560850061088[2] = 0;
   out_6240937560850061088[3] = 0;
   out_6240937560850061088[4] = 0;
   out_6240937560850061088[5] = 0;
   out_6240937560850061088[6] = 1;
   out_6240937560850061088[7] = 0;
   out_6240937560850061088[8] = 0;
   out_6240937560850061088[9] = 0;
   out_6240937560850061088[10] = 0;
   out_6240937560850061088[11] = 0;
   out_6240937560850061088[12] = 0;
   out_6240937560850061088[13] = 0;
   out_6240937560850061088[14] = 0;
   out_6240937560850061088[15] = 0;
   out_6240937560850061088[16] = 0;
   out_6240937560850061088[17] = 0;
   out_6240937560850061088[18] = 0;
   out_6240937560850061088[19] = 0;
   out_6240937560850061088[20] = 0;
   out_6240937560850061088[21] = 0;
   out_6240937560850061088[22] = 0;
   out_6240937560850061088[23] = 0;
   out_6240937560850061088[24] = 0;
   out_6240937560850061088[25] = 1;
   out_6240937560850061088[26] = 0;
   out_6240937560850061088[27] = 0;
   out_6240937560850061088[28] = 0;
   out_6240937560850061088[29] = 0;
   out_6240937560850061088[30] = 0;
   out_6240937560850061088[31] = 0;
   out_6240937560850061088[32] = 0;
   out_6240937560850061088[33] = 0;
   out_6240937560850061088[34] = 0;
   out_6240937560850061088[35] = 0;
   out_6240937560850061088[36] = 0;
   out_6240937560850061088[37] = 0;
   out_6240937560850061088[38] = 0;
   out_6240937560850061088[39] = 0;
   out_6240937560850061088[40] = 0;
   out_6240937560850061088[41] = 0;
   out_6240937560850061088[42] = 0;
   out_6240937560850061088[43] = 0;
   out_6240937560850061088[44] = 1;
   out_6240937560850061088[45] = 0;
   out_6240937560850061088[46] = 0;
   out_6240937560850061088[47] = 0;
   out_6240937560850061088[48] = 0;
   out_6240937560850061088[49] = 0;
   out_6240937560850061088[50] = 0;
   out_6240937560850061088[51] = 0;
   out_6240937560850061088[52] = 0;
   out_6240937560850061088[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_2613128383328169093) {
  err_fun(nom_x, delta_x, out_2613128383328169093);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2805699213723397762) {
  inv_err_fun(nom_x, true_x, out_2805699213723397762);
}
void pose_H_mod_fun(double *state, double *out_4356096610437586896) {
  H_mod_fun(state, out_4356096610437586896);
}
void pose_f_fun(double *state, double dt, double *out_838605051751895038) {
  f_fun(state,  dt, out_838605051751895038);
}
void pose_F_fun(double *state, double dt, double *out_4469587873875989476) {
  F_fun(state,  dt, out_4469587873875989476);
}
void pose_h_4(double *state, double *unused, double *out_3292015623268015671) {
  h_4(state, unused, out_3292015623268015671);
}
void pose_H_4(double *state, double *unused, double *out_8242565656520005999) {
  H_4(state, unused, out_8242565656520005999);
}
void pose_h_10(double *state, double *unused, double *out_3738960453134783073) {
  h_10(state, unused, out_3738960453134783073);
}
void pose_H_10(double *state, double *unused, double *out_1772545011172825092) {
  H_10(state, unused, out_1772545011172825092);
}
void pose_h_13(double *state, double *unused, double *out_2179608341068223281) {
  h_13(state, unused, out_2179608341068223281);
}
void pose_H_13(double *state, double *unused, double *out_6991904591857212816) {
  H_13(state, unused, out_6991904591857212816);
}
void pose_h_14(double *state, double *unused, double *out_3603750172356241210) {
  h_14(state, unused, out_3603750172356241210);
}
void pose_H_14(double *state, double *unused, double *out_6240937560850061088) {
  H_14(state, unused, out_6240937560850061088);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
