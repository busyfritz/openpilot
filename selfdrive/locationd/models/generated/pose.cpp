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
void err_fun(double *nom_x, double *delta_x, double *out_4278397644236826764) {
   out_4278397644236826764[0] = delta_x[0] + nom_x[0];
   out_4278397644236826764[1] = delta_x[1] + nom_x[1];
   out_4278397644236826764[2] = delta_x[2] + nom_x[2];
   out_4278397644236826764[3] = delta_x[3] + nom_x[3];
   out_4278397644236826764[4] = delta_x[4] + nom_x[4];
   out_4278397644236826764[5] = delta_x[5] + nom_x[5];
   out_4278397644236826764[6] = delta_x[6] + nom_x[6];
   out_4278397644236826764[7] = delta_x[7] + nom_x[7];
   out_4278397644236826764[8] = delta_x[8] + nom_x[8];
   out_4278397644236826764[9] = delta_x[9] + nom_x[9];
   out_4278397644236826764[10] = delta_x[10] + nom_x[10];
   out_4278397644236826764[11] = delta_x[11] + nom_x[11];
   out_4278397644236826764[12] = delta_x[12] + nom_x[12];
   out_4278397644236826764[13] = delta_x[13] + nom_x[13];
   out_4278397644236826764[14] = delta_x[14] + nom_x[14];
   out_4278397644236826764[15] = delta_x[15] + nom_x[15];
   out_4278397644236826764[16] = delta_x[16] + nom_x[16];
   out_4278397644236826764[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8989697841247581240) {
   out_8989697841247581240[0] = -nom_x[0] + true_x[0];
   out_8989697841247581240[1] = -nom_x[1] + true_x[1];
   out_8989697841247581240[2] = -nom_x[2] + true_x[2];
   out_8989697841247581240[3] = -nom_x[3] + true_x[3];
   out_8989697841247581240[4] = -nom_x[4] + true_x[4];
   out_8989697841247581240[5] = -nom_x[5] + true_x[5];
   out_8989697841247581240[6] = -nom_x[6] + true_x[6];
   out_8989697841247581240[7] = -nom_x[7] + true_x[7];
   out_8989697841247581240[8] = -nom_x[8] + true_x[8];
   out_8989697841247581240[9] = -nom_x[9] + true_x[9];
   out_8989697841247581240[10] = -nom_x[10] + true_x[10];
   out_8989697841247581240[11] = -nom_x[11] + true_x[11];
   out_8989697841247581240[12] = -nom_x[12] + true_x[12];
   out_8989697841247581240[13] = -nom_x[13] + true_x[13];
   out_8989697841247581240[14] = -nom_x[14] + true_x[14];
   out_8989697841247581240[15] = -nom_x[15] + true_x[15];
   out_8989697841247581240[16] = -nom_x[16] + true_x[16];
   out_8989697841247581240[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_2984951409191049856) {
   out_2984951409191049856[0] = 1.0;
   out_2984951409191049856[1] = 0.0;
   out_2984951409191049856[2] = 0.0;
   out_2984951409191049856[3] = 0.0;
   out_2984951409191049856[4] = 0.0;
   out_2984951409191049856[5] = 0.0;
   out_2984951409191049856[6] = 0.0;
   out_2984951409191049856[7] = 0.0;
   out_2984951409191049856[8] = 0.0;
   out_2984951409191049856[9] = 0.0;
   out_2984951409191049856[10] = 0.0;
   out_2984951409191049856[11] = 0.0;
   out_2984951409191049856[12] = 0.0;
   out_2984951409191049856[13] = 0.0;
   out_2984951409191049856[14] = 0.0;
   out_2984951409191049856[15] = 0.0;
   out_2984951409191049856[16] = 0.0;
   out_2984951409191049856[17] = 0.0;
   out_2984951409191049856[18] = 0.0;
   out_2984951409191049856[19] = 1.0;
   out_2984951409191049856[20] = 0.0;
   out_2984951409191049856[21] = 0.0;
   out_2984951409191049856[22] = 0.0;
   out_2984951409191049856[23] = 0.0;
   out_2984951409191049856[24] = 0.0;
   out_2984951409191049856[25] = 0.0;
   out_2984951409191049856[26] = 0.0;
   out_2984951409191049856[27] = 0.0;
   out_2984951409191049856[28] = 0.0;
   out_2984951409191049856[29] = 0.0;
   out_2984951409191049856[30] = 0.0;
   out_2984951409191049856[31] = 0.0;
   out_2984951409191049856[32] = 0.0;
   out_2984951409191049856[33] = 0.0;
   out_2984951409191049856[34] = 0.0;
   out_2984951409191049856[35] = 0.0;
   out_2984951409191049856[36] = 0.0;
   out_2984951409191049856[37] = 0.0;
   out_2984951409191049856[38] = 1.0;
   out_2984951409191049856[39] = 0.0;
   out_2984951409191049856[40] = 0.0;
   out_2984951409191049856[41] = 0.0;
   out_2984951409191049856[42] = 0.0;
   out_2984951409191049856[43] = 0.0;
   out_2984951409191049856[44] = 0.0;
   out_2984951409191049856[45] = 0.0;
   out_2984951409191049856[46] = 0.0;
   out_2984951409191049856[47] = 0.0;
   out_2984951409191049856[48] = 0.0;
   out_2984951409191049856[49] = 0.0;
   out_2984951409191049856[50] = 0.0;
   out_2984951409191049856[51] = 0.0;
   out_2984951409191049856[52] = 0.0;
   out_2984951409191049856[53] = 0.0;
   out_2984951409191049856[54] = 0.0;
   out_2984951409191049856[55] = 0.0;
   out_2984951409191049856[56] = 0.0;
   out_2984951409191049856[57] = 1.0;
   out_2984951409191049856[58] = 0.0;
   out_2984951409191049856[59] = 0.0;
   out_2984951409191049856[60] = 0.0;
   out_2984951409191049856[61] = 0.0;
   out_2984951409191049856[62] = 0.0;
   out_2984951409191049856[63] = 0.0;
   out_2984951409191049856[64] = 0.0;
   out_2984951409191049856[65] = 0.0;
   out_2984951409191049856[66] = 0.0;
   out_2984951409191049856[67] = 0.0;
   out_2984951409191049856[68] = 0.0;
   out_2984951409191049856[69] = 0.0;
   out_2984951409191049856[70] = 0.0;
   out_2984951409191049856[71] = 0.0;
   out_2984951409191049856[72] = 0.0;
   out_2984951409191049856[73] = 0.0;
   out_2984951409191049856[74] = 0.0;
   out_2984951409191049856[75] = 0.0;
   out_2984951409191049856[76] = 1.0;
   out_2984951409191049856[77] = 0.0;
   out_2984951409191049856[78] = 0.0;
   out_2984951409191049856[79] = 0.0;
   out_2984951409191049856[80] = 0.0;
   out_2984951409191049856[81] = 0.0;
   out_2984951409191049856[82] = 0.0;
   out_2984951409191049856[83] = 0.0;
   out_2984951409191049856[84] = 0.0;
   out_2984951409191049856[85] = 0.0;
   out_2984951409191049856[86] = 0.0;
   out_2984951409191049856[87] = 0.0;
   out_2984951409191049856[88] = 0.0;
   out_2984951409191049856[89] = 0.0;
   out_2984951409191049856[90] = 0.0;
   out_2984951409191049856[91] = 0.0;
   out_2984951409191049856[92] = 0.0;
   out_2984951409191049856[93] = 0.0;
   out_2984951409191049856[94] = 0.0;
   out_2984951409191049856[95] = 1.0;
   out_2984951409191049856[96] = 0.0;
   out_2984951409191049856[97] = 0.0;
   out_2984951409191049856[98] = 0.0;
   out_2984951409191049856[99] = 0.0;
   out_2984951409191049856[100] = 0.0;
   out_2984951409191049856[101] = 0.0;
   out_2984951409191049856[102] = 0.0;
   out_2984951409191049856[103] = 0.0;
   out_2984951409191049856[104] = 0.0;
   out_2984951409191049856[105] = 0.0;
   out_2984951409191049856[106] = 0.0;
   out_2984951409191049856[107] = 0.0;
   out_2984951409191049856[108] = 0.0;
   out_2984951409191049856[109] = 0.0;
   out_2984951409191049856[110] = 0.0;
   out_2984951409191049856[111] = 0.0;
   out_2984951409191049856[112] = 0.0;
   out_2984951409191049856[113] = 0.0;
   out_2984951409191049856[114] = 1.0;
   out_2984951409191049856[115] = 0.0;
   out_2984951409191049856[116] = 0.0;
   out_2984951409191049856[117] = 0.0;
   out_2984951409191049856[118] = 0.0;
   out_2984951409191049856[119] = 0.0;
   out_2984951409191049856[120] = 0.0;
   out_2984951409191049856[121] = 0.0;
   out_2984951409191049856[122] = 0.0;
   out_2984951409191049856[123] = 0.0;
   out_2984951409191049856[124] = 0.0;
   out_2984951409191049856[125] = 0.0;
   out_2984951409191049856[126] = 0.0;
   out_2984951409191049856[127] = 0.0;
   out_2984951409191049856[128] = 0.0;
   out_2984951409191049856[129] = 0.0;
   out_2984951409191049856[130] = 0.0;
   out_2984951409191049856[131] = 0.0;
   out_2984951409191049856[132] = 0.0;
   out_2984951409191049856[133] = 1.0;
   out_2984951409191049856[134] = 0.0;
   out_2984951409191049856[135] = 0.0;
   out_2984951409191049856[136] = 0.0;
   out_2984951409191049856[137] = 0.0;
   out_2984951409191049856[138] = 0.0;
   out_2984951409191049856[139] = 0.0;
   out_2984951409191049856[140] = 0.0;
   out_2984951409191049856[141] = 0.0;
   out_2984951409191049856[142] = 0.0;
   out_2984951409191049856[143] = 0.0;
   out_2984951409191049856[144] = 0.0;
   out_2984951409191049856[145] = 0.0;
   out_2984951409191049856[146] = 0.0;
   out_2984951409191049856[147] = 0.0;
   out_2984951409191049856[148] = 0.0;
   out_2984951409191049856[149] = 0.0;
   out_2984951409191049856[150] = 0.0;
   out_2984951409191049856[151] = 0.0;
   out_2984951409191049856[152] = 1.0;
   out_2984951409191049856[153] = 0.0;
   out_2984951409191049856[154] = 0.0;
   out_2984951409191049856[155] = 0.0;
   out_2984951409191049856[156] = 0.0;
   out_2984951409191049856[157] = 0.0;
   out_2984951409191049856[158] = 0.0;
   out_2984951409191049856[159] = 0.0;
   out_2984951409191049856[160] = 0.0;
   out_2984951409191049856[161] = 0.0;
   out_2984951409191049856[162] = 0.0;
   out_2984951409191049856[163] = 0.0;
   out_2984951409191049856[164] = 0.0;
   out_2984951409191049856[165] = 0.0;
   out_2984951409191049856[166] = 0.0;
   out_2984951409191049856[167] = 0.0;
   out_2984951409191049856[168] = 0.0;
   out_2984951409191049856[169] = 0.0;
   out_2984951409191049856[170] = 0.0;
   out_2984951409191049856[171] = 1.0;
   out_2984951409191049856[172] = 0.0;
   out_2984951409191049856[173] = 0.0;
   out_2984951409191049856[174] = 0.0;
   out_2984951409191049856[175] = 0.0;
   out_2984951409191049856[176] = 0.0;
   out_2984951409191049856[177] = 0.0;
   out_2984951409191049856[178] = 0.0;
   out_2984951409191049856[179] = 0.0;
   out_2984951409191049856[180] = 0.0;
   out_2984951409191049856[181] = 0.0;
   out_2984951409191049856[182] = 0.0;
   out_2984951409191049856[183] = 0.0;
   out_2984951409191049856[184] = 0.0;
   out_2984951409191049856[185] = 0.0;
   out_2984951409191049856[186] = 0.0;
   out_2984951409191049856[187] = 0.0;
   out_2984951409191049856[188] = 0.0;
   out_2984951409191049856[189] = 0.0;
   out_2984951409191049856[190] = 1.0;
   out_2984951409191049856[191] = 0.0;
   out_2984951409191049856[192] = 0.0;
   out_2984951409191049856[193] = 0.0;
   out_2984951409191049856[194] = 0.0;
   out_2984951409191049856[195] = 0.0;
   out_2984951409191049856[196] = 0.0;
   out_2984951409191049856[197] = 0.0;
   out_2984951409191049856[198] = 0.0;
   out_2984951409191049856[199] = 0.0;
   out_2984951409191049856[200] = 0.0;
   out_2984951409191049856[201] = 0.0;
   out_2984951409191049856[202] = 0.0;
   out_2984951409191049856[203] = 0.0;
   out_2984951409191049856[204] = 0.0;
   out_2984951409191049856[205] = 0.0;
   out_2984951409191049856[206] = 0.0;
   out_2984951409191049856[207] = 0.0;
   out_2984951409191049856[208] = 0.0;
   out_2984951409191049856[209] = 1.0;
   out_2984951409191049856[210] = 0.0;
   out_2984951409191049856[211] = 0.0;
   out_2984951409191049856[212] = 0.0;
   out_2984951409191049856[213] = 0.0;
   out_2984951409191049856[214] = 0.0;
   out_2984951409191049856[215] = 0.0;
   out_2984951409191049856[216] = 0.0;
   out_2984951409191049856[217] = 0.0;
   out_2984951409191049856[218] = 0.0;
   out_2984951409191049856[219] = 0.0;
   out_2984951409191049856[220] = 0.0;
   out_2984951409191049856[221] = 0.0;
   out_2984951409191049856[222] = 0.0;
   out_2984951409191049856[223] = 0.0;
   out_2984951409191049856[224] = 0.0;
   out_2984951409191049856[225] = 0.0;
   out_2984951409191049856[226] = 0.0;
   out_2984951409191049856[227] = 0.0;
   out_2984951409191049856[228] = 1.0;
   out_2984951409191049856[229] = 0.0;
   out_2984951409191049856[230] = 0.0;
   out_2984951409191049856[231] = 0.0;
   out_2984951409191049856[232] = 0.0;
   out_2984951409191049856[233] = 0.0;
   out_2984951409191049856[234] = 0.0;
   out_2984951409191049856[235] = 0.0;
   out_2984951409191049856[236] = 0.0;
   out_2984951409191049856[237] = 0.0;
   out_2984951409191049856[238] = 0.0;
   out_2984951409191049856[239] = 0.0;
   out_2984951409191049856[240] = 0.0;
   out_2984951409191049856[241] = 0.0;
   out_2984951409191049856[242] = 0.0;
   out_2984951409191049856[243] = 0.0;
   out_2984951409191049856[244] = 0.0;
   out_2984951409191049856[245] = 0.0;
   out_2984951409191049856[246] = 0.0;
   out_2984951409191049856[247] = 1.0;
   out_2984951409191049856[248] = 0.0;
   out_2984951409191049856[249] = 0.0;
   out_2984951409191049856[250] = 0.0;
   out_2984951409191049856[251] = 0.0;
   out_2984951409191049856[252] = 0.0;
   out_2984951409191049856[253] = 0.0;
   out_2984951409191049856[254] = 0.0;
   out_2984951409191049856[255] = 0.0;
   out_2984951409191049856[256] = 0.0;
   out_2984951409191049856[257] = 0.0;
   out_2984951409191049856[258] = 0.0;
   out_2984951409191049856[259] = 0.0;
   out_2984951409191049856[260] = 0.0;
   out_2984951409191049856[261] = 0.0;
   out_2984951409191049856[262] = 0.0;
   out_2984951409191049856[263] = 0.0;
   out_2984951409191049856[264] = 0.0;
   out_2984951409191049856[265] = 0.0;
   out_2984951409191049856[266] = 1.0;
   out_2984951409191049856[267] = 0.0;
   out_2984951409191049856[268] = 0.0;
   out_2984951409191049856[269] = 0.0;
   out_2984951409191049856[270] = 0.0;
   out_2984951409191049856[271] = 0.0;
   out_2984951409191049856[272] = 0.0;
   out_2984951409191049856[273] = 0.0;
   out_2984951409191049856[274] = 0.0;
   out_2984951409191049856[275] = 0.0;
   out_2984951409191049856[276] = 0.0;
   out_2984951409191049856[277] = 0.0;
   out_2984951409191049856[278] = 0.0;
   out_2984951409191049856[279] = 0.0;
   out_2984951409191049856[280] = 0.0;
   out_2984951409191049856[281] = 0.0;
   out_2984951409191049856[282] = 0.0;
   out_2984951409191049856[283] = 0.0;
   out_2984951409191049856[284] = 0.0;
   out_2984951409191049856[285] = 1.0;
   out_2984951409191049856[286] = 0.0;
   out_2984951409191049856[287] = 0.0;
   out_2984951409191049856[288] = 0.0;
   out_2984951409191049856[289] = 0.0;
   out_2984951409191049856[290] = 0.0;
   out_2984951409191049856[291] = 0.0;
   out_2984951409191049856[292] = 0.0;
   out_2984951409191049856[293] = 0.0;
   out_2984951409191049856[294] = 0.0;
   out_2984951409191049856[295] = 0.0;
   out_2984951409191049856[296] = 0.0;
   out_2984951409191049856[297] = 0.0;
   out_2984951409191049856[298] = 0.0;
   out_2984951409191049856[299] = 0.0;
   out_2984951409191049856[300] = 0.0;
   out_2984951409191049856[301] = 0.0;
   out_2984951409191049856[302] = 0.0;
   out_2984951409191049856[303] = 0.0;
   out_2984951409191049856[304] = 1.0;
   out_2984951409191049856[305] = 0.0;
   out_2984951409191049856[306] = 0.0;
   out_2984951409191049856[307] = 0.0;
   out_2984951409191049856[308] = 0.0;
   out_2984951409191049856[309] = 0.0;
   out_2984951409191049856[310] = 0.0;
   out_2984951409191049856[311] = 0.0;
   out_2984951409191049856[312] = 0.0;
   out_2984951409191049856[313] = 0.0;
   out_2984951409191049856[314] = 0.0;
   out_2984951409191049856[315] = 0.0;
   out_2984951409191049856[316] = 0.0;
   out_2984951409191049856[317] = 0.0;
   out_2984951409191049856[318] = 0.0;
   out_2984951409191049856[319] = 0.0;
   out_2984951409191049856[320] = 0.0;
   out_2984951409191049856[321] = 0.0;
   out_2984951409191049856[322] = 0.0;
   out_2984951409191049856[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5963344649312491371) {
   out_5963344649312491371[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5963344649312491371[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5963344649312491371[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5963344649312491371[3] = dt*state[12] + state[3];
   out_5963344649312491371[4] = dt*state[13] + state[4];
   out_5963344649312491371[5] = dt*state[14] + state[5];
   out_5963344649312491371[6] = state[6];
   out_5963344649312491371[7] = state[7];
   out_5963344649312491371[8] = state[8];
   out_5963344649312491371[9] = state[9];
   out_5963344649312491371[10] = state[10];
   out_5963344649312491371[11] = state[11];
   out_5963344649312491371[12] = state[12];
   out_5963344649312491371[13] = state[13];
   out_5963344649312491371[14] = state[14];
   out_5963344649312491371[15] = state[15];
   out_5963344649312491371[16] = state[16];
   out_5963344649312491371[17] = state[17];
}
void F_fun(double *state, double dt, double *out_4433429895516971321) {
   out_4433429895516971321[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4433429895516971321[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4433429895516971321[2] = 0;
   out_4433429895516971321[3] = 0;
   out_4433429895516971321[4] = 0;
   out_4433429895516971321[5] = 0;
   out_4433429895516971321[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4433429895516971321[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4433429895516971321[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4433429895516971321[9] = 0;
   out_4433429895516971321[10] = 0;
   out_4433429895516971321[11] = 0;
   out_4433429895516971321[12] = 0;
   out_4433429895516971321[13] = 0;
   out_4433429895516971321[14] = 0;
   out_4433429895516971321[15] = 0;
   out_4433429895516971321[16] = 0;
   out_4433429895516971321[17] = 0;
   out_4433429895516971321[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4433429895516971321[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4433429895516971321[20] = 0;
   out_4433429895516971321[21] = 0;
   out_4433429895516971321[22] = 0;
   out_4433429895516971321[23] = 0;
   out_4433429895516971321[24] = 0;
   out_4433429895516971321[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4433429895516971321[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4433429895516971321[27] = 0;
   out_4433429895516971321[28] = 0;
   out_4433429895516971321[29] = 0;
   out_4433429895516971321[30] = 0;
   out_4433429895516971321[31] = 0;
   out_4433429895516971321[32] = 0;
   out_4433429895516971321[33] = 0;
   out_4433429895516971321[34] = 0;
   out_4433429895516971321[35] = 0;
   out_4433429895516971321[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4433429895516971321[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4433429895516971321[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4433429895516971321[39] = 0;
   out_4433429895516971321[40] = 0;
   out_4433429895516971321[41] = 0;
   out_4433429895516971321[42] = 0;
   out_4433429895516971321[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4433429895516971321[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4433429895516971321[45] = 0;
   out_4433429895516971321[46] = 0;
   out_4433429895516971321[47] = 0;
   out_4433429895516971321[48] = 0;
   out_4433429895516971321[49] = 0;
   out_4433429895516971321[50] = 0;
   out_4433429895516971321[51] = 0;
   out_4433429895516971321[52] = 0;
   out_4433429895516971321[53] = 0;
   out_4433429895516971321[54] = 0;
   out_4433429895516971321[55] = 0;
   out_4433429895516971321[56] = 0;
   out_4433429895516971321[57] = 1;
   out_4433429895516971321[58] = 0;
   out_4433429895516971321[59] = 0;
   out_4433429895516971321[60] = 0;
   out_4433429895516971321[61] = 0;
   out_4433429895516971321[62] = 0;
   out_4433429895516971321[63] = 0;
   out_4433429895516971321[64] = 0;
   out_4433429895516971321[65] = 0;
   out_4433429895516971321[66] = dt;
   out_4433429895516971321[67] = 0;
   out_4433429895516971321[68] = 0;
   out_4433429895516971321[69] = 0;
   out_4433429895516971321[70] = 0;
   out_4433429895516971321[71] = 0;
   out_4433429895516971321[72] = 0;
   out_4433429895516971321[73] = 0;
   out_4433429895516971321[74] = 0;
   out_4433429895516971321[75] = 0;
   out_4433429895516971321[76] = 1;
   out_4433429895516971321[77] = 0;
   out_4433429895516971321[78] = 0;
   out_4433429895516971321[79] = 0;
   out_4433429895516971321[80] = 0;
   out_4433429895516971321[81] = 0;
   out_4433429895516971321[82] = 0;
   out_4433429895516971321[83] = 0;
   out_4433429895516971321[84] = 0;
   out_4433429895516971321[85] = dt;
   out_4433429895516971321[86] = 0;
   out_4433429895516971321[87] = 0;
   out_4433429895516971321[88] = 0;
   out_4433429895516971321[89] = 0;
   out_4433429895516971321[90] = 0;
   out_4433429895516971321[91] = 0;
   out_4433429895516971321[92] = 0;
   out_4433429895516971321[93] = 0;
   out_4433429895516971321[94] = 0;
   out_4433429895516971321[95] = 1;
   out_4433429895516971321[96] = 0;
   out_4433429895516971321[97] = 0;
   out_4433429895516971321[98] = 0;
   out_4433429895516971321[99] = 0;
   out_4433429895516971321[100] = 0;
   out_4433429895516971321[101] = 0;
   out_4433429895516971321[102] = 0;
   out_4433429895516971321[103] = 0;
   out_4433429895516971321[104] = dt;
   out_4433429895516971321[105] = 0;
   out_4433429895516971321[106] = 0;
   out_4433429895516971321[107] = 0;
   out_4433429895516971321[108] = 0;
   out_4433429895516971321[109] = 0;
   out_4433429895516971321[110] = 0;
   out_4433429895516971321[111] = 0;
   out_4433429895516971321[112] = 0;
   out_4433429895516971321[113] = 0;
   out_4433429895516971321[114] = 1;
   out_4433429895516971321[115] = 0;
   out_4433429895516971321[116] = 0;
   out_4433429895516971321[117] = 0;
   out_4433429895516971321[118] = 0;
   out_4433429895516971321[119] = 0;
   out_4433429895516971321[120] = 0;
   out_4433429895516971321[121] = 0;
   out_4433429895516971321[122] = 0;
   out_4433429895516971321[123] = 0;
   out_4433429895516971321[124] = 0;
   out_4433429895516971321[125] = 0;
   out_4433429895516971321[126] = 0;
   out_4433429895516971321[127] = 0;
   out_4433429895516971321[128] = 0;
   out_4433429895516971321[129] = 0;
   out_4433429895516971321[130] = 0;
   out_4433429895516971321[131] = 0;
   out_4433429895516971321[132] = 0;
   out_4433429895516971321[133] = 1;
   out_4433429895516971321[134] = 0;
   out_4433429895516971321[135] = 0;
   out_4433429895516971321[136] = 0;
   out_4433429895516971321[137] = 0;
   out_4433429895516971321[138] = 0;
   out_4433429895516971321[139] = 0;
   out_4433429895516971321[140] = 0;
   out_4433429895516971321[141] = 0;
   out_4433429895516971321[142] = 0;
   out_4433429895516971321[143] = 0;
   out_4433429895516971321[144] = 0;
   out_4433429895516971321[145] = 0;
   out_4433429895516971321[146] = 0;
   out_4433429895516971321[147] = 0;
   out_4433429895516971321[148] = 0;
   out_4433429895516971321[149] = 0;
   out_4433429895516971321[150] = 0;
   out_4433429895516971321[151] = 0;
   out_4433429895516971321[152] = 1;
   out_4433429895516971321[153] = 0;
   out_4433429895516971321[154] = 0;
   out_4433429895516971321[155] = 0;
   out_4433429895516971321[156] = 0;
   out_4433429895516971321[157] = 0;
   out_4433429895516971321[158] = 0;
   out_4433429895516971321[159] = 0;
   out_4433429895516971321[160] = 0;
   out_4433429895516971321[161] = 0;
   out_4433429895516971321[162] = 0;
   out_4433429895516971321[163] = 0;
   out_4433429895516971321[164] = 0;
   out_4433429895516971321[165] = 0;
   out_4433429895516971321[166] = 0;
   out_4433429895516971321[167] = 0;
   out_4433429895516971321[168] = 0;
   out_4433429895516971321[169] = 0;
   out_4433429895516971321[170] = 0;
   out_4433429895516971321[171] = 1;
   out_4433429895516971321[172] = 0;
   out_4433429895516971321[173] = 0;
   out_4433429895516971321[174] = 0;
   out_4433429895516971321[175] = 0;
   out_4433429895516971321[176] = 0;
   out_4433429895516971321[177] = 0;
   out_4433429895516971321[178] = 0;
   out_4433429895516971321[179] = 0;
   out_4433429895516971321[180] = 0;
   out_4433429895516971321[181] = 0;
   out_4433429895516971321[182] = 0;
   out_4433429895516971321[183] = 0;
   out_4433429895516971321[184] = 0;
   out_4433429895516971321[185] = 0;
   out_4433429895516971321[186] = 0;
   out_4433429895516971321[187] = 0;
   out_4433429895516971321[188] = 0;
   out_4433429895516971321[189] = 0;
   out_4433429895516971321[190] = 1;
   out_4433429895516971321[191] = 0;
   out_4433429895516971321[192] = 0;
   out_4433429895516971321[193] = 0;
   out_4433429895516971321[194] = 0;
   out_4433429895516971321[195] = 0;
   out_4433429895516971321[196] = 0;
   out_4433429895516971321[197] = 0;
   out_4433429895516971321[198] = 0;
   out_4433429895516971321[199] = 0;
   out_4433429895516971321[200] = 0;
   out_4433429895516971321[201] = 0;
   out_4433429895516971321[202] = 0;
   out_4433429895516971321[203] = 0;
   out_4433429895516971321[204] = 0;
   out_4433429895516971321[205] = 0;
   out_4433429895516971321[206] = 0;
   out_4433429895516971321[207] = 0;
   out_4433429895516971321[208] = 0;
   out_4433429895516971321[209] = 1;
   out_4433429895516971321[210] = 0;
   out_4433429895516971321[211] = 0;
   out_4433429895516971321[212] = 0;
   out_4433429895516971321[213] = 0;
   out_4433429895516971321[214] = 0;
   out_4433429895516971321[215] = 0;
   out_4433429895516971321[216] = 0;
   out_4433429895516971321[217] = 0;
   out_4433429895516971321[218] = 0;
   out_4433429895516971321[219] = 0;
   out_4433429895516971321[220] = 0;
   out_4433429895516971321[221] = 0;
   out_4433429895516971321[222] = 0;
   out_4433429895516971321[223] = 0;
   out_4433429895516971321[224] = 0;
   out_4433429895516971321[225] = 0;
   out_4433429895516971321[226] = 0;
   out_4433429895516971321[227] = 0;
   out_4433429895516971321[228] = 1;
   out_4433429895516971321[229] = 0;
   out_4433429895516971321[230] = 0;
   out_4433429895516971321[231] = 0;
   out_4433429895516971321[232] = 0;
   out_4433429895516971321[233] = 0;
   out_4433429895516971321[234] = 0;
   out_4433429895516971321[235] = 0;
   out_4433429895516971321[236] = 0;
   out_4433429895516971321[237] = 0;
   out_4433429895516971321[238] = 0;
   out_4433429895516971321[239] = 0;
   out_4433429895516971321[240] = 0;
   out_4433429895516971321[241] = 0;
   out_4433429895516971321[242] = 0;
   out_4433429895516971321[243] = 0;
   out_4433429895516971321[244] = 0;
   out_4433429895516971321[245] = 0;
   out_4433429895516971321[246] = 0;
   out_4433429895516971321[247] = 1;
   out_4433429895516971321[248] = 0;
   out_4433429895516971321[249] = 0;
   out_4433429895516971321[250] = 0;
   out_4433429895516971321[251] = 0;
   out_4433429895516971321[252] = 0;
   out_4433429895516971321[253] = 0;
   out_4433429895516971321[254] = 0;
   out_4433429895516971321[255] = 0;
   out_4433429895516971321[256] = 0;
   out_4433429895516971321[257] = 0;
   out_4433429895516971321[258] = 0;
   out_4433429895516971321[259] = 0;
   out_4433429895516971321[260] = 0;
   out_4433429895516971321[261] = 0;
   out_4433429895516971321[262] = 0;
   out_4433429895516971321[263] = 0;
   out_4433429895516971321[264] = 0;
   out_4433429895516971321[265] = 0;
   out_4433429895516971321[266] = 1;
   out_4433429895516971321[267] = 0;
   out_4433429895516971321[268] = 0;
   out_4433429895516971321[269] = 0;
   out_4433429895516971321[270] = 0;
   out_4433429895516971321[271] = 0;
   out_4433429895516971321[272] = 0;
   out_4433429895516971321[273] = 0;
   out_4433429895516971321[274] = 0;
   out_4433429895516971321[275] = 0;
   out_4433429895516971321[276] = 0;
   out_4433429895516971321[277] = 0;
   out_4433429895516971321[278] = 0;
   out_4433429895516971321[279] = 0;
   out_4433429895516971321[280] = 0;
   out_4433429895516971321[281] = 0;
   out_4433429895516971321[282] = 0;
   out_4433429895516971321[283] = 0;
   out_4433429895516971321[284] = 0;
   out_4433429895516971321[285] = 1;
   out_4433429895516971321[286] = 0;
   out_4433429895516971321[287] = 0;
   out_4433429895516971321[288] = 0;
   out_4433429895516971321[289] = 0;
   out_4433429895516971321[290] = 0;
   out_4433429895516971321[291] = 0;
   out_4433429895516971321[292] = 0;
   out_4433429895516971321[293] = 0;
   out_4433429895516971321[294] = 0;
   out_4433429895516971321[295] = 0;
   out_4433429895516971321[296] = 0;
   out_4433429895516971321[297] = 0;
   out_4433429895516971321[298] = 0;
   out_4433429895516971321[299] = 0;
   out_4433429895516971321[300] = 0;
   out_4433429895516971321[301] = 0;
   out_4433429895516971321[302] = 0;
   out_4433429895516971321[303] = 0;
   out_4433429895516971321[304] = 1;
   out_4433429895516971321[305] = 0;
   out_4433429895516971321[306] = 0;
   out_4433429895516971321[307] = 0;
   out_4433429895516971321[308] = 0;
   out_4433429895516971321[309] = 0;
   out_4433429895516971321[310] = 0;
   out_4433429895516971321[311] = 0;
   out_4433429895516971321[312] = 0;
   out_4433429895516971321[313] = 0;
   out_4433429895516971321[314] = 0;
   out_4433429895516971321[315] = 0;
   out_4433429895516971321[316] = 0;
   out_4433429895516971321[317] = 0;
   out_4433429895516971321[318] = 0;
   out_4433429895516971321[319] = 0;
   out_4433429895516971321[320] = 0;
   out_4433429895516971321[321] = 0;
   out_4433429895516971321[322] = 0;
   out_4433429895516971321[323] = 1;
}
void h_4(double *state, double *unused, double *out_8370372608422906031) {
   out_8370372608422906031[0] = state[6] + state[9];
   out_8370372608422906031[1] = state[7] + state[10];
   out_8370372608422906031[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_3739112411245762463) {
   out_3739112411245762463[0] = 0;
   out_3739112411245762463[1] = 0;
   out_3739112411245762463[2] = 0;
   out_3739112411245762463[3] = 0;
   out_3739112411245762463[4] = 0;
   out_3739112411245762463[5] = 0;
   out_3739112411245762463[6] = 1;
   out_3739112411245762463[7] = 0;
   out_3739112411245762463[8] = 0;
   out_3739112411245762463[9] = 1;
   out_3739112411245762463[10] = 0;
   out_3739112411245762463[11] = 0;
   out_3739112411245762463[12] = 0;
   out_3739112411245762463[13] = 0;
   out_3739112411245762463[14] = 0;
   out_3739112411245762463[15] = 0;
   out_3739112411245762463[16] = 0;
   out_3739112411245762463[17] = 0;
   out_3739112411245762463[18] = 0;
   out_3739112411245762463[19] = 0;
   out_3739112411245762463[20] = 0;
   out_3739112411245762463[21] = 0;
   out_3739112411245762463[22] = 0;
   out_3739112411245762463[23] = 0;
   out_3739112411245762463[24] = 0;
   out_3739112411245762463[25] = 1;
   out_3739112411245762463[26] = 0;
   out_3739112411245762463[27] = 0;
   out_3739112411245762463[28] = 1;
   out_3739112411245762463[29] = 0;
   out_3739112411245762463[30] = 0;
   out_3739112411245762463[31] = 0;
   out_3739112411245762463[32] = 0;
   out_3739112411245762463[33] = 0;
   out_3739112411245762463[34] = 0;
   out_3739112411245762463[35] = 0;
   out_3739112411245762463[36] = 0;
   out_3739112411245762463[37] = 0;
   out_3739112411245762463[38] = 0;
   out_3739112411245762463[39] = 0;
   out_3739112411245762463[40] = 0;
   out_3739112411245762463[41] = 0;
   out_3739112411245762463[42] = 0;
   out_3739112411245762463[43] = 0;
   out_3739112411245762463[44] = 1;
   out_3739112411245762463[45] = 0;
   out_3739112411245762463[46] = 0;
   out_3739112411245762463[47] = 1;
   out_3739112411245762463[48] = 0;
   out_3739112411245762463[49] = 0;
   out_3739112411245762463[50] = 0;
   out_3739112411245762463[51] = 0;
   out_3739112411245762463[52] = 0;
   out_3739112411245762463[53] = 0;
}
void h_10(double *state, double *unused, double *out_1951244009672801524) {
   out_1951244009672801524[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_1951244009672801524[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_1951244009672801524[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_431513790114911654) {
   out_431513790114911654[0] = 0;
   out_431513790114911654[1] = 9.8100000000000005*cos(state[1]);
   out_431513790114911654[2] = 0;
   out_431513790114911654[3] = 0;
   out_431513790114911654[4] = -state[8];
   out_431513790114911654[5] = state[7];
   out_431513790114911654[6] = 0;
   out_431513790114911654[7] = state[5];
   out_431513790114911654[8] = -state[4];
   out_431513790114911654[9] = 0;
   out_431513790114911654[10] = 0;
   out_431513790114911654[11] = 0;
   out_431513790114911654[12] = 1;
   out_431513790114911654[13] = 0;
   out_431513790114911654[14] = 0;
   out_431513790114911654[15] = 1;
   out_431513790114911654[16] = 0;
   out_431513790114911654[17] = 0;
   out_431513790114911654[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_431513790114911654[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_431513790114911654[20] = 0;
   out_431513790114911654[21] = state[8];
   out_431513790114911654[22] = 0;
   out_431513790114911654[23] = -state[6];
   out_431513790114911654[24] = -state[5];
   out_431513790114911654[25] = 0;
   out_431513790114911654[26] = state[3];
   out_431513790114911654[27] = 0;
   out_431513790114911654[28] = 0;
   out_431513790114911654[29] = 0;
   out_431513790114911654[30] = 0;
   out_431513790114911654[31] = 1;
   out_431513790114911654[32] = 0;
   out_431513790114911654[33] = 0;
   out_431513790114911654[34] = 1;
   out_431513790114911654[35] = 0;
   out_431513790114911654[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_431513790114911654[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_431513790114911654[38] = 0;
   out_431513790114911654[39] = -state[7];
   out_431513790114911654[40] = state[6];
   out_431513790114911654[41] = 0;
   out_431513790114911654[42] = state[4];
   out_431513790114911654[43] = -state[3];
   out_431513790114911654[44] = 0;
   out_431513790114911654[45] = 0;
   out_431513790114911654[46] = 0;
   out_431513790114911654[47] = 0;
   out_431513790114911654[48] = 0;
   out_431513790114911654[49] = 0;
   out_431513790114911654[50] = 1;
   out_431513790114911654[51] = 0;
   out_431513790114911654[52] = 0;
   out_431513790114911654[53] = 1;
}
void h_13(double *state, double *unused, double *out_6412097572544315454) {
   out_6412097572544315454[0] = state[3];
   out_6412097572544315454[1] = state[4];
   out_6412097572544315454[2] = state[5];
}
void H_13(double *state, double *unused, double *out_6951386236578095264) {
   out_6951386236578095264[0] = 0;
   out_6951386236578095264[1] = 0;
   out_6951386236578095264[2] = 0;
   out_6951386236578095264[3] = 1;
   out_6951386236578095264[4] = 0;
   out_6951386236578095264[5] = 0;
   out_6951386236578095264[6] = 0;
   out_6951386236578095264[7] = 0;
   out_6951386236578095264[8] = 0;
   out_6951386236578095264[9] = 0;
   out_6951386236578095264[10] = 0;
   out_6951386236578095264[11] = 0;
   out_6951386236578095264[12] = 0;
   out_6951386236578095264[13] = 0;
   out_6951386236578095264[14] = 0;
   out_6951386236578095264[15] = 0;
   out_6951386236578095264[16] = 0;
   out_6951386236578095264[17] = 0;
   out_6951386236578095264[18] = 0;
   out_6951386236578095264[19] = 0;
   out_6951386236578095264[20] = 0;
   out_6951386236578095264[21] = 0;
   out_6951386236578095264[22] = 1;
   out_6951386236578095264[23] = 0;
   out_6951386236578095264[24] = 0;
   out_6951386236578095264[25] = 0;
   out_6951386236578095264[26] = 0;
   out_6951386236578095264[27] = 0;
   out_6951386236578095264[28] = 0;
   out_6951386236578095264[29] = 0;
   out_6951386236578095264[30] = 0;
   out_6951386236578095264[31] = 0;
   out_6951386236578095264[32] = 0;
   out_6951386236578095264[33] = 0;
   out_6951386236578095264[34] = 0;
   out_6951386236578095264[35] = 0;
   out_6951386236578095264[36] = 0;
   out_6951386236578095264[37] = 0;
   out_6951386236578095264[38] = 0;
   out_6951386236578095264[39] = 0;
   out_6951386236578095264[40] = 0;
   out_6951386236578095264[41] = 1;
   out_6951386236578095264[42] = 0;
   out_6951386236578095264[43] = 0;
   out_6951386236578095264[44] = 0;
   out_6951386236578095264[45] = 0;
   out_6951386236578095264[46] = 0;
   out_6951386236578095264[47] = 0;
   out_6951386236578095264[48] = 0;
   out_6951386236578095264[49] = 0;
   out_6951386236578095264[50] = 0;
   out_6951386236578095264[51] = 0;
   out_6951386236578095264[52] = 0;
   out_6951386236578095264[53] = 0;
}
void h_14(double *state, double *unused, double *out_5552192962493310549) {
   out_5552192962493310549[0] = state[6];
   out_5552192962493310549[1] = state[7];
   out_5552192962493310549[2] = state[8];
}
void H_14(double *state, double *unused, double *out_656323978950390167) {
   out_656323978950390167[0] = 0;
   out_656323978950390167[1] = 0;
   out_656323978950390167[2] = 0;
   out_656323978950390167[3] = 0;
   out_656323978950390167[4] = 0;
   out_656323978950390167[5] = 0;
   out_656323978950390167[6] = 1;
   out_656323978950390167[7] = 0;
   out_656323978950390167[8] = 0;
   out_656323978950390167[9] = 0;
   out_656323978950390167[10] = 0;
   out_656323978950390167[11] = 0;
   out_656323978950390167[12] = 0;
   out_656323978950390167[13] = 0;
   out_656323978950390167[14] = 0;
   out_656323978950390167[15] = 0;
   out_656323978950390167[16] = 0;
   out_656323978950390167[17] = 0;
   out_656323978950390167[18] = 0;
   out_656323978950390167[19] = 0;
   out_656323978950390167[20] = 0;
   out_656323978950390167[21] = 0;
   out_656323978950390167[22] = 0;
   out_656323978950390167[23] = 0;
   out_656323978950390167[24] = 0;
   out_656323978950390167[25] = 1;
   out_656323978950390167[26] = 0;
   out_656323978950390167[27] = 0;
   out_656323978950390167[28] = 0;
   out_656323978950390167[29] = 0;
   out_656323978950390167[30] = 0;
   out_656323978950390167[31] = 0;
   out_656323978950390167[32] = 0;
   out_656323978950390167[33] = 0;
   out_656323978950390167[34] = 0;
   out_656323978950390167[35] = 0;
   out_656323978950390167[36] = 0;
   out_656323978950390167[37] = 0;
   out_656323978950390167[38] = 0;
   out_656323978950390167[39] = 0;
   out_656323978950390167[40] = 0;
   out_656323978950390167[41] = 0;
   out_656323978950390167[42] = 0;
   out_656323978950390167[43] = 0;
   out_656323978950390167[44] = 1;
   out_656323978950390167[45] = 0;
   out_656323978950390167[46] = 0;
   out_656323978950390167[47] = 0;
   out_656323978950390167[48] = 0;
   out_656323978950390167[49] = 0;
   out_656323978950390167[50] = 0;
   out_656323978950390167[51] = 0;
   out_656323978950390167[52] = 0;
   out_656323978950390167[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_4278397644236826764) {
  err_fun(nom_x, delta_x, out_4278397644236826764);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_8989697841247581240) {
  inv_err_fun(nom_x, true_x, out_8989697841247581240);
}
void pose_H_mod_fun(double *state, double *out_2984951409191049856) {
  H_mod_fun(state, out_2984951409191049856);
}
void pose_f_fun(double *state, double dt, double *out_5963344649312491371) {
  f_fun(state,  dt, out_5963344649312491371);
}
void pose_F_fun(double *state, double dt, double *out_4433429895516971321) {
  F_fun(state,  dt, out_4433429895516971321);
}
void pose_h_4(double *state, double *unused, double *out_8370372608422906031) {
  h_4(state, unused, out_8370372608422906031);
}
void pose_H_4(double *state, double *unused, double *out_3739112411245762463) {
  H_4(state, unused, out_3739112411245762463);
}
void pose_h_10(double *state, double *unused, double *out_1951244009672801524) {
  h_10(state, unused, out_1951244009672801524);
}
void pose_H_10(double *state, double *unused, double *out_431513790114911654) {
  H_10(state, unused, out_431513790114911654);
}
void pose_h_13(double *state, double *unused, double *out_6412097572544315454) {
  h_13(state, unused, out_6412097572544315454);
}
void pose_H_13(double *state, double *unused, double *out_6951386236578095264) {
  H_13(state, unused, out_6951386236578095264);
}
void pose_h_14(double *state, double *unused, double *out_5552192962493310549) {
  h_14(state, unused, out_5552192962493310549);
}
void pose_H_14(double *state, double *unused, double *out_656323978950390167) {
  H_14(state, unused, out_656323978950390167);
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
