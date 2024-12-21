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
void err_fun(double *nom_x, double *delta_x, double *out_8396481004364689900) {
   out_8396481004364689900[0] = delta_x[0] + nom_x[0];
   out_8396481004364689900[1] = delta_x[1] + nom_x[1];
   out_8396481004364689900[2] = delta_x[2] + nom_x[2];
   out_8396481004364689900[3] = delta_x[3] + nom_x[3];
   out_8396481004364689900[4] = delta_x[4] + nom_x[4];
   out_8396481004364689900[5] = delta_x[5] + nom_x[5];
   out_8396481004364689900[6] = delta_x[6] + nom_x[6];
   out_8396481004364689900[7] = delta_x[7] + nom_x[7];
   out_8396481004364689900[8] = delta_x[8] + nom_x[8];
   out_8396481004364689900[9] = delta_x[9] + nom_x[9];
   out_8396481004364689900[10] = delta_x[10] + nom_x[10];
   out_8396481004364689900[11] = delta_x[11] + nom_x[11];
   out_8396481004364689900[12] = delta_x[12] + nom_x[12];
   out_8396481004364689900[13] = delta_x[13] + nom_x[13];
   out_8396481004364689900[14] = delta_x[14] + nom_x[14];
   out_8396481004364689900[15] = delta_x[15] + nom_x[15];
   out_8396481004364689900[16] = delta_x[16] + nom_x[16];
   out_8396481004364689900[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6933496826796386374) {
   out_6933496826796386374[0] = -nom_x[0] + true_x[0];
   out_6933496826796386374[1] = -nom_x[1] + true_x[1];
   out_6933496826796386374[2] = -nom_x[2] + true_x[2];
   out_6933496826796386374[3] = -nom_x[3] + true_x[3];
   out_6933496826796386374[4] = -nom_x[4] + true_x[4];
   out_6933496826796386374[5] = -nom_x[5] + true_x[5];
   out_6933496826796386374[6] = -nom_x[6] + true_x[6];
   out_6933496826796386374[7] = -nom_x[7] + true_x[7];
   out_6933496826796386374[8] = -nom_x[8] + true_x[8];
   out_6933496826796386374[9] = -nom_x[9] + true_x[9];
   out_6933496826796386374[10] = -nom_x[10] + true_x[10];
   out_6933496826796386374[11] = -nom_x[11] + true_x[11];
   out_6933496826796386374[12] = -nom_x[12] + true_x[12];
   out_6933496826796386374[13] = -nom_x[13] + true_x[13];
   out_6933496826796386374[14] = -nom_x[14] + true_x[14];
   out_6933496826796386374[15] = -nom_x[15] + true_x[15];
   out_6933496826796386374[16] = -nom_x[16] + true_x[16];
   out_6933496826796386374[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_2065338275713658617) {
   out_2065338275713658617[0] = 1.0;
   out_2065338275713658617[1] = 0.0;
   out_2065338275713658617[2] = 0.0;
   out_2065338275713658617[3] = 0.0;
   out_2065338275713658617[4] = 0.0;
   out_2065338275713658617[5] = 0.0;
   out_2065338275713658617[6] = 0.0;
   out_2065338275713658617[7] = 0.0;
   out_2065338275713658617[8] = 0.0;
   out_2065338275713658617[9] = 0.0;
   out_2065338275713658617[10] = 0.0;
   out_2065338275713658617[11] = 0.0;
   out_2065338275713658617[12] = 0.0;
   out_2065338275713658617[13] = 0.0;
   out_2065338275713658617[14] = 0.0;
   out_2065338275713658617[15] = 0.0;
   out_2065338275713658617[16] = 0.0;
   out_2065338275713658617[17] = 0.0;
   out_2065338275713658617[18] = 0.0;
   out_2065338275713658617[19] = 1.0;
   out_2065338275713658617[20] = 0.0;
   out_2065338275713658617[21] = 0.0;
   out_2065338275713658617[22] = 0.0;
   out_2065338275713658617[23] = 0.0;
   out_2065338275713658617[24] = 0.0;
   out_2065338275713658617[25] = 0.0;
   out_2065338275713658617[26] = 0.0;
   out_2065338275713658617[27] = 0.0;
   out_2065338275713658617[28] = 0.0;
   out_2065338275713658617[29] = 0.0;
   out_2065338275713658617[30] = 0.0;
   out_2065338275713658617[31] = 0.0;
   out_2065338275713658617[32] = 0.0;
   out_2065338275713658617[33] = 0.0;
   out_2065338275713658617[34] = 0.0;
   out_2065338275713658617[35] = 0.0;
   out_2065338275713658617[36] = 0.0;
   out_2065338275713658617[37] = 0.0;
   out_2065338275713658617[38] = 1.0;
   out_2065338275713658617[39] = 0.0;
   out_2065338275713658617[40] = 0.0;
   out_2065338275713658617[41] = 0.0;
   out_2065338275713658617[42] = 0.0;
   out_2065338275713658617[43] = 0.0;
   out_2065338275713658617[44] = 0.0;
   out_2065338275713658617[45] = 0.0;
   out_2065338275713658617[46] = 0.0;
   out_2065338275713658617[47] = 0.0;
   out_2065338275713658617[48] = 0.0;
   out_2065338275713658617[49] = 0.0;
   out_2065338275713658617[50] = 0.0;
   out_2065338275713658617[51] = 0.0;
   out_2065338275713658617[52] = 0.0;
   out_2065338275713658617[53] = 0.0;
   out_2065338275713658617[54] = 0.0;
   out_2065338275713658617[55] = 0.0;
   out_2065338275713658617[56] = 0.0;
   out_2065338275713658617[57] = 1.0;
   out_2065338275713658617[58] = 0.0;
   out_2065338275713658617[59] = 0.0;
   out_2065338275713658617[60] = 0.0;
   out_2065338275713658617[61] = 0.0;
   out_2065338275713658617[62] = 0.0;
   out_2065338275713658617[63] = 0.0;
   out_2065338275713658617[64] = 0.0;
   out_2065338275713658617[65] = 0.0;
   out_2065338275713658617[66] = 0.0;
   out_2065338275713658617[67] = 0.0;
   out_2065338275713658617[68] = 0.0;
   out_2065338275713658617[69] = 0.0;
   out_2065338275713658617[70] = 0.0;
   out_2065338275713658617[71] = 0.0;
   out_2065338275713658617[72] = 0.0;
   out_2065338275713658617[73] = 0.0;
   out_2065338275713658617[74] = 0.0;
   out_2065338275713658617[75] = 0.0;
   out_2065338275713658617[76] = 1.0;
   out_2065338275713658617[77] = 0.0;
   out_2065338275713658617[78] = 0.0;
   out_2065338275713658617[79] = 0.0;
   out_2065338275713658617[80] = 0.0;
   out_2065338275713658617[81] = 0.0;
   out_2065338275713658617[82] = 0.0;
   out_2065338275713658617[83] = 0.0;
   out_2065338275713658617[84] = 0.0;
   out_2065338275713658617[85] = 0.0;
   out_2065338275713658617[86] = 0.0;
   out_2065338275713658617[87] = 0.0;
   out_2065338275713658617[88] = 0.0;
   out_2065338275713658617[89] = 0.0;
   out_2065338275713658617[90] = 0.0;
   out_2065338275713658617[91] = 0.0;
   out_2065338275713658617[92] = 0.0;
   out_2065338275713658617[93] = 0.0;
   out_2065338275713658617[94] = 0.0;
   out_2065338275713658617[95] = 1.0;
   out_2065338275713658617[96] = 0.0;
   out_2065338275713658617[97] = 0.0;
   out_2065338275713658617[98] = 0.0;
   out_2065338275713658617[99] = 0.0;
   out_2065338275713658617[100] = 0.0;
   out_2065338275713658617[101] = 0.0;
   out_2065338275713658617[102] = 0.0;
   out_2065338275713658617[103] = 0.0;
   out_2065338275713658617[104] = 0.0;
   out_2065338275713658617[105] = 0.0;
   out_2065338275713658617[106] = 0.0;
   out_2065338275713658617[107] = 0.0;
   out_2065338275713658617[108] = 0.0;
   out_2065338275713658617[109] = 0.0;
   out_2065338275713658617[110] = 0.0;
   out_2065338275713658617[111] = 0.0;
   out_2065338275713658617[112] = 0.0;
   out_2065338275713658617[113] = 0.0;
   out_2065338275713658617[114] = 1.0;
   out_2065338275713658617[115] = 0.0;
   out_2065338275713658617[116] = 0.0;
   out_2065338275713658617[117] = 0.0;
   out_2065338275713658617[118] = 0.0;
   out_2065338275713658617[119] = 0.0;
   out_2065338275713658617[120] = 0.0;
   out_2065338275713658617[121] = 0.0;
   out_2065338275713658617[122] = 0.0;
   out_2065338275713658617[123] = 0.0;
   out_2065338275713658617[124] = 0.0;
   out_2065338275713658617[125] = 0.0;
   out_2065338275713658617[126] = 0.0;
   out_2065338275713658617[127] = 0.0;
   out_2065338275713658617[128] = 0.0;
   out_2065338275713658617[129] = 0.0;
   out_2065338275713658617[130] = 0.0;
   out_2065338275713658617[131] = 0.0;
   out_2065338275713658617[132] = 0.0;
   out_2065338275713658617[133] = 1.0;
   out_2065338275713658617[134] = 0.0;
   out_2065338275713658617[135] = 0.0;
   out_2065338275713658617[136] = 0.0;
   out_2065338275713658617[137] = 0.0;
   out_2065338275713658617[138] = 0.0;
   out_2065338275713658617[139] = 0.0;
   out_2065338275713658617[140] = 0.0;
   out_2065338275713658617[141] = 0.0;
   out_2065338275713658617[142] = 0.0;
   out_2065338275713658617[143] = 0.0;
   out_2065338275713658617[144] = 0.0;
   out_2065338275713658617[145] = 0.0;
   out_2065338275713658617[146] = 0.0;
   out_2065338275713658617[147] = 0.0;
   out_2065338275713658617[148] = 0.0;
   out_2065338275713658617[149] = 0.0;
   out_2065338275713658617[150] = 0.0;
   out_2065338275713658617[151] = 0.0;
   out_2065338275713658617[152] = 1.0;
   out_2065338275713658617[153] = 0.0;
   out_2065338275713658617[154] = 0.0;
   out_2065338275713658617[155] = 0.0;
   out_2065338275713658617[156] = 0.0;
   out_2065338275713658617[157] = 0.0;
   out_2065338275713658617[158] = 0.0;
   out_2065338275713658617[159] = 0.0;
   out_2065338275713658617[160] = 0.0;
   out_2065338275713658617[161] = 0.0;
   out_2065338275713658617[162] = 0.0;
   out_2065338275713658617[163] = 0.0;
   out_2065338275713658617[164] = 0.0;
   out_2065338275713658617[165] = 0.0;
   out_2065338275713658617[166] = 0.0;
   out_2065338275713658617[167] = 0.0;
   out_2065338275713658617[168] = 0.0;
   out_2065338275713658617[169] = 0.0;
   out_2065338275713658617[170] = 0.0;
   out_2065338275713658617[171] = 1.0;
   out_2065338275713658617[172] = 0.0;
   out_2065338275713658617[173] = 0.0;
   out_2065338275713658617[174] = 0.0;
   out_2065338275713658617[175] = 0.0;
   out_2065338275713658617[176] = 0.0;
   out_2065338275713658617[177] = 0.0;
   out_2065338275713658617[178] = 0.0;
   out_2065338275713658617[179] = 0.0;
   out_2065338275713658617[180] = 0.0;
   out_2065338275713658617[181] = 0.0;
   out_2065338275713658617[182] = 0.0;
   out_2065338275713658617[183] = 0.0;
   out_2065338275713658617[184] = 0.0;
   out_2065338275713658617[185] = 0.0;
   out_2065338275713658617[186] = 0.0;
   out_2065338275713658617[187] = 0.0;
   out_2065338275713658617[188] = 0.0;
   out_2065338275713658617[189] = 0.0;
   out_2065338275713658617[190] = 1.0;
   out_2065338275713658617[191] = 0.0;
   out_2065338275713658617[192] = 0.0;
   out_2065338275713658617[193] = 0.0;
   out_2065338275713658617[194] = 0.0;
   out_2065338275713658617[195] = 0.0;
   out_2065338275713658617[196] = 0.0;
   out_2065338275713658617[197] = 0.0;
   out_2065338275713658617[198] = 0.0;
   out_2065338275713658617[199] = 0.0;
   out_2065338275713658617[200] = 0.0;
   out_2065338275713658617[201] = 0.0;
   out_2065338275713658617[202] = 0.0;
   out_2065338275713658617[203] = 0.0;
   out_2065338275713658617[204] = 0.0;
   out_2065338275713658617[205] = 0.0;
   out_2065338275713658617[206] = 0.0;
   out_2065338275713658617[207] = 0.0;
   out_2065338275713658617[208] = 0.0;
   out_2065338275713658617[209] = 1.0;
   out_2065338275713658617[210] = 0.0;
   out_2065338275713658617[211] = 0.0;
   out_2065338275713658617[212] = 0.0;
   out_2065338275713658617[213] = 0.0;
   out_2065338275713658617[214] = 0.0;
   out_2065338275713658617[215] = 0.0;
   out_2065338275713658617[216] = 0.0;
   out_2065338275713658617[217] = 0.0;
   out_2065338275713658617[218] = 0.0;
   out_2065338275713658617[219] = 0.0;
   out_2065338275713658617[220] = 0.0;
   out_2065338275713658617[221] = 0.0;
   out_2065338275713658617[222] = 0.0;
   out_2065338275713658617[223] = 0.0;
   out_2065338275713658617[224] = 0.0;
   out_2065338275713658617[225] = 0.0;
   out_2065338275713658617[226] = 0.0;
   out_2065338275713658617[227] = 0.0;
   out_2065338275713658617[228] = 1.0;
   out_2065338275713658617[229] = 0.0;
   out_2065338275713658617[230] = 0.0;
   out_2065338275713658617[231] = 0.0;
   out_2065338275713658617[232] = 0.0;
   out_2065338275713658617[233] = 0.0;
   out_2065338275713658617[234] = 0.0;
   out_2065338275713658617[235] = 0.0;
   out_2065338275713658617[236] = 0.0;
   out_2065338275713658617[237] = 0.0;
   out_2065338275713658617[238] = 0.0;
   out_2065338275713658617[239] = 0.0;
   out_2065338275713658617[240] = 0.0;
   out_2065338275713658617[241] = 0.0;
   out_2065338275713658617[242] = 0.0;
   out_2065338275713658617[243] = 0.0;
   out_2065338275713658617[244] = 0.0;
   out_2065338275713658617[245] = 0.0;
   out_2065338275713658617[246] = 0.0;
   out_2065338275713658617[247] = 1.0;
   out_2065338275713658617[248] = 0.0;
   out_2065338275713658617[249] = 0.0;
   out_2065338275713658617[250] = 0.0;
   out_2065338275713658617[251] = 0.0;
   out_2065338275713658617[252] = 0.0;
   out_2065338275713658617[253] = 0.0;
   out_2065338275713658617[254] = 0.0;
   out_2065338275713658617[255] = 0.0;
   out_2065338275713658617[256] = 0.0;
   out_2065338275713658617[257] = 0.0;
   out_2065338275713658617[258] = 0.0;
   out_2065338275713658617[259] = 0.0;
   out_2065338275713658617[260] = 0.0;
   out_2065338275713658617[261] = 0.0;
   out_2065338275713658617[262] = 0.0;
   out_2065338275713658617[263] = 0.0;
   out_2065338275713658617[264] = 0.0;
   out_2065338275713658617[265] = 0.0;
   out_2065338275713658617[266] = 1.0;
   out_2065338275713658617[267] = 0.0;
   out_2065338275713658617[268] = 0.0;
   out_2065338275713658617[269] = 0.0;
   out_2065338275713658617[270] = 0.0;
   out_2065338275713658617[271] = 0.0;
   out_2065338275713658617[272] = 0.0;
   out_2065338275713658617[273] = 0.0;
   out_2065338275713658617[274] = 0.0;
   out_2065338275713658617[275] = 0.0;
   out_2065338275713658617[276] = 0.0;
   out_2065338275713658617[277] = 0.0;
   out_2065338275713658617[278] = 0.0;
   out_2065338275713658617[279] = 0.0;
   out_2065338275713658617[280] = 0.0;
   out_2065338275713658617[281] = 0.0;
   out_2065338275713658617[282] = 0.0;
   out_2065338275713658617[283] = 0.0;
   out_2065338275713658617[284] = 0.0;
   out_2065338275713658617[285] = 1.0;
   out_2065338275713658617[286] = 0.0;
   out_2065338275713658617[287] = 0.0;
   out_2065338275713658617[288] = 0.0;
   out_2065338275713658617[289] = 0.0;
   out_2065338275713658617[290] = 0.0;
   out_2065338275713658617[291] = 0.0;
   out_2065338275713658617[292] = 0.0;
   out_2065338275713658617[293] = 0.0;
   out_2065338275713658617[294] = 0.0;
   out_2065338275713658617[295] = 0.0;
   out_2065338275713658617[296] = 0.0;
   out_2065338275713658617[297] = 0.0;
   out_2065338275713658617[298] = 0.0;
   out_2065338275713658617[299] = 0.0;
   out_2065338275713658617[300] = 0.0;
   out_2065338275713658617[301] = 0.0;
   out_2065338275713658617[302] = 0.0;
   out_2065338275713658617[303] = 0.0;
   out_2065338275713658617[304] = 1.0;
   out_2065338275713658617[305] = 0.0;
   out_2065338275713658617[306] = 0.0;
   out_2065338275713658617[307] = 0.0;
   out_2065338275713658617[308] = 0.0;
   out_2065338275713658617[309] = 0.0;
   out_2065338275713658617[310] = 0.0;
   out_2065338275713658617[311] = 0.0;
   out_2065338275713658617[312] = 0.0;
   out_2065338275713658617[313] = 0.0;
   out_2065338275713658617[314] = 0.0;
   out_2065338275713658617[315] = 0.0;
   out_2065338275713658617[316] = 0.0;
   out_2065338275713658617[317] = 0.0;
   out_2065338275713658617[318] = 0.0;
   out_2065338275713658617[319] = 0.0;
   out_2065338275713658617[320] = 0.0;
   out_2065338275713658617[321] = 0.0;
   out_2065338275713658617[322] = 0.0;
   out_2065338275713658617[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_47123365585028398) {
   out_47123365585028398[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_47123365585028398[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_47123365585028398[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_47123365585028398[3] = dt*state[12] + state[3];
   out_47123365585028398[4] = dt*state[13] + state[4];
   out_47123365585028398[5] = dt*state[14] + state[5];
   out_47123365585028398[6] = state[6];
   out_47123365585028398[7] = state[7];
   out_47123365585028398[8] = state[8];
   out_47123365585028398[9] = state[9];
   out_47123365585028398[10] = state[10];
   out_47123365585028398[11] = state[11];
   out_47123365585028398[12] = state[12];
   out_47123365585028398[13] = state[13];
   out_47123365585028398[14] = state[14];
   out_47123365585028398[15] = state[15];
   out_47123365585028398[16] = state[16];
   out_47123365585028398[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3459660374911670949) {
   out_3459660374911670949[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3459660374911670949[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3459660374911670949[2] = 0;
   out_3459660374911670949[3] = 0;
   out_3459660374911670949[4] = 0;
   out_3459660374911670949[5] = 0;
   out_3459660374911670949[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3459660374911670949[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3459660374911670949[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3459660374911670949[9] = 0;
   out_3459660374911670949[10] = 0;
   out_3459660374911670949[11] = 0;
   out_3459660374911670949[12] = 0;
   out_3459660374911670949[13] = 0;
   out_3459660374911670949[14] = 0;
   out_3459660374911670949[15] = 0;
   out_3459660374911670949[16] = 0;
   out_3459660374911670949[17] = 0;
   out_3459660374911670949[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3459660374911670949[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3459660374911670949[20] = 0;
   out_3459660374911670949[21] = 0;
   out_3459660374911670949[22] = 0;
   out_3459660374911670949[23] = 0;
   out_3459660374911670949[24] = 0;
   out_3459660374911670949[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3459660374911670949[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3459660374911670949[27] = 0;
   out_3459660374911670949[28] = 0;
   out_3459660374911670949[29] = 0;
   out_3459660374911670949[30] = 0;
   out_3459660374911670949[31] = 0;
   out_3459660374911670949[32] = 0;
   out_3459660374911670949[33] = 0;
   out_3459660374911670949[34] = 0;
   out_3459660374911670949[35] = 0;
   out_3459660374911670949[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3459660374911670949[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3459660374911670949[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3459660374911670949[39] = 0;
   out_3459660374911670949[40] = 0;
   out_3459660374911670949[41] = 0;
   out_3459660374911670949[42] = 0;
   out_3459660374911670949[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3459660374911670949[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3459660374911670949[45] = 0;
   out_3459660374911670949[46] = 0;
   out_3459660374911670949[47] = 0;
   out_3459660374911670949[48] = 0;
   out_3459660374911670949[49] = 0;
   out_3459660374911670949[50] = 0;
   out_3459660374911670949[51] = 0;
   out_3459660374911670949[52] = 0;
   out_3459660374911670949[53] = 0;
   out_3459660374911670949[54] = 0;
   out_3459660374911670949[55] = 0;
   out_3459660374911670949[56] = 0;
   out_3459660374911670949[57] = 1;
   out_3459660374911670949[58] = 0;
   out_3459660374911670949[59] = 0;
   out_3459660374911670949[60] = 0;
   out_3459660374911670949[61] = 0;
   out_3459660374911670949[62] = 0;
   out_3459660374911670949[63] = 0;
   out_3459660374911670949[64] = 0;
   out_3459660374911670949[65] = 0;
   out_3459660374911670949[66] = dt;
   out_3459660374911670949[67] = 0;
   out_3459660374911670949[68] = 0;
   out_3459660374911670949[69] = 0;
   out_3459660374911670949[70] = 0;
   out_3459660374911670949[71] = 0;
   out_3459660374911670949[72] = 0;
   out_3459660374911670949[73] = 0;
   out_3459660374911670949[74] = 0;
   out_3459660374911670949[75] = 0;
   out_3459660374911670949[76] = 1;
   out_3459660374911670949[77] = 0;
   out_3459660374911670949[78] = 0;
   out_3459660374911670949[79] = 0;
   out_3459660374911670949[80] = 0;
   out_3459660374911670949[81] = 0;
   out_3459660374911670949[82] = 0;
   out_3459660374911670949[83] = 0;
   out_3459660374911670949[84] = 0;
   out_3459660374911670949[85] = dt;
   out_3459660374911670949[86] = 0;
   out_3459660374911670949[87] = 0;
   out_3459660374911670949[88] = 0;
   out_3459660374911670949[89] = 0;
   out_3459660374911670949[90] = 0;
   out_3459660374911670949[91] = 0;
   out_3459660374911670949[92] = 0;
   out_3459660374911670949[93] = 0;
   out_3459660374911670949[94] = 0;
   out_3459660374911670949[95] = 1;
   out_3459660374911670949[96] = 0;
   out_3459660374911670949[97] = 0;
   out_3459660374911670949[98] = 0;
   out_3459660374911670949[99] = 0;
   out_3459660374911670949[100] = 0;
   out_3459660374911670949[101] = 0;
   out_3459660374911670949[102] = 0;
   out_3459660374911670949[103] = 0;
   out_3459660374911670949[104] = dt;
   out_3459660374911670949[105] = 0;
   out_3459660374911670949[106] = 0;
   out_3459660374911670949[107] = 0;
   out_3459660374911670949[108] = 0;
   out_3459660374911670949[109] = 0;
   out_3459660374911670949[110] = 0;
   out_3459660374911670949[111] = 0;
   out_3459660374911670949[112] = 0;
   out_3459660374911670949[113] = 0;
   out_3459660374911670949[114] = 1;
   out_3459660374911670949[115] = 0;
   out_3459660374911670949[116] = 0;
   out_3459660374911670949[117] = 0;
   out_3459660374911670949[118] = 0;
   out_3459660374911670949[119] = 0;
   out_3459660374911670949[120] = 0;
   out_3459660374911670949[121] = 0;
   out_3459660374911670949[122] = 0;
   out_3459660374911670949[123] = 0;
   out_3459660374911670949[124] = 0;
   out_3459660374911670949[125] = 0;
   out_3459660374911670949[126] = 0;
   out_3459660374911670949[127] = 0;
   out_3459660374911670949[128] = 0;
   out_3459660374911670949[129] = 0;
   out_3459660374911670949[130] = 0;
   out_3459660374911670949[131] = 0;
   out_3459660374911670949[132] = 0;
   out_3459660374911670949[133] = 1;
   out_3459660374911670949[134] = 0;
   out_3459660374911670949[135] = 0;
   out_3459660374911670949[136] = 0;
   out_3459660374911670949[137] = 0;
   out_3459660374911670949[138] = 0;
   out_3459660374911670949[139] = 0;
   out_3459660374911670949[140] = 0;
   out_3459660374911670949[141] = 0;
   out_3459660374911670949[142] = 0;
   out_3459660374911670949[143] = 0;
   out_3459660374911670949[144] = 0;
   out_3459660374911670949[145] = 0;
   out_3459660374911670949[146] = 0;
   out_3459660374911670949[147] = 0;
   out_3459660374911670949[148] = 0;
   out_3459660374911670949[149] = 0;
   out_3459660374911670949[150] = 0;
   out_3459660374911670949[151] = 0;
   out_3459660374911670949[152] = 1;
   out_3459660374911670949[153] = 0;
   out_3459660374911670949[154] = 0;
   out_3459660374911670949[155] = 0;
   out_3459660374911670949[156] = 0;
   out_3459660374911670949[157] = 0;
   out_3459660374911670949[158] = 0;
   out_3459660374911670949[159] = 0;
   out_3459660374911670949[160] = 0;
   out_3459660374911670949[161] = 0;
   out_3459660374911670949[162] = 0;
   out_3459660374911670949[163] = 0;
   out_3459660374911670949[164] = 0;
   out_3459660374911670949[165] = 0;
   out_3459660374911670949[166] = 0;
   out_3459660374911670949[167] = 0;
   out_3459660374911670949[168] = 0;
   out_3459660374911670949[169] = 0;
   out_3459660374911670949[170] = 0;
   out_3459660374911670949[171] = 1;
   out_3459660374911670949[172] = 0;
   out_3459660374911670949[173] = 0;
   out_3459660374911670949[174] = 0;
   out_3459660374911670949[175] = 0;
   out_3459660374911670949[176] = 0;
   out_3459660374911670949[177] = 0;
   out_3459660374911670949[178] = 0;
   out_3459660374911670949[179] = 0;
   out_3459660374911670949[180] = 0;
   out_3459660374911670949[181] = 0;
   out_3459660374911670949[182] = 0;
   out_3459660374911670949[183] = 0;
   out_3459660374911670949[184] = 0;
   out_3459660374911670949[185] = 0;
   out_3459660374911670949[186] = 0;
   out_3459660374911670949[187] = 0;
   out_3459660374911670949[188] = 0;
   out_3459660374911670949[189] = 0;
   out_3459660374911670949[190] = 1;
   out_3459660374911670949[191] = 0;
   out_3459660374911670949[192] = 0;
   out_3459660374911670949[193] = 0;
   out_3459660374911670949[194] = 0;
   out_3459660374911670949[195] = 0;
   out_3459660374911670949[196] = 0;
   out_3459660374911670949[197] = 0;
   out_3459660374911670949[198] = 0;
   out_3459660374911670949[199] = 0;
   out_3459660374911670949[200] = 0;
   out_3459660374911670949[201] = 0;
   out_3459660374911670949[202] = 0;
   out_3459660374911670949[203] = 0;
   out_3459660374911670949[204] = 0;
   out_3459660374911670949[205] = 0;
   out_3459660374911670949[206] = 0;
   out_3459660374911670949[207] = 0;
   out_3459660374911670949[208] = 0;
   out_3459660374911670949[209] = 1;
   out_3459660374911670949[210] = 0;
   out_3459660374911670949[211] = 0;
   out_3459660374911670949[212] = 0;
   out_3459660374911670949[213] = 0;
   out_3459660374911670949[214] = 0;
   out_3459660374911670949[215] = 0;
   out_3459660374911670949[216] = 0;
   out_3459660374911670949[217] = 0;
   out_3459660374911670949[218] = 0;
   out_3459660374911670949[219] = 0;
   out_3459660374911670949[220] = 0;
   out_3459660374911670949[221] = 0;
   out_3459660374911670949[222] = 0;
   out_3459660374911670949[223] = 0;
   out_3459660374911670949[224] = 0;
   out_3459660374911670949[225] = 0;
   out_3459660374911670949[226] = 0;
   out_3459660374911670949[227] = 0;
   out_3459660374911670949[228] = 1;
   out_3459660374911670949[229] = 0;
   out_3459660374911670949[230] = 0;
   out_3459660374911670949[231] = 0;
   out_3459660374911670949[232] = 0;
   out_3459660374911670949[233] = 0;
   out_3459660374911670949[234] = 0;
   out_3459660374911670949[235] = 0;
   out_3459660374911670949[236] = 0;
   out_3459660374911670949[237] = 0;
   out_3459660374911670949[238] = 0;
   out_3459660374911670949[239] = 0;
   out_3459660374911670949[240] = 0;
   out_3459660374911670949[241] = 0;
   out_3459660374911670949[242] = 0;
   out_3459660374911670949[243] = 0;
   out_3459660374911670949[244] = 0;
   out_3459660374911670949[245] = 0;
   out_3459660374911670949[246] = 0;
   out_3459660374911670949[247] = 1;
   out_3459660374911670949[248] = 0;
   out_3459660374911670949[249] = 0;
   out_3459660374911670949[250] = 0;
   out_3459660374911670949[251] = 0;
   out_3459660374911670949[252] = 0;
   out_3459660374911670949[253] = 0;
   out_3459660374911670949[254] = 0;
   out_3459660374911670949[255] = 0;
   out_3459660374911670949[256] = 0;
   out_3459660374911670949[257] = 0;
   out_3459660374911670949[258] = 0;
   out_3459660374911670949[259] = 0;
   out_3459660374911670949[260] = 0;
   out_3459660374911670949[261] = 0;
   out_3459660374911670949[262] = 0;
   out_3459660374911670949[263] = 0;
   out_3459660374911670949[264] = 0;
   out_3459660374911670949[265] = 0;
   out_3459660374911670949[266] = 1;
   out_3459660374911670949[267] = 0;
   out_3459660374911670949[268] = 0;
   out_3459660374911670949[269] = 0;
   out_3459660374911670949[270] = 0;
   out_3459660374911670949[271] = 0;
   out_3459660374911670949[272] = 0;
   out_3459660374911670949[273] = 0;
   out_3459660374911670949[274] = 0;
   out_3459660374911670949[275] = 0;
   out_3459660374911670949[276] = 0;
   out_3459660374911670949[277] = 0;
   out_3459660374911670949[278] = 0;
   out_3459660374911670949[279] = 0;
   out_3459660374911670949[280] = 0;
   out_3459660374911670949[281] = 0;
   out_3459660374911670949[282] = 0;
   out_3459660374911670949[283] = 0;
   out_3459660374911670949[284] = 0;
   out_3459660374911670949[285] = 1;
   out_3459660374911670949[286] = 0;
   out_3459660374911670949[287] = 0;
   out_3459660374911670949[288] = 0;
   out_3459660374911670949[289] = 0;
   out_3459660374911670949[290] = 0;
   out_3459660374911670949[291] = 0;
   out_3459660374911670949[292] = 0;
   out_3459660374911670949[293] = 0;
   out_3459660374911670949[294] = 0;
   out_3459660374911670949[295] = 0;
   out_3459660374911670949[296] = 0;
   out_3459660374911670949[297] = 0;
   out_3459660374911670949[298] = 0;
   out_3459660374911670949[299] = 0;
   out_3459660374911670949[300] = 0;
   out_3459660374911670949[301] = 0;
   out_3459660374911670949[302] = 0;
   out_3459660374911670949[303] = 0;
   out_3459660374911670949[304] = 1;
   out_3459660374911670949[305] = 0;
   out_3459660374911670949[306] = 0;
   out_3459660374911670949[307] = 0;
   out_3459660374911670949[308] = 0;
   out_3459660374911670949[309] = 0;
   out_3459660374911670949[310] = 0;
   out_3459660374911670949[311] = 0;
   out_3459660374911670949[312] = 0;
   out_3459660374911670949[313] = 0;
   out_3459660374911670949[314] = 0;
   out_3459660374911670949[315] = 0;
   out_3459660374911670949[316] = 0;
   out_3459660374911670949[317] = 0;
   out_3459660374911670949[318] = 0;
   out_3459660374911670949[319] = 0;
   out_3459660374911670949[320] = 0;
   out_3459660374911670949[321] = 0;
   out_3459660374911670949[322] = 0;
   out_3459660374911670949[323] = 1;
}
void h_4(double *state, double *unused, double *out_3884991371756977501) {
   out_3884991371756977501[0] = state[6] + state[9];
   out_3884991371756977501[1] = state[7] + state[10];
   out_3884991371756977501[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7913420082465617338) {
   out_7913420082465617338[0] = 0;
   out_7913420082465617338[1] = 0;
   out_7913420082465617338[2] = 0;
   out_7913420082465617338[3] = 0;
   out_7913420082465617338[4] = 0;
   out_7913420082465617338[5] = 0;
   out_7913420082465617338[6] = 1;
   out_7913420082465617338[7] = 0;
   out_7913420082465617338[8] = 0;
   out_7913420082465617338[9] = 1;
   out_7913420082465617338[10] = 0;
   out_7913420082465617338[11] = 0;
   out_7913420082465617338[12] = 0;
   out_7913420082465617338[13] = 0;
   out_7913420082465617338[14] = 0;
   out_7913420082465617338[15] = 0;
   out_7913420082465617338[16] = 0;
   out_7913420082465617338[17] = 0;
   out_7913420082465617338[18] = 0;
   out_7913420082465617338[19] = 0;
   out_7913420082465617338[20] = 0;
   out_7913420082465617338[21] = 0;
   out_7913420082465617338[22] = 0;
   out_7913420082465617338[23] = 0;
   out_7913420082465617338[24] = 0;
   out_7913420082465617338[25] = 1;
   out_7913420082465617338[26] = 0;
   out_7913420082465617338[27] = 0;
   out_7913420082465617338[28] = 1;
   out_7913420082465617338[29] = 0;
   out_7913420082465617338[30] = 0;
   out_7913420082465617338[31] = 0;
   out_7913420082465617338[32] = 0;
   out_7913420082465617338[33] = 0;
   out_7913420082465617338[34] = 0;
   out_7913420082465617338[35] = 0;
   out_7913420082465617338[36] = 0;
   out_7913420082465617338[37] = 0;
   out_7913420082465617338[38] = 0;
   out_7913420082465617338[39] = 0;
   out_7913420082465617338[40] = 0;
   out_7913420082465617338[41] = 0;
   out_7913420082465617338[42] = 0;
   out_7913420082465617338[43] = 0;
   out_7913420082465617338[44] = 1;
   out_7913420082465617338[45] = 0;
   out_7913420082465617338[46] = 0;
   out_7913420082465617338[47] = 1;
   out_7913420082465617338[48] = 0;
   out_7913420082465617338[49] = 0;
   out_7913420082465617338[50] = 0;
   out_7913420082465617338[51] = 0;
   out_7913420082465617338[52] = 0;
   out_7913420082465617338[53] = 0;
}
void h_10(double *state, double *unused, double *out_6800379684859322945) {
   out_6800379684859322945[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_6800379684859322945[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_6800379684859322945[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8355631585322469337) {
   out_8355631585322469337[0] = 0;
   out_8355631585322469337[1] = 9.8100000000000005*cos(state[1]);
   out_8355631585322469337[2] = 0;
   out_8355631585322469337[3] = 0;
   out_8355631585322469337[4] = -state[8];
   out_8355631585322469337[5] = state[7];
   out_8355631585322469337[6] = 0;
   out_8355631585322469337[7] = state[5];
   out_8355631585322469337[8] = -state[4];
   out_8355631585322469337[9] = 0;
   out_8355631585322469337[10] = 0;
   out_8355631585322469337[11] = 0;
   out_8355631585322469337[12] = 1;
   out_8355631585322469337[13] = 0;
   out_8355631585322469337[14] = 0;
   out_8355631585322469337[15] = 1;
   out_8355631585322469337[16] = 0;
   out_8355631585322469337[17] = 0;
   out_8355631585322469337[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8355631585322469337[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8355631585322469337[20] = 0;
   out_8355631585322469337[21] = state[8];
   out_8355631585322469337[22] = 0;
   out_8355631585322469337[23] = -state[6];
   out_8355631585322469337[24] = -state[5];
   out_8355631585322469337[25] = 0;
   out_8355631585322469337[26] = state[3];
   out_8355631585322469337[27] = 0;
   out_8355631585322469337[28] = 0;
   out_8355631585322469337[29] = 0;
   out_8355631585322469337[30] = 0;
   out_8355631585322469337[31] = 1;
   out_8355631585322469337[32] = 0;
   out_8355631585322469337[33] = 0;
   out_8355631585322469337[34] = 1;
   out_8355631585322469337[35] = 0;
   out_8355631585322469337[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8355631585322469337[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8355631585322469337[38] = 0;
   out_8355631585322469337[39] = -state[7];
   out_8355631585322469337[40] = state[6];
   out_8355631585322469337[41] = 0;
   out_8355631585322469337[42] = state[4];
   out_8355631585322469337[43] = -state[3];
   out_8355631585322469337[44] = 0;
   out_8355631585322469337[45] = 0;
   out_8355631585322469337[46] = 0;
   out_8355631585322469337[47] = 0;
   out_8355631585322469337[48] = 0;
   out_8355631585322469337[49] = 0;
   out_8355631585322469337[50] = 1;
   out_8355631585322469337[51] = 0;
   out_8355631585322469337[52] = 0;
   out_8355631585322469337[53] = 1;
}
void h_13(double *state, double *unused, double *out_19955561701487762) {
   out_19955561701487762[0] = state[3];
   out_19955561701487762[1] = state[4];
   out_19955561701487762[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4701146257133284537) {
   out_4701146257133284537[0] = 0;
   out_4701146257133284537[1] = 0;
   out_4701146257133284537[2] = 0;
   out_4701146257133284537[3] = 1;
   out_4701146257133284537[4] = 0;
   out_4701146257133284537[5] = 0;
   out_4701146257133284537[6] = 0;
   out_4701146257133284537[7] = 0;
   out_4701146257133284537[8] = 0;
   out_4701146257133284537[9] = 0;
   out_4701146257133284537[10] = 0;
   out_4701146257133284537[11] = 0;
   out_4701146257133284537[12] = 0;
   out_4701146257133284537[13] = 0;
   out_4701146257133284537[14] = 0;
   out_4701146257133284537[15] = 0;
   out_4701146257133284537[16] = 0;
   out_4701146257133284537[17] = 0;
   out_4701146257133284537[18] = 0;
   out_4701146257133284537[19] = 0;
   out_4701146257133284537[20] = 0;
   out_4701146257133284537[21] = 0;
   out_4701146257133284537[22] = 1;
   out_4701146257133284537[23] = 0;
   out_4701146257133284537[24] = 0;
   out_4701146257133284537[25] = 0;
   out_4701146257133284537[26] = 0;
   out_4701146257133284537[27] = 0;
   out_4701146257133284537[28] = 0;
   out_4701146257133284537[29] = 0;
   out_4701146257133284537[30] = 0;
   out_4701146257133284537[31] = 0;
   out_4701146257133284537[32] = 0;
   out_4701146257133284537[33] = 0;
   out_4701146257133284537[34] = 0;
   out_4701146257133284537[35] = 0;
   out_4701146257133284537[36] = 0;
   out_4701146257133284537[37] = 0;
   out_4701146257133284537[38] = 0;
   out_4701146257133284537[39] = 0;
   out_4701146257133284537[40] = 0;
   out_4701146257133284537[41] = 1;
   out_4701146257133284537[42] = 0;
   out_4701146257133284537[43] = 0;
   out_4701146257133284537[44] = 0;
   out_4701146257133284537[45] = 0;
   out_4701146257133284537[46] = 0;
   out_4701146257133284537[47] = 0;
   out_4701146257133284537[48] = 0;
   out_4701146257133284537[49] = 0;
   out_4701146257133284537[50] = 0;
   out_4701146257133284537[51] = 0;
   out_4701146257133284537[52] = 0;
   out_4701146257133284537[53] = 0;
}
void h_14(double *state, double *unused, double *out_5485320499311477942) {
   out_5485320499311477942[0] = state[6];
   out_5485320499311477942[1] = state[7];
   out_5485320499311477942[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3950179226126132809) {
   out_3950179226126132809[0] = 0;
   out_3950179226126132809[1] = 0;
   out_3950179226126132809[2] = 0;
   out_3950179226126132809[3] = 0;
   out_3950179226126132809[4] = 0;
   out_3950179226126132809[5] = 0;
   out_3950179226126132809[6] = 1;
   out_3950179226126132809[7] = 0;
   out_3950179226126132809[8] = 0;
   out_3950179226126132809[9] = 0;
   out_3950179226126132809[10] = 0;
   out_3950179226126132809[11] = 0;
   out_3950179226126132809[12] = 0;
   out_3950179226126132809[13] = 0;
   out_3950179226126132809[14] = 0;
   out_3950179226126132809[15] = 0;
   out_3950179226126132809[16] = 0;
   out_3950179226126132809[17] = 0;
   out_3950179226126132809[18] = 0;
   out_3950179226126132809[19] = 0;
   out_3950179226126132809[20] = 0;
   out_3950179226126132809[21] = 0;
   out_3950179226126132809[22] = 0;
   out_3950179226126132809[23] = 0;
   out_3950179226126132809[24] = 0;
   out_3950179226126132809[25] = 1;
   out_3950179226126132809[26] = 0;
   out_3950179226126132809[27] = 0;
   out_3950179226126132809[28] = 0;
   out_3950179226126132809[29] = 0;
   out_3950179226126132809[30] = 0;
   out_3950179226126132809[31] = 0;
   out_3950179226126132809[32] = 0;
   out_3950179226126132809[33] = 0;
   out_3950179226126132809[34] = 0;
   out_3950179226126132809[35] = 0;
   out_3950179226126132809[36] = 0;
   out_3950179226126132809[37] = 0;
   out_3950179226126132809[38] = 0;
   out_3950179226126132809[39] = 0;
   out_3950179226126132809[40] = 0;
   out_3950179226126132809[41] = 0;
   out_3950179226126132809[42] = 0;
   out_3950179226126132809[43] = 0;
   out_3950179226126132809[44] = 1;
   out_3950179226126132809[45] = 0;
   out_3950179226126132809[46] = 0;
   out_3950179226126132809[47] = 0;
   out_3950179226126132809[48] = 0;
   out_3950179226126132809[49] = 0;
   out_3950179226126132809[50] = 0;
   out_3950179226126132809[51] = 0;
   out_3950179226126132809[52] = 0;
   out_3950179226126132809[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_8396481004364689900) {
  err_fun(nom_x, delta_x, out_8396481004364689900);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6933496826796386374) {
  inv_err_fun(nom_x, true_x, out_6933496826796386374);
}
void pose_H_mod_fun(double *state, double *out_2065338275713658617) {
  H_mod_fun(state, out_2065338275713658617);
}
void pose_f_fun(double *state, double dt, double *out_47123365585028398) {
  f_fun(state,  dt, out_47123365585028398);
}
void pose_F_fun(double *state, double dt, double *out_3459660374911670949) {
  F_fun(state,  dt, out_3459660374911670949);
}
void pose_h_4(double *state, double *unused, double *out_3884991371756977501) {
  h_4(state, unused, out_3884991371756977501);
}
void pose_H_4(double *state, double *unused, double *out_7913420082465617338) {
  H_4(state, unused, out_7913420082465617338);
}
void pose_h_10(double *state, double *unused, double *out_6800379684859322945) {
  h_10(state, unused, out_6800379684859322945);
}
void pose_H_10(double *state, double *unused, double *out_8355631585322469337) {
  H_10(state, unused, out_8355631585322469337);
}
void pose_h_13(double *state, double *unused, double *out_19955561701487762) {
  h_13(state, unused, out_19955561701487762);
}
void pose_H_13(double *state, double *unused, double *out_4701146257133284537) {
  H_13(state, unused, out_4701146257133284537);
}
void pose_h_14(double *state, double *unused, double *out_5485320499311477942) {
  h_14(state, unused, out_5485320499311477942);
}
void pose_H_14(double *state, double *unused, double *out_3950179226126132809) {
  H_14(state, unused, out_3950179226126132809);
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
