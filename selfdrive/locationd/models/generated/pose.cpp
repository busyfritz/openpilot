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
void err_fun(double *nom_x, double *delta_x, double *out_8138643469158452515) {
   out_8138643469158452515[0] = delta_x[0] + nom_x[0];
   out_8138643469158452515[1] = delta_x[1] + nom_x[1];
   out_8138643469158452515[2] = delta_x[2] + nom_x[2];
   out_8138643469158452515[3] = delta_x[3] + nom_x[3];
   out_8138643469158452515[4] = delta_x[4] + nom_x[4];
   out_8138643469158452515[5] = delta_x[5] + nom_x[5];
   out_8138643469158452515[6] = delta_x[6] + nom_x[6];
   out_8138643469158452515[7] = delta_x[7] + nom_x[7];
   out_8138643469158452515[8] = delta_x[8] + nom_x[8];
   out_8138643469158452515[9] = delta_x[9] + nom_x[9];
   out_8138643469158452515[10] = delta_x[10] + nom_x[10];
   out_8138643469158452515[11] = delta_x[11] + nom_x[11];
   out_8138643469158452515[12] = delta_x[12] + nom_x[12];
   out_8138643469158452515[13] = delta_x[13] + nom_x[13];
   out_8138643469158452515[14] = delta_x[14] + nom_x[14];
   out_8138643469158452515[15] = delta_x[15] + nom_x[15];
   out_8138643469158452515[16] = delta_x[16] + nom_x[16];
   out_8138643469158452515[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4486085605069433673) {
   out_4486085605069433673[0] = -nom_x[0] + true_x[0];
   out_4486085605069433673[1] = -nom_x[1] + true_x[1];
   out_4486085605069433673[2] = -nom_x[2] + true_x[2];
   out_4486085605069433673[3] = -nom_x[3] + true_x[3];
   out_4486085605069433673[4] = -nom_x[4] + true_x[4];
   out_4486085605069433673[5] = -nom_x[5] + true_x[5];
   out_4486085605069433673[6] = -nom_x[6] + true_x[6];
   out_4486085605069433673[7] = -nom_x[7] + true_x[7];
   out_4486085605069433673[8] = -nom_x[8] + true_x[8];
   out_4486085605069433673[9] = -nom_x[9] + true_x[9];
   out_4486085605069433673[10] = -nom_x[10] + true_x[10];
   out_4486085605069433673[11] = -nom_x[11] + true_x[11];
   out_4486085605069433673[12] = -nom_x[12] + true_x[12];
   out_4486085605069433673[13] = -nom_x[13] + true_x[13];
   out_4486085605069433673[14] = -nom_x[14] + true_x[14];
   out_4486085605069433673[15] = -nom_x[15] + true_x[15];
   out_4486085605069433673[16] = -nom_x[16] + true_x[16];
   out_4486085605069433673[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4128386777611714386) {
   out_4128386777611714386[0] = 1.0;
   out_4128386777611714386[1] = 0.0;
   out_4128386777611714386[2] = 0.0;
   out_4128386777611714386[3] = 0.0;
   out_4128386777611714386[4] = 0.0;
   out_4128386777611714386[5] = 0.0;
   out_4128386777611714386[6] = 0.0;
   out_4128386777611714386[7] = 0.0;
   out_4128386777611714386[8] = 0.0;
   out_4128386777611714386[9] = 0.0;
   out_4128386777611714386[10] = 0.0;
   out_4128386777611714386[11] = 0.0;
   out_4128386777611714386[12] = 0.0;
   out_4128386777611714386[13] = 0.0;
   out_4128386777611714386[14] = 0.0;
   out_4128386777611714386[15] = 0.0;
   out_4128386777611714386[16] = 0.0;
   out_4128386777611714386[17] = 0.0;
   out_4128386777611714386[18] = 0.0;
   out_4128386777611714386[19] = 1.0;
   out_4128386777611714386[20] = 0.0;
   out_4128386777611714386[21] = 0.0;
   out_4128386777611714386[22] = 0.0;
   out_4128386777611714386[23] = 0.0;
   out_4128386777611714386[24] = 0.0;
   out_4128386777611714386[25] = 0.0;
   out_4128386777611714386[26] = 0.0;
   out_4128386777611714386[27] = 0.0;
   out_4128386777611714386[28] = 0.0;
   out_4128386777611714386[29] = 0.0;
   out_4128386777611714386[30] = 0.0;
   out_4128386777611714386[31] = 0.0;
   out_4128386777611714386[32] = 0.0;
   out_4128386777611714386[33] = 0.0;
   out_4128386777611714386[34] = 0.0;
   out_4128386777611714386[35] = 0.0;
   out_4128386777611714386[36] = 0.0;
   out_4128386777611714386[37] = 0.0;
   out_4128386777611714386[38] = 1.0;
   out_4128386777611714386[39] = 0.0;
   out_4128386777611714386[40] = 0.0;
   out_4128386777611714386[41] = 0.0;
   out_4128386777611714386[42] = 0.0;
   out_4128386777611714386[43] = 0.0;
   out_4128386777611714386[44] = 0.0;
   out_4128386777611714386[45] = 0.0;
   out_4128386777611714386[46] = 0.0;
   out_4128386777611714386[47] = 0.0;
   out_4128386777611714386[48] = 0.0;
   out_4128386777611714386[49] = 0.0;
   out_4128386777611714386[50] = 0.0;
   out_4128386777611714386[51] = 0.0;
   out_4128386777611714386[52] = 0.0;
   out_4128386777611714386[53] = 0.0;
   out_4128386777611714386[54] = 0.0;
   out_4128386777611714386[55] = 0.0;
   out_4128386777611714386[56] = 0.0;
   out_4128386777611714386[57] = 1.0;
   out_4128386777611714386[58] = 0.0;
   out_4128386777611714386[59] = 0.0;
   out_4128386777611714386[60] = 0.0;
   out_4128386777611714386[61] = 0.0;
   out_4128386777611714386[62] = 0.0;
   out_4128386777611714386[63] = 0.0;
   out_4128386777611714386[64] = 0.0;
   out_4128386777611714386[65] = 0.0;
   out_4128386777611714386[66] = 0.0;
   out_4128386777611714386[67] = 0.0;
   out_4128386777611714386[68] = 0.0;
   out_4128386777611714386[69] = 0.0;
   out_4128386777611714386[70] = 0.0;
   out_4128386777611714386[71] = 0.0;
   out_4128386777611714386[72] = 0.0;
   out_4128386777611714386[73] = 0.0;
   out_4128386777611714386[74] = 0.0;
   out_4128386777611714386[75] = 0.0;
   out_4128386777611714386[76] = 1.0;
   out_4128386777611714386[77] = 0.0;
   out_4128386777611714386[78] = 0.0;
   out_4128386777611714386[79] = 0.0;
   out_4128386777611714386[80] = 0.0;
   out_4128386777611714386[81] = 0.0;
   out_4128386777611714386[82] = 0.0;
   out_4128386777611714386[83] = 0.0;
   out_4128386777611714386[84] = 0.0;
   out_4128386777611714386[85] = 0.0;
   out_4128386777611714386[86] = 0.0;
   out_4128386777611714386[87] = 0.0;
   out_4128386777611714386[88] = 0.0;
   out_4128386777611714386[89] = 0.0;
   out_4128386777611714386[90] = 0.0;
   out_4128386777611714386[91] = 0.0;
   out_4128386777611714386[92] = 0.0;
   out_4128386777611714386[93] = 0.0;
   out_4128386777611714386[94] = 0.0;
   out_4128386777611714386[95] = 1.0;
   out_4128386777611714386[96] = 0.0;
   out_4128386777611714386[97] = 0.0;
   out_4128386777611714386[98] = 0.0;
   out_4128386777611714386[99] = 0.0;
   out_4128386777611714386[100] = 0.0;
   out_4128386777611714386[101] = 0.0;
   out_4128386777611714386[102] = 0.0;
   out_4128386777611714386[103] = 0.0;
   out_4128386777611714386[104] = 0.0;
   out_4128386777611714386[105] = 0.0;
   out_4128386777611714386[106] = 0.0;
   out_4128386777611714386[107] = 0.0;
   out_4128386777611714386[108] = 0.0;
   out_4128386777611714386[109] = 0.0;
   out_4128386777611714386[110] = 0.0;
   out_4128386777611714386[111] = 0.0;
   out_4128386777611714386[112] = 0.0;
   out_4128386777611714386[113] = 0.0;
   out_4128386777611714386[114] = 1.0;
   out_4128386777611714386[115] = 0.0;
   out_4128386777611714386[116] = 0.0;
   out_4128386777611714386[117] = 0.0;
   out_4128386777611714386[118] = 0.0;
   out_4128386777611714386[119] = 0.0;
   out_4128386777611714386[120] = 0.0;
   out_4128386777611714386[121] = 0.0;
   out_4128386777611714386[122] = 0.0;
   out_4128386777611714386[123] = 0.0;
   out_4128386777611714386[124] = 0.0;
   out_4128386777611714386[125] = 0.0;
   out_4128386777611714386[126] = 0.0;
   out_4128386777611714386[127] = 0.0;
   out_4128386777611714386[128] = 0.0;
   out_4128386777611714386[129] = 0.0;
   out_4128386777611714386[130] = 0.0;
   out_4128386777611714386[131] = 0.0;
   out_4128386777611714386[132] = 0.0;
   out_4128386777611714386[133] = 1.0;
   out_4128386777611714386[134] = 0.0;
   out_4128386777611714386[135] = 0.0;
   out_4128386777611714386[136] = 0.0;
   out_4128386777611714386[137] = 0.0;
   out_4128386777611714386[138] = 0.0;
   out_4128386777611714386[139] = 0.0;
   out_4128386777611714386[140] = 0.0;
   out_4128386777611714386[141] = 0.0;
   out_4128386777611714386[142] = 0.0;
   out_4128386777611714386[143] = 0.0;
   out_4128386777611714386[144] = 0.0;
   out_4128386777611714386[145] = 0.0;
   out_4128386777611714386[146] = 0.0;
   out_4128386777611714386[147] = 0.0;
   out_4128386777611714386[148] = 0.0;
   out_4128386777611714386[149] = 0.0;
   out_4128386777611714386[150] = 0.0;
   out_4128386777611714386[151] = 0.0;
   out_4128386777611714386[152] = 1.0;
   out_4128386777611714386[153] = 0.0;
   out_4128386777611714386[154] = 0.0;
   out_4128386777611714386[155] = 0.0;
   out_4128386777611714386[156] = 0.0;
   out_4128386777611714386[157] = 0.0;
   out_4128386777611714386[158] = 0.0;
   out_4128386777611714386[159] = 0.0;
   out_4128386777611714386[160] = 0.0;
   out_4128386777611714386[161] = 0.0;
   out_4128386777611714386[162] = 0.0;
   out_4128386777611714386[163] = 0.0;
   out_4128386777611714386[164] = 0.0;
   out_4128386777611714386[165] = 0.0;
   out_4128386777611714386[166] = 0.0;
   out_4128386777611714386[167] = 0.0;
   out_4128386777611714386[168] = 0.0;
   out_4128386777611714386[169] = 0.0;
   out_4128386777611714386[170] = 0.0;
   out_4128386777611714386[171] = 1.0;
   out_4128386777611714386[172] = 0.0;
   out_4128386777611714386[173] = 0.0;
   out_4128386777611714386[174] = 0.0;
   out_4128386777611714386[175] = 0.0;
   out_4128386777611714386[176] = 0.0;
   out_4128386777611714386[177] = 0.0;
   out_4128386777611714386[178] = 0.0;
   out_4128386777611714386[179] = 0.0;
   out_4128386777611714386[180] = 0.0;
   out_4128386777611714386[181] = 0.0;
   out_4128386777611714386[182] = 0.0;
   out_4128386777611714386[183] = 0.0;
   out_4128386777611714386[184] = 0.0;
   out_4128386777611714386[185] = 0.0;
   out_4128386777611714386[186] = 0.0;
   out_4128386777611714386[187] = 0.0;
   out_4128386777611714386[188] = 0.0;
   out_4128386777611714386[189] = 0.0;
   out_4128386777611714386[190] = 1.0;
   out_4128386777611714386[191] = 0.0;
   out_4128386777611714386[192] = 0.0;
   out_4128386777611714386[193] = 0.0;
   out_4128386777611714386[194] = 0.0;
   out_4128386777611714386[195] = 0.0;
   out_4128386777611714386[196] = 0.0;
   out_4128386777611714386[197] = 0.0;
   out_4128386777611714386[198] = 0.0;
   out_4128386777611714386[199] = 0.0;
   out_4128386777611714386[200] = 0.0;
   out_4128386777611714386[201] = 0.0;
   out_4128386777611714386[202] = 0.0;
   out_4128386777611714386[203] = 0.0;
   out_4128386777611714386[204] = 0.0;
   out_4128386777611714386[205] = 0.0;
   out_4128386777611714386[206] = 0.0;
   out_4128386777611714386[207] = 0.0;
   out_4128386777611714386[208] = 0.0;
   out_4128386777611714386[209] = 1.0;
   out_4128386777611714386[210] = 0.0;
   out_4128386777611714386[211] = 0.0;
   out_4128386777611714386[212] = 0.0;
   out_4128386777611714386[213] = 0.0;
   out_4128386777611714386[214] = 0.0;
   out_4128386777611714386[215] = 0.0;
   out_4128386777611714386[216] = 0.0;
   out_4128386777611714386[217] = 0.0;
   out_4128386777611714386[218] = 0.0;
   out_4128386777611714386[219] = 0.0;
   out_4128386777611714386[220] = 0.0;
   out_4128386777611714386[221] = 0.0;
   out_4128386777611714386[222] = 0.0;
   out_4128386777611714386[223] = 0.0;
   out_4128386777611714386[224] = 0.0;
   out_4128386777611714386[225] = 0.0;
   out_4128386777611714386[226] = 0.0;
   out_4128386777611714386[227] = 0.0;
   out_4128386777611714386[228] = 1.0;
   out_4128386777611714386[229] = 0.0;
   out_4128386777611714386[230] = 0.0;
   out_4128386777611714386[231] = 0.0;
   out_4128386777611714386[232] = 0.0;
   out_4128386777611714386[233] = 0.0;
   out_4128386777611714386[234] = 0.0;
   out_4128386777611714386[235] = 0.0;
   out_4128386777611714386[236] = 0.0;
   out_4128386777611714386[237] = 0.0;
   out_4128386777611714386[238] = 0.0;
   out_4128386777611714386[239] = 0.0;
   out_4128386777611714386[240] = 0.0;
   out_4128386777611714386[241] = 0.0;
   out_4128386777611714386[242] = 0.0;
   out_4128386777611714386[243] = 0.0;
   out_4128386777611714386[244] = 0.0;
   out_4128386777611714386[245] = 0.0;
   out_4128386777611714386[246] = 0.0;
   out_4128386777611714386[247] = 1.0;
   out_4128386777611714386[248] = 0.0;
   out_4128386777611714386[249] = 0.0;
   out_4128386777611714386[250] = 0.0;
   out_4128386777611714386[251] = 0.0;
   out_4128386777611714386[252] = 0.0;
   out_4128386777611714386[253] = 0.0;
   out_4128386777611714386[254] = 0.0;
   out_4128386777611714386[255] = 0.0;
   out_4128386777611714386[256] = 0.0;
   out_4128386777611714386[257] = 0.0;
   out_4128386777611714386[258] = 0.0;
   out_4128386777611714386[259] = 0.0;
   out_4128386777611714386[260] = 0.0;
   out_4128386777611714386[261] = 0.0;
   out_4128386777611714386[262] = 0.0;
   out_4128386777611714386[263] = 0.0;
   out_4128386777611714386[264] = 0.0;
   out_4128386777611714386[265] = 0.0;
   out_4128386777611714386[266] = 1.0;
   out_4128386777611714386[267] = 0.0;
   out_4128386777611714386[268] = 0.0;
   out_4128386777611714386[269] = 0.0;
   out_4128386777611714386[270] = 0.0;
   out_4128386777611714386[271] = 0.0;
   out_4128386777611714386[272] = 0.0;
   out_4128386777611714386[273] = 0.0;
   out_4128386777611714386[274] = 0.0;
   out_4128386777611714386[275] = 0.0;
   out_4128386777611714386[276] = 0.0;
   out_4128386777611714386[277] = 0.0;
   out_4128386777611714386[278] = 0.0;
   out_4128386777611714386[279] = 0.0;
   out_4128386777611714386[280] = 0.0;
   out_4128386777611714386[281] = 0.0;
   out_4128386777611714386[282] = 0.0;
   out_4128386777611714386[283] = 0.0;
   out_4128386777611714386[284] = 0.0;
   out_4128386777611714386[285] = 1.0;
   out_4128386777611714386[286] = 0.0;
   out_4128386777611714386[287] = 0.0;
   out_4128386777611714386[288] = 0.0;
   out_4128386777611714386[289] = 0.0;
   out_4128386777611714386[290] = 0.0;
   out_4128386777611714386[291] = 0.0;
   out_4128386777611714386[292] = 0.0;
   out_4128386777611714386[293] = 0.0;
   out_4128386777611714386[294] = 0.0;
   out_4128386777611714386[295] = 0.0;
   out_4128386777611714386[296] = 0.0;
   out_4128386777611714386[297] = 0.0;
   out_4128386777611714386[298] = 0.0;
   out_4128386777611714386[299] = 0.0;
   out_4128386777611714386[300] = 0.0;
   out_4128386777611714386[301] = 0.0;
   out_4128386777611714386[302] = 0.0;
   out_4128386777611714386[303] = 0.0;
   out_4128386777611714386[304] = 1.0;
   out_4128386777611714386[305] = 0.0;
   out_4128386777611714386[306] = 0.0;
   out_4128386777611714386[307] = 0.0;
   out_4128386777611714386[308] = 0.0;
   out_4128386777611714386[309] = 0.0;
   out_4128386777611714386[310] = 0.0;
   out_4128386777611714386[311] = 0.0;
   out_4128386777611714386[312] = 0.0;
   out_4128386777611714386[313] = 0.0;
   out_4128386777611714386[314] = 0.0;
   out_4128386777611714386[315] = 0.0;
   out_4128386777611714386[316] = 0.0;
   out_4128386777611714386[317] = 0.0;
   out_4128386777611714386[318] = 0.0;
   out_4128386777611714386[319] = 0.0;
   out_4128386777611714386[320] = 0.0;
   out_4128386777611714386[321] = 0.0;
   out_4128386777611714386[322] = 0.0;
   out_4128386777611714386[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_3926695733139875668) {
   out_3926695733139875668[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_3926695733139875668[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_3926695733139875668[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_3926695733139875668[3] = dt*state[12] + state[3];
   out_3926695733139875668[4] = dt*state[13] + state[4];
   out_3926695733139875668[5] = dt*state[14] + state[5];
   out_3926695733139875668[6] = state[6];
   out_3926695733139875668[7] = state[7];
   out_3926695733139875668[8] = state[8];
   out_3926695733139875668[9] = state[9];
   out_3926695733139875668[10] = state[10];
   out_3926695733139875668[11] = state[11];
   out_3926695733139875668[12] = state[12];
   out_3926695733139875668[13] = state[13];
   out_3926695733139875668[14] = state[14];
   out_3926695733139875668[15] = state[15];
   out_3926695733139875668[16] = state[16];
   out_3926695733139875668[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3790507139778411869) {
   out_3790507139778411869[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3790507139778411869[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3790507139778411869[2] = 0;
   out_3790507139778411869[3] = 0;
   out_3790507139778411869[4] = 0;
   out_3790507139778411869[5] = 0;
   out_3790507139778411869[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3790507139778411869[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3790507139778411869[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3790507139778411869[9] = 0;
   out_3790507139778411869[10] = 0;
   out_3790507139778411869[11] = 0;
   out_3790507139778411869[12] = 0;
   out_3790507139778411869[13] = 0;
   out_3790507139778411869[14] = 0;
   out_3790507139778411869[15] = 0;
   out_3790507139778411869[16] = 0;
   out_3790507139778411869[17] = 0;
   out_3790507139778411869[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3790507139778411869[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3790507139778411869[20] = 0;
   out_3790507139778411869[21] = 0;
   out_3790507139778411869[22] = 0;
   out_3790507139778411869[23] = 0;
   out_3790507139778411869[24] = 0;
   out_3790507139778411869[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3790507139778411869[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3790507139778411869[27] = 0;
   out_3790507139778411869[28] = 0;
   out_3790507139778411869[29] = 0;
   out_3790507139778411869[30] = 0;
   out_3790507139778411869[31] = 0;
   out_3790507139778411869[32] = 0;
   out_3790507139778411869[33] = 0;
   out_3790507139778411869[34] = 0;
   out_3790507139778411869[35] = 0;
   out_3790507139778411869[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3790507139778411869[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3790507139778411869[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3790507139778411869[39] = 0;
   out_3790507139778411869[40] = 0;
   out_3790507139778411869[41] = 0;
   out_3790507139778411869[42] = 0;
   out_3790507139778411869[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3790507139778411869[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3790507139778411869[45] = 0;
   out_3790507139778411869[46] = 0;
   out_3790507139778411869[47] = 0;
   out_3790507139778411869[48] = 0;
   out_3790507139778411869[49] = 0;
   out_3790507139778411869[50] = 0;
   out_3790507139778411869[51] = 0;
   out_3790507139778411869[52] = 0;
   out_3790507139778411869[53] = 0;
   out_3790507139778411869[54] = 0;
   out_3790507139778411869[55] = 0;
   out_3790507139778411869[56] = 0;
   out_3790507139778411869[57] = 1;
   out_3790507139778411869[58] = 0;
   out_3790507139778411869[59] = 0;
   out_3790507139778411869[60] = 0;
   out_3790507139778411869[61] = 0;
   out_3790507139778411869[62] = 0;
   out_3790507139778411869[63] = 0;
   out_3790507139778411869[64] = 0;
   out_3790507139778411869[65] = 0;
   out_3790507139778411869[66] = dt;
   out_3790507139778411869[67] = 0;
   out_3790507139778411869[68] = 0;
   out_3790507139778411869[69] = 0;
   out_3790507139778411869[70] = 0;
   out_3790507139778411869[71] = 0;
   out_3790507139778411869[72] = 0;
   out_3790507139778411869[73] = 0;
   out_3790507139778411869[74] = 0;
   out_3790507139778411869[75] = 0;
   out_3790507139778411869[76] = 1;
   out_3790507139778411869[77] = 0;
   out_3790507139778411869[78] = 0;
   out_3790507139778411869[79] = 0;
   out_3790507139778411869[80] = 0;
   out_3790507139778411869[81] = 0;
   out_3790507139778411869[82] = 0;
   out_3790507139778411869[83] = 0;
   out_3790507139778411869[84] = 0;
   out_3790507139778411869[85] = dt;
   out_3790507139778411869[86] = 0;
   out_3790507139778411869[87] = 0;
   out_3790507139778411869[88] = 0;
   out_3790507139778411869[89] = 0;
   out_3790507139778411869[90] = 0;
   out_3790507139778411869[91] = 0;
   out_3790507139778411869[92] = 0;
   out_3790507139778411869[93] = 0;
   out_3790507139778411869[94] = 0;
   out_3790507139778411869[95] = 1;
   out_3790507139778411869[96] = 0;
   out_3790507139778411869[97] = 0;
   out_3790507139778411869[98] = 0;
   out_3790507139778411869[99] = 0;
   out_3790507139778411869[100] = 0;
   out_3790507139778411869[101] = 0;
   out_3790507139778411869[102] = 0;
   out_3790507139778411869[103] = 0;
   out_3790507139778411869[104] = dt;
   out_3790507139778411869[105] = 0;
   out_3790507139778411869[106] = 0;
   out_3790507139778411869[107] = 0;
   out_3790507139778411869[108] = 0;
   out_3790507139778411869[109] = 0;
   out_3790507139778411869[110] = 0;
   out_3790507139778411869[111] = 0;
   out_3790507139778411869[112] = 0;
   out_3790507139778411869[113] = 0;
   out_3790507139778411869[114] = 1;
   out_3790507139778411869[115] = 0;
   out_3790507139778411869[116] = 0;
   out_3790507139778411869[117] = 0;
   out_3790507139778411869[118] = 0;
   out_3790507139778411869[119] = 0;
   out_3790507139778411869[120] = 0;
   out_3790507139778411869[121] = 0;
   out_3790507139778411869[122] = 0;
   out_3790507139778411869[123] = 0;
   out_3790507139778411869[124] = 0;
   out_3790507139778411869[125] = 0;
   out_3790507139778411869[126] = 0;
   out_3790507139778411869[127] = 0;
   out_3790507139778411869[128] = 0;
   out_3790507139778411869[129] = 0;
   out_3790507139778411869[130] = 0;
   out_3790507139778411869[131] = 0;
   out_3790507139778411869[132] = 0;
   out_3790507139778411869[133] = 1;
   out_3790507139778411869[134] = 0;
   out_3790507139778411869[135] = 0;
   out_3790507139778411869[136] = 0;
   out_3790507139778411869[137] = 0;
   out_3790507139778411869[138] = 0;
   out_3790507139778411869[139] = 0;
   out_3790507139778411869[140] = 0;
   out_3790507139778411869[141] = 0;
   out_3790507139778411869[142] = 0;
   out_3790507139778411869[143] = 0;
   out_3790507139778411869[144] = 0;
   out_3790507139778411869[145] = 0;
   out_3790507139778411869[146] = 0;
   out_3790507139778411869[147] = 0;
   out_3790507139778411869[148] = 0;
   out_3790507139778411869[149] = 0;
   out_3790507139778411869[150] = 0;
   out_3790507139778411869[151] = 0;
   out_3790507139778411869[152] = 1;
   out_3790507139778411869[153] = 0;
   out_3790507139778411869[154] = 0;
   out_3790507139778411869[155] = 0;
   out_3790507139778411869[156] = 0;
   out_3790507139778411869[157] = 0;
   out_3790507139778411869[158] = 0;
   out_3790507139778411869[159] = 0;
   out_3790507139778411869[160] = 0;
   out_3790507139778411869[161] = 0;
   out_3790507139778411869[162] = 0;
   out_3790507139778411869[163] = 0;
   out_3790507139778411869[164] = 0;
   out_3790507139778411869[165] = 0;
   out_3790507139778411869[166] = 0;
   out_3790507139778411869[167] = 0;
   out_3790507139778411869[168] = 0;
   out_3790507139778411869[169] = 0;
   out_3790507139778411869[170] = 0;
   out_3790507139778411869[171] = 1;
   out_3790507139778411869[172] = 0;
   out_3790507139778411869[173] = 0;
   out_3790507139778411869[174] = 0;
   out_3790507139778411869[175] = 0;
   out_3790507139778411869[176] = 0;
   out_3790507139778411869[177] = 0;
   out_3790507139778411869[178] = 0;
   out_3790507139778411869[179] = 0;
   out_3790507139778411869[180] = 0;
   out_3790507139778411869[181] = 0;
   out_3790507139778411869[182] = 0;
   out_3790507139778411869[183] = 0;
   out_3790507139778411869[184] = 0;
   out_3790507139778411869[185] = 0;
   out_3790507139778411869[186] = 0;
   out_3790507139778411869[187] = 0;
   out_3790507139778411869[188] = 0;
   out_3790507139778411869[189] = 0;
   out_3790507139778411869[190] = 1;
   out_3790507139778411869[191] = 0;
   out_3790507139778411869[192] = 0;
   out_3790507139778411869[193] = 0;
   out_3790507139778411869[194] = 0;
   out_3790507139778411869[195] = 0;
   out_3790507139778411869[196] = 0;
   out_3790507139778411869[197] = 0;
   out_3790507139778411869[198] = 0;
   out_3790507139778411869[199] = 0;
   out_3790507139778411869[200] = 0;
   out_3790507139778411869[201] = 0;
   out_3790507139778411869[202] = 0;
   out_3790507139778411869[203] = 0;
   out_3790507139778411869[204] = 0;
   out_3790507139778411869[205] = 0;
   out_3790507139778411869[206] = 0;
   out_3790507139778411869[207] = 0;
   out_3790507139778411869[208] = 0;
   out_3790507139778411869[209] = 1;
   out_3790507139778411869[210] = 0;
   out_3790507139778411869[211] = 0;
   out_3790507139778411869[212] = 0;
   out_3790507139778411869[213] = 0;
   out_3790507139778411869[214] = 0;
   out_3790507139778411869[215] = 0;
   out_3790507139778411869[216] = 0;
   out_3790507139778411869[217] = 0;
   out_3790507139778411869[218] = 0;
   out_3790507139778411869[219] = 0;
   out_3790507139778411869[220] = 0;
   out_3790507139778411869[221] = 0;
   out_3790507139778411869[222] = 0;
   out_3790507139778411869[223] = 0;
   out_3790507139778411869[224] = 0;
   out_3790507139778411869[225] = 0;
   out_3790507139778411869[226] = 0;
   out_3790507139778411869[227] = 0;
   out_3790507139778411869[228] = 1;
   out_3790507139778411869[229] = 0;
   out_3790507139778411869[230] = 0;
   out_3790507139778411869[231] = 0;
   out_3790507139778411869[232] = 0;
   out_3790507139778411869[233] = 0;
   out_3790507139778411869[234] = 0;
   out_3790507139778411869[235] = 0;
   out_3790507139778411869[236] = 0;
   out_3790507139778411869[237] = 0;
   out_3790507139778411869[238] = 0;
   out_3790507139778411869[239] = 0;
   out_3790507139778411869[240] = 0;
   out_3790507139778411869[241] = 0;
   out_3790507139778411869[242] = 0;
   out_3790507139778411869[243] = 0;
   out_3790507139778411869[244] = 0;
   out_3790507139778411869[245] = 0;
   out_3790507139778411869[246] = 0;
   out_3790507139778411869[247] = 1;
   out_3790507139778411869[248] = 0;
   out_3790507139778411869[249] = 0;
   out_3790507139778411869[250] = 0;
   out_3790507139778411869[251] = 0;
   out_3790507139778411869[252] = 0;
   out_3790507139778411869[253] = 0;
   out_3790507139778411869[254] = 0;
   out_3790507139778411869[255] = 0;
   out_3790507139778411869[256] = 0;
   out_3790507139778411869[257] = 0;
   out_3790507139778411869[258] = 0;
   out_3790507139778411869[259] = 0;
   out_3790507139778411869[260] = 0;
   out_3790507139778411869[261] = 0;
   out_3790507139778411869[262] = 0;
   out_3790507139778411869[263] = 0;
   out_3790507139778411869[264] = 0;
   out_3790507139778411869[265] = 0;
   out_3790507139778411869[266] = 1;
   out_3790507139778411869[267] = 0;
   out_3790507139778411869[268] = 0;
   out_3790507139778411869[269] = 0;
   out_3790507139778411869[270] = 0;
   out_3790507139778411869[271] = 0;
   out_3790507139778411869[272] = 0;
   out_3790507139778411869[273] = 0;
   out_3790507139778411869[274] = 0;
   out_3790507139778411869[275] = 0;
   out_3790507139778411869[276] = 0;
   out_3790507139778411869[277] = 0;
   out_3790507139778411869[278] = 0;
   out_3790507139778411869[279] = 0;
   out_3790507139778411869[280] = 0;
   out_3790507139778411869[281] = 0;
   out_3790507139778411869[282] = 0;
   out_3790507139778411869[283] = 0;
   out_3790507139778411869[284] = 0;
   out_3790507139778411869[285] = 1;
   out_3790507139778411869[286] = 0;
   out_3790507139778411869[287] = 0;
   out_3790507139778411869[288] = 0;
   out_3790507139778411869[289] = 0;
   out_3790507139778411869[290] = 0;
   out_3790507139778411869[291] = 0;
   out_3790507139778411869[292] = 0;
   out_3790507139778411869[293] = 0;
   out_3790507139778411869[294] = 0;
   out_3790507139778411869[295] = 0;
   out_3790507139778411869[296] = 0;
   out_3790507139778411869[297] = 0;
   out_3790507139778411869[298] = 0;
   out_3790507139778411869[299] = 0;
   out_3790507139778411869[300] = 0;
   out_3790507139778411869[301] = 0;
   out_3790507139778411869[302] = 0;
   out_3790507139778411869[303] = 0;
   out_3790507139778411869[304] = 1;
   out_3790507139778411869[305] = 0;
   out_3790507139778411869[306] = 0;
   out_3790507139778411869[307] = 0;
   out_3790507139778411869[308] = 0;
   out_3790507139778411869[309] = 0;
   out_3790507139778411869[310] = 0;
   out_3790507139778411869[311] = 0;
   out_3790507139778411869[312] = 0;
   out_3790507139778411869[313] = 0;
   out_3790507139778411869[314] = 0;
   out_3790507139778411869[315] = 0;
   out_3790507139778411869[316] = 0;
   out_3790507139778411869[317] = 0;
   out_3790507139778411869[318] = 0;
   out_3790507139778411869[319] = 0;
   out_3790507139778411869[320] = 0;
   out_3790507139778411869[321] = 0;
   out_3790507139778411869[322] = 0;
   out_3790507139778411869[323] = 1;
}
void h_4(double *state, double *unused, double *out_7316226510605005089) {
   out_7316226510605005089[0] = state[6] + state[9];
   out_7316226510605005089[1] = state[7] + state[10];
   out_7316226510605005089[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_3374225775557001779) {
   out_3374225775557001779[0] = 0;
   out_3374225775557001779[1] = 0;
   out_3374225775557001779[2] = 0;
   out_3374225775557001779[3] = 0;
   out_3374225775557001779[4] = 0;
   out_3374225775557001779[5] = 0;
   out_3374225775557001779[6] = 1;
   out_3374225775557001779[7] = 0;
   out_3374225775557001779[8] = 0;
   out_3374225775557001779[9] = 1;
   out_3374225775557001779[10] = 0;
   out_3374225775557001779[11] = 0;
   out_3374225775557001779[12] = 0;
   out_3374225775557001779[13] = 0;
   out_3374225775557001779[14] = 0;
   out_3374225775557001779[15] = 0;
   out_3374225775557001779[16] = 0;
   out_3374225775557001779[17] = 0;
   out_3374225775557001779[18] = 0;
   out_3374225775557001779[19] = 0;
   out_3374225775557001779[20] = 0;
   out_3374225775557001779[21] = 0;
   out_3374225775557001779[22] = 0;
   out_3374225775557001779[23] = 0;
   out_3374225775557001779[24] = 0;
   out_3374225775557001779[25] = 1;
   out_3374225775557001779[26] = 0;
   out_3374225775557001779[27] = 0;
   out_3374225775557001779[28] = 1;
   out_3374225775557001779[29] = 0;
   out_3374225775557001779[30] = 0;
   out_3374225775557001779[31] = 0;
   out_3374225775557001779[32] = 0;
   out_3374225775557001779[33] = 0;
   out_3374225775557001779[34] = 0;
   out_3374225775557001779[35] = 0;
   out_3374225775557001779[36] = 0;
   out_3374225775557001779[37] = 0;
   out_3374225775557001779[38] = 0;
   out_3374225775557001779[39] = 0;
   out_3374225775557001779[40] = 0;
   out_3374225775557001779[41] = 0;
   out_3374225775557001779[42] = 0;
   out_3374225775557001779[43] = 0;
   out_3374225775557001779[44] = 1;
   out_3374225775557001779[45] = 0;
   out_3374225775557001779[46] = 0;
   out_3374225775557001779[47] = 1;
   out_3374225775557001779[48] = 0;
   out_3374225775557001779[49] = 0;
   out_3374225775557001779[50] = 0;
   out_3374225775557001779[51] = 0;
   out_3374225775557001779[52] = 0;
   out_3374225775557001779[53] = 0;
}
void h_10(double *state, double *unused, double *out_2540837594140349822) {
   out_2540837594140349822[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2540837594140349822[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2540837594140349822[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_6682096001196788588) {
   out_6682096001196788588[0] = 0;
   out_6682096001196788588[1] = 9.8100000000000005*cos(state[1]);
   out_6682096001196788588[2] = 0;
   out_6682096001196788588[3] = 0;
   out_6682096001196788588[4] = -state[8];
   out_6682096001196788588[5] = state[7];
   out_6682096001196788588[6] = 0;
   out_6682096001196788588[7] = state[5];
   out_6682096001196788588[8] = -state[4];
   out_6682096001196788588[9] = 0;
   out_6682096001196788588[10] = 0;
   out_6682096001196788588[11] = 0;
   out_6682096001196788588[12] = 1;
   out_6682096001196788588[13] = 0;
   out_6682096001196788588[14] = 0;
   out_6682096001196788588[15] = 1;
   out_6682096001196788588[16] = 0;
   out_6682096001196788588[17] = 0;
   out_6682096001196788588[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_6682096001196788588[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_6682096001196788588[20] = 0;
   out_6682096001196788588[21] = state[8];
   out_6682096001196788588[22] = 0;
   out_6682096001196788588[23] = -state[6];
   out_6682096001196788588[24] = -state[5];
   out_6682096001196788588[25] = 0;
   out_6682096001196788588[26] = state[3];
   out_6682096001196788588[27] = 0;
   out_6682096001196788588[28] = 0;
   out_6682096001196788588[29] = 0;
   out_6682096001196788588[30] = 0;
   out_6682096001196788588[31] = 1;
   out_6682096001196788588[32] = 0;
   out_6682096001196788588[33] = 0;
   out_6682096001196788588[34] = 1;
   out_6682096001196788588[35] = 0;
   out_6682096001196788588[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_6682096001196788588[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_6682096001196788588[38] = 0;
   out_6682096001196788588[39] = -state[7];
   out_6682096001196788588[40] = state[6];
   out_6682096001196788588[41] = 0;
   out_6682096001196788588[42] = state[4];
   out_6682096001196788588[43] = -state[3];
   out_6682096001196788588[44] = 0;
   out_6682096001196788588[45] = 0;
   out_6682096001196788588[46] = 0;
   out_6682096001196788588[47] = 0;
   out_6682096001196788588[48] = 0;
   out_6682096001196788588[49] = 0;
   out_6682096001196788588[50] = 1;
   out_6682096001196788588[51] = 0;
   out_6682096001196788588[52] = 0;
   out_6682096001196788588[53] = 1;
}
void h_13(double *state, double *unused, double *out_1446024959322562397) {
   out_1446024959322562397[0] = state[3];
   out_1446024959322562397[1] = state[4];
   out_1446024959322562397[2] = state[5];
}
void H_13(double *state, double *unused, double *out_161951950224668978) {
   out_161951950224668978[0] = 0;
   out_161951950224668978[1] = 0;
   out_161951950224668978[2] = 0;
   out_161951950224668978[3] = 1;
   out_161951950224668978[4] = 0;
   out_161951950224668978[5] = 0;
   out_161951950224668978[6] = 0;
   out_161951950224668978[7] = 0;
   out_161951950224668978[8] = 0;
   out_161951950224668978[9] = 0;
   out_161951950224668978[10] = 0;
   out_161951950224668978[11] = 0;
   out_161951950224668978[12] = 0;
   out_161951950224668978[13] = 0;
   out_161951950224668978[14] = 0;
   out_161951950224668978[15] = 0;
   out_161951950224668978[16] = 0;
   out_161951950224668978[17] = 0;
   out_161951950224668978[18] = 0;
   out_161951950224668978[19] = 0;
   out_161951950224668978[20] = 0;
   out_161951950224668978[21] = 0;
   out_161951950224668978[22] = 1;
   out_161951950224668978[23] = 0;
   out_161951950224668978[24] = 0;
   out_161951950224668978[25] = 0;
   out_161951950224668978[26] = 0;
   out_161951950224668978[27] = 0;
   out_161951950224668978[28] = 0;
   out_161951950224668978[29] = 0;
   out_161951950224668978[30] = 0;
   out_161951950224668978[31] = 0;
   out_161951950224668978[32] = 0;
   out_161951950224668978[33] = 0;
   out_161951950224668978[34] = 0;
   out_161951950224668978[35] = 0;
   out_161951950224668978[36] = 0;
   out_161951950224668978[37] = 0;
   out_161951950224668978[38] = 0;
   out_161951950224668978[39] = 0;
   out_161951950224668978[40] = 0;
   out_161951950224668978[41] = 1;
   out_161951950224668978[42] = 0;
   out_161951950224668978[43] = 0;
   out_161951950224668978[44] = 0;
   out_161951950224668978[45] = 0;
   out_161951950224668978[46] = 0;
   out_161951950224668978[47] = 0;
   out_161951950224668978[48] = 0;
   out_161951950224668978[49] = 0;
   out_161951950224668978[50] = 0;
   out_161951950224668978[51] = 0;
   out_161951950224668978[52] = 0;
   out_161951950224668978[53] = 0;
}
void h_14(double *state, double *unused, double *out_7758273995292918031) {
   out_7758273995292918031[0] = state[6];
   out_7758273995292918031[1] = state[7];
   out_7758273995292918031[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6457014207852374075) {
   out_6457014207852374075[0] = 0;
   out_6457014207852374075[1] = 0;
   out_6457014207852374075[2] = 0;
   out_6457014207852374075[3] = 0;
   out_6457014207852374075[4] = 0;
   out_6457014207852374075[5] = 0;
   out_6457014207852374075[6] = 1;
   out_6457014207852374075[7] = 0;
   out_6457014207852374075[8] = 0;
   out_6457014207852374075[9] = 0;
   out_6457014207852374075[10] = 0;
   out_6457014207852374075[11] = 0;
   out_6457014207852374075[12] = 0;
   out_6457014207852374075[13] = 0;
   out_6457014207852374075[14] = 0;
   out_6457014207852374075[15] = 0;
   out_6457014207852374075[16] = 0;
   out_6457014207852374075[17] = 0;
   out_6457014207852374075[18] = 0;
   out_6457014207852374075[19] = 0;
   out_6457014207852374075[20] = 0;
   out_6457014207852374075[21] = 0;
   out_6457014207852374075[22] = 0;
   out_6457014207852374075[23] = 0;
   out_6457014207852374075[24] = 0;
   out_6457014207852374075[25] = 1;
   out_6457014207852374075[26] = 0;
   out_6457014207852374075[27] = 0;
   out_6457014207852374075[28] = 0;
   out_6457014207852374075[29] = 0;
   out_6457014207852374075[30] = 0;
   out_6457014207852374075[31] = 0;
   out_6457014207852374075[32] = 0;
   out_6457014207852374075[33] = 0;
   out_6457014207852374075[34] = 0;
   out_6457014207852374075[35] = 0;
   out_6457014207852374075[36] = 0;
   out_6457014207852374075[37] = 0;
   out_6457014207852374075[38] = 0;
   out_6457014207852374075[39] = 0;
   out_6457014207852374075[40] = 0;
   out_6457014207852374075[41] = 0;
   out_6457014207852374075[42] = 0;
   out_6457014207852374075[43] = 0;
   out_6457014207852374075[44] = 1;
   out_6457014207852374075[45] = 0;
   out_6457014207852374075[46] = 0;
   out_6457014207852374075[47] = 0;
   out_6457014207852374075[48] = 0;
   out_6457014207852374075[49] = 0;
   out_6457014207852374075[50] = 0;
   out_6457014207852374075[51] = 0;
   out_6457014207852374075[52] = 0;
   out_6457014207852374075[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_8138643469158452515) {
  err_fun(nom_x, delta_x, out_8138643469158452515);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4486085605069433673) {
  inv_err_fun(nom_x, true_x, out_4486085605069433673);
}
void pose_H_mod_fun(double *state, double *out_4128386777611714386) {
  H_mod_fun(state, out_4128386777611714386);
}
void pose_f_fun(double *state, double dt, double *out_3926695733139875668) {
  f_fun(state,  dt, out_3926695733139875668);
}
void pose_F_fun(double *state, double dt, double *out_3790507139778411869) {
  F_fun(state,  dt, out_3790507139778411869);
}
void pose_h_4(double *state, double *unused, double *out_7316226510605005089) {
  h_4(state, unused, out_7316226510605005089);
}
void pose_H_4(double *state, double *unused, double *out_3374225775557001779) {
  H_4(state, unused, out_3374225775557001779);
}
void pose_h_10(double *state, double *unused, double *out_2540837594140349822) {
  h_10(state, unused, out_2540837594140349822);
}
void pose_H_10(double *state, double *unused, double *out_6682096001196788588) {
  H_10(state, unused, out_6682096001196788588);
}
void pose_h_13(double *state, double *unused, double *out_1446024959322562397) {
  h_13(state, unused, out_1446024959322562397);
}
void pose_H_13(double *state, double *unused, double *out_161951950224668978) {
  H_13(state, unused, out_161951950224668978);
}
void pose_h_14(double *state, double *unused, double *out_7758273995292918031) {
  h_14(state, unused, out_7758273995292918031);
}
void pose_H_14(double *state, double *unused, double *out_6457014207852374075) {
  H_14(state, unused, out_6457014207852374075);
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
