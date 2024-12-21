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
void err_fun(double *nom_x, double *delta_x, double *out_7449750869360596060) {
   out_7449750869360596060[0] = delta_x[0] + nom_x[0];
   out_7449750869360596060[1] = delta_x[1] + nom_x[1];
   out_7449750869360596060[2] = delta_x[2] + nom_x[2];
   out_7449750869360596060[3] = delta_x[3] + nom_x[3];
   out_7449750869360596060[4] = delta_x[4] + nom_x[4];
   out_7449750869360596060[5] = delta_x[5] + nom_x[5];
   out_7449750869360596060[6] = delta_x[6] + nom_x[6];
   out_7449750869360596060[7] = delta_x[7] + nom_x[7];
   out_7449750869360596060[8] = delta_x[8] + nom_x[8];
   out_7449750869360596060[9] = delta_x[9] + nom_x[9];
   out_7449750869360596060[10] = delta_x[10] + nom_x[10];
   out_7449750869360596060[11] = delta_x[11] + nom_x[11];
   out_7449750869360596060[12] = delta_x[12] + nom_x[12];
   out_7449750869360596060[13] = delta_x[13] + nom_x[13];
   out_7449750869360596060[14] = delta_x[14] + nom_x[14];
   out_7449750869360596060[15] = delta_x[15] + nom_x[15];
   out_7449750869360596060[16] = delta_x[16] + nom_x[16];
   out_7449750869360596060[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_9017488319834403806) {
   out_9017488319834403806[0] = -nom_x[0] + true_x[0];
   out_9017488319834403806[1] = -nom_x[1] + true_x[1];
   out_9017488319834403806[2] = -nom_x[2] + true_x[2];
   out_9017488319834403806[3] = -nom_x[3] + true_x[3];
   out_9017488319834403806[4] = -nom_x[4] + true_x[4];
   out_9017488319834403806[5] = -nom_x[5] + true_x[5];
   out_9017488319834403806[6] = -nom_x[6] + true_x[6];
   out_9017488319834403806[7] = -nom_x[7] + true_x[7];
   out_9017488319834403806[8] = -nom_x[8] + true_x[8];
   out_9017488319834403806[9] = -nom_x[9] + true_x[9];
   out_9017488319834403806[10] = -nom_x[10] + true_x[10];
   out_9017488319834403806[11] = -nom_x[11] + true_x[11];
   out_9017488319834403806[12] = -nom_x[12] + true_x[12];
   out_9017488319834403806[13] = -nom_x[13] + true_x[13];
   out_9017488319834403806[14] = -nom_x[14] + true_x[14];
   out_9017488319834403806[15] = -nom_x[15] + true_x[15];
   out_9017488319834403806[16] = -nom_x[16] + true_x[16];
   out_9017488319834403806[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_5161440433737183966) {
   out_5161440433737183966[0] = 1.0;
   out_5161440433737183966[1] = 0.0;
   out_5161440433737183966[2] = 0.0;
   out_5161440433737183966[3] = 0.0;
   out_5161440433737183966[4] = 0.0;
   out_5161440433737183966[5] = 0.0;
   out_5161440433737183966[6] = 0.0;
   out_5161440433737183966[7] = 0.0;
   out_5161440433737183966[8] = 0.0;
   out_5161440433737183966[9] = 0.0;
   out_5161440433737183966[10] = 0.0;
   out_5161440433737183966[11] = 0.0;
   out_5161440433737183966[12] = 0.0;
   out_5161440433737183966[13] = 0.0;
   out_5161440433737183966[14] = 0.0;
   out_5161440433737183966[15] = 0.0;
   out_5161440433737183966[16] = 0.0;
   out_5161440433737183966[17] = 0.0;
   out_5161440433737183966[18] = 0.0;
   out_5161440433737183966[19] = 1.0;
   out_5161440433737183966[20] = 0.0;
   out_5161440433737183966[21] = 0.0;
   out_5161440433737183966[22] = 0.0;
   out_5161440433737183966[23] = 0.0;
   out_5161440433737183966[24] = 0.0;
   out_5161440433737183966[25] = 0.0;
   out_5161440433737183966[26] = 0.0;
   out_5161440433737183966[27] = 0.0;
   out_5161440433737183966[28] = 0.0;
   out_5161440433737183966[29] = 0.0;
   out_5161440433737183966[30] = 0.0;
   out_5161440433737183966[31] = 0.0;
   out_5161440433737183966[32] = 0.0;
   out_5161440433737183966[33] = 0.0;
   out_5161440433737183966[34] = 0.0;
   out_5161440433737183966[35] = 0.0;
   out_5161440433737183966[36] = 0.0;
   out_5161440433737183966[37] = 0.0;
   out_5161440433737183966[38] = 1.0;
   out_5161440433737183966[39] = 0.0;
   out_5161440433737183966[40] = 0.0;
   out_5161440433737183966[41] = 0.0;
   out_5161440433737183966[42] = 0.0;
   out_5161440433737183966[43] = 0.0;
   out_5161440433737183966[44] = 0.0;
   out_5161440433737183966[45] = 0.0;
   out_5161440433737183966[46] = 0.0;
   out_5161440433737183966[47] = 0.0;
   out_5161440433737183966[48] = 0.0;
   out_5161440433737183966[49] = 0.0;
   out_5161440433737183966[50] = 0.0;
   out_5161440433737183966[51] = 0.0;
   out_5161440433737183966[52] = 0.0;
   out_5161440433737183966[53] = 0.0;
   out_5161440433737183966[54] = 0.0;
   out_5161440433737183966[55] = 0.0;
   out_5161440433737183966[56] = 0.0;
   out_5161440433737183966[57] = 1.0;
   out_5161440433737183966[58] = 0.0;
   out_5161440433737183966[59] = 0.0;
   out_5161440433737183966[60] = 0.0;
   out_5161440433737183966[61] = 0.0;
   out_5161440433737183966[62] = 0.0;
   out_5161440433737183966[63] = 0.0;
   out_5161440433737183966[64] = 0.0;
   out_5161440433737183966[65] = 0.0;
   out_5161440433737183966[66] = 0.0;
   out_5161440433737183966[67] = 0.0;
   out_5161440433737183966[68] = 0.0;
   out_5161440433737183966[69] = 0.0;
   out_5161440433737183966[70] = 0.0;
   out_5161440433737183966[71] = 0.0;
   out_5161440433737183966[72] = 0.0;
   out_5161440433737183966[73] = 0.0;
   out_5161440433737183966[74] = 0.0;
   out_5161440433737183966[75] = 0.0;
   out_5161440433737183966[76] = 1.0;
   out_5161440433737183966[77] = 0.0;
   out_5161440433737183966[78] = 0.0;
   out_5161440433737183966[79] = 0.0;
   out_5161440433737183966[80] = 0.0;
   out_5161440433737183966[81] = 0.0;
   out_5161440433737183966[82] = 0.0;
   out_5161440433737183966[83] = 0.0;
   out_5161440433737183966[84] = 0.0;
   out_5161440433737183966[85] = 0.0;
   out_5161440433737183966[86] = 0.0;
   out_5161440433737183966[87] = 0.0;
   out_5161440433737183966[88] = 0.0;
   out_5161440433737183966[89] = 0.0;
   out_5161440433737183966[90] = 0.0;
   out_5161440433737183966[91] = 0.0;
   out_5161440433737183966[92] = 0.0;
   out_5161440433737183966[93] = 0.0;
   out_5161440433737183966[94] = 0.0;
   out_5161440433737183966[95] = 1.0;
   out_5161440433737183966[96] = 0.0;
   out_5161440433737183966[97] = 0.0;
   out_5161440433737183966[98] = 0.0;
   out_5161440433737183966[99] = 0.0;
   out_5161440433737183966[100] = 0.0;
   out_5161440433737183966[101] = 0.0;
   out_5161440433737183966[102] = 0.0;
   out_5161440433737183966[103] = 0.0;
   out_5161440433737183966[104] = 0.0;
   out_5161440433737183966[105] = 0.0;
   out_5161440433737183966[106] = 0.0;
   out_5161440433737183966[107] = 0.0;
   out_5161440433737183966[108] = 0.0;
   out_5161440433737183966[109] = 0.0;
   out_5161440433737183966[110] = 0.0;
   out_5161440433737183966[111] = 0.0;
   out_5161440433737183966[112] = 0.0;
   out_5161440433737183966[113] = 0.0;
   out_5161440433737183966[114] = 1.0;
   out_5161440433737183966[115] = 0.0;
   out_5161440433737183966[116] = 0.0;
   out_5161440433737183966[117] = 0.0;
   out_5161440433737183966[118] = 0.0;
   out_5161440433737183966[119] = 0.0;
   out_5161440433737183966[120] = 0.0;
   out_5161440433737183966[121] = 0.0;
   out_5161440433737183966[122] = 0.0;
   out_5161440433737183966[123] = 0.0;
   out_5161440433737183966[124] = 0.0;
   out_5161440433737183966[125] = 0.0;
   out_5161440433737183966[126] = 0.0;
   out_5161440433737183966[127] = 0.0;
   out_5161440433737183966[128] = 0.0;
   out_5161440433737183966[129] = 0.0;
   out_5161440433737183966[130] = 0.0;
   out_5161440433737183966[131] = 0.0;
   out_5161440433737183966[132] = 0.0;
   out_5161440433737183966[133] = 1.0;
   out_5161440433737183966[134] = 0.0;
   out_5161440433737183966[135] = 0.0;
   out_5161440433737183966[136] = 0.0;
   out_5161440433737183966[137] = 0.0;
   out_5161440433737183966[138] = 0.0;
   out_5161440433737183966[139] = 0.0;
   out_5161440433737183966[140] = 0.0;
   out_5161440433737183966[141] = 0.0;
   out_5161440433737183966[142] = 0.0;
   out_5161440433737183966[143] = 0.0;
   out_5161440433737183966[144] = 0.0;
   out_5161440433737183966[145] = 0.0;
   out_5161440433737183966[146] = 0.0;
   out_5161440433737183966[147] = 0.0;
   out_5161440433737183966[148] = 0.0;
   out_5161440433737183966[149] = 0.0;
   out_5161440433737183966[150] = 0.0;
   out_5161440433737183966[151] = 0.0;
   out_5161440433737183966[152] = 1.0;
   out_5161440433737183966[153] = 0.0;
   out_5161440433737183966[154] = 0.0;
   out_5161440433737183966[155] = 0.0;
   out_5161440433737183966[156] = 0.0;
   out_5161440433737183966[157] = 0.0;
   out_5161440433737183966[158] = 0.0;
   out_5161440433737183966[159] = 0.0;
   out_5161440433737183966[160] = 0.0;
   out_5161440433737183966[161] = 0.0;
   out_5161440433737183966[162] = 0.0;
   out_5161440433737183966[163] = 0.0;
   out_5161440433737183966[164] = 0.0;
   out_5161440433737183966[165] = 0.0;
   out_5161440433737183966[166] = 0.0;
   out_5161440433737183966[167] = 0.0;
   out_5161440433737183966[168] = 0.0;
   out_5161440433737183966[169] = 0.0;
   out_5161440433737183966[170] = 0.0;
   out_5161440433737183966[171] = 1.0;
   out_5161440433737183966[172] = 0.0;
   out_5161440433737183966[173] = 0.0;
   out_5161440433737183966[174] = 0.0;
   out_5161440433737183966[175] = 0.0;
   out_5161440433737183966[176] = 0.0;
   out_5161440433737183966[177] = 0.0;
   out_5161440433737183966[178] = 0.0;
   out_5161440433737183966[179] = 0.0;
   out_5161440433737183966[180] = 0.0;
   out_5161440433737183966[181] = 0.0;
   out_5161440433737183966[182] = 0.0;
   out_5161440433737183966[183] = 0.0;
   out_5161440433737183966[184] = 0.0;
   out_5161440433737183966[185] = 0.0;
   out_5161440433737183966[186] = 0.0;
   out_5161440433737183966[187] = 0.0;
   out_5161440433737183966[188] = 0.0;
   out_5161440433737183966[189] = 0.0;
   out_5161440433737183966[190] = 1.0;
   out_5161440433737183966[191] = 0.0;
   out_5161440433737183966[192] = 0.0;
   out_5161440433737183966[193] = 0.0;
   out_5161440433737183966[194] = 0.0;
   out_5161440433737183966[195] = 0.0;
   out_5161440433737183966[196] = 0.0;
   out_5161440433737183966[197] = 0.0;
   out_5161440433737183966[198] = 0.0;
   out_5161440433737183966[199] = 0.0;
   out_5161440433737183966[200] = 0.0;
   out_5161440433737183966[201] = 0.0;
   out_5161440433737183966[202] = 0.0;
   out_5161440433737183966[203] = 0.0;
   out_5161440433737183966[204] = 0.0;
   out_5161440433737183966[205] = 0.0;
   out_5161440433737183966[206] = 0.0;
   out_5161440433737183966[207] = 0.0;
   out_5161440433737183966[208] = 0.0;
   out_5161440433737183966[209] = 1.0;
   out_5161440433737183966[210] = 0.0;
   out_5161440433737183966[211] = 0.0;
   out_5161440433737183966[212] = 0.0;
   out_5161440433737183966[213] = 0.0;
   out_5161440433737183966[214] = 0.0;
   out_5161440433737183966[215] = 0.0;
   out_5161440433737183966[216] = 0.0;
   out_5161440433737183966[217] = 0.0;
   out_5161440433737183966[218] = 0.0;
   out_5161440433737183966[219] = 0.0;
   out_5161440433737183966[220] = 0.0;
   out_5161440433737183966[221] = 0.0;
   out_5161440433737183966[222] = 0.0;
   out_5161440433737183966[223] = 0.0;
   out_5161440433737183966[224] = 0.0;
   out_5161440433737183966[225] = 0.0;
   out_5161440433737183966[226] = 0.0;
   out_5161440433737183966[227] = 0.0;
   out_5161440433737183966[228] = 1.0;
   out_5161440433737183966[229] = 0.0;
   out_5161440433737183966[230] = 0.0;
   out_5161440433737183966[231] = 0.0;
   out_5161440433737183966[232] = 0.0;
   out_5161440433737183966[233] = 0.0;
   out_5161440433737183966[234] = 0.0;
   out_5161440433737183966[235] = 0.0;
   out_5161440433737183966[236] = 0.0;
   out_5161440433737183966[237] = 0.0;
   out_5161440433737183966[238] = 0.0;
   out_5161440433737183966[239] = 0.0;
   out_5161440433737183966[240] = 0.0;
   out_5161440433737183966[241] = 0.0;
   out_5161440433737183966[242] = 0.0;
   out_5161440433737183966[243] = 0.0;
   out_5161440433737183966[244] = 0.0;
   out_5161440433737183966[245] = 0.0;
   out_5161440433737183966[246] = 0.0;
   out_5161440433737183966[247] = 1.0;
   out_5161440433737183966[248] = 0.0;
   out_5161440433737183966[249] = 0.0;
   out_5161440433737183966[250] = 0.0;
   out_5161440433737183966[251] = 0.0;
   out_5161440433737183966[252] = 0.0;
   out_5161440433737183966[253] = 0.0;
   out_5161440433737183966[254] = 0.0;
   out_5161440433737183966[255] = 0.0;
   out_5161440433737183966[256] = 0.0;
   out_5161440433737183966[257] = 0.0;
   out_5161440433737183966[258] = 0.0;
   out_5161440433737183966[259] = 0.0;
   out_5161440433737183966[260] = 0.0;
   out_5161440433737183966[261] = 0.0;
   out_5161440433737183966[262] = 0.0;
   out_5161440433737183966[263] = 0.0;
   out_5161440433737183966[264] = 0.0;
   out_5161440433737183966[265] = 0.0;
   out_5161440433737183966[266] = 1.0;
   out_5161440433737183966[267] = 0.0;
   out_5161440433737183966[268] = 0.0;
   out_5161440433737183966[269] = 0.0;
   out_5161440433737183966[270] = 0.0;
   out_5161440433737183966[271] = 0.0;
   out_5161440433737183966[272] = 0.0;
   out_5161440433737183966[273] = 0.0;
   out_5161440433737183966[274] = 0.0;
   out_5161440433737183966[275] = 0.0;
   out_5161440433737183966[276] = 0.0;
   out_5161440433737183966[277] = 0.0;
   out_5161440433737183966[278] = 0.0;
   out_5161440433737183966[279] = 0.0;
   out_5161440433737183966[280] = 0.0;
   out_5161440433737183966[281] = 0.0;
   out_5161440433737183966[282] = 0.0;
   out_5161440433737183966[283] = 0.0;
   out_5161440433737183966[284] = 0.0;
   out_5161440433737183966[285] = 1.0;
   out_5161440433737183966[286] = 0.0;
   out_5161440433737183966[287] = 0.0;
   out_5161440433737183966[288] = 0.0;
   out_5161440433737183966[289] = 0.0;
   out_5161440433737183966[290] = 0.0;
   out_5161440433737183966[291] = 0.0;
   out_5161440433737183966[292] = 0.0;
   out_5161440433737183966[293] = 0.0;
   out_5161440433737183966[294] = 0.0;
   out_5161440433737183966[295] = 0.0;
   out_5161440433737183966[296] = 0.0;
   out_5161440433737183966[297] = 0.0;
   out_5161440433737183966[298] = 0.0;
   out_5161440433737183966[299] = 0.0;
   out_5161440433737183966[300] = 0.0;
   out_5161440433737183966[301] = 0.0;
   out_5161440433737183966[302] = 0.0;
   out_5161440433737183966[303] = 0.0;
   out_5161440433737183966[304] = 1.0;
   out_5161440433737183966[305] = 0.0;
   out_5161440433737183966[306] = 0.0;
   out_5161440433737183966[307] = 0.0;
   out_5161440433737183966[308] = 0.0;
   out_5161440433737183966[309] = 0.0;
   out_5161440433737183966[310] = 0.0;
   out_5161440433737183966[311] = 0.0;
   out_5161440433737183966[312] = 0.0;
   out_5161440433737183966[313] = 0.0;
   out_5161440433737183966[314] = 0.0;
   out_5161440433737183966[315] = 0.0;
   out_5161440433737183966[316] = 0.0;
   out_5161440433737183966[317] = 0.0;
   out_5161440433737183966[318] = 0.0;
   out_5161440433737183966[319] = 0.0;
   out_5161440433737183966[320] = 0.0;
   out_5161440433737183966[321] = 0.0;
   out_5161440433737183966[322] = 0.0;
   out_5161440433737183966[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_4374988147753332159) {
   out_4374988147753332159[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_4374988147753332159[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_4374988147753332159[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_4374988147753332159[3] = dt*state[12] + state[3];
   out_4374988147753332159[4] = dt*state[13] + state[4];
   out_4374988147753332159[5] = dt*state[14] + state[5];
   out_4374988147753332159[6] = state[6];
   out_4374988147753332159[7] = state[7];
   out_4374988147753332159[8] = state[8];
   out_4374988147753332159[9] = state[9];
   out_4374988147753332159[10] = state[10];
   out_4374988147753332159[11] = state[11];
   out_4374988147753332159[12] = state[12];
   out_4374988147753332159[13] = state[13];
   out_4374988147753332159[14] = state[14];
   out_4374988147753332159[15] = state[15];
   out_4374988147753332159[16] = state[16];
   out_4374988147753332159[17] = state[17];
}
void F_fun(double *state, double dt, double *out_4799445358667755900) {
   out_4799445358667755900[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4799445358667755900[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4799445358667755900[2] = 0;
   out_4799445358667755900[3] = 0;
   out_4799445358667755900[4] = 0;
   out_4799445358667755900[5] = 0;
   out_4799445358667755900[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4799445358667755900[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4799445358667755900[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4799445358667755900[9] = 0;
   out_4799445358667755900[10] = 0;
   out_4799445358667755900[11] = 0;
   out_4799445358667755900[12] = 0;
   out_4799445358667755900[13] = 0;
   out_4799445358667755900[14] = 0;
   out_4799445358667755900[15] = 0;
   out_4799445358667755900[16] = 0;
   out_4799445358667755900[17] = 0;
   out_4799445358667755900[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4799445358667755900[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4799445358667755900[20] = 0;
   out_4799445358667755900[21] = 0;
   out_4799445358667755900[22] = 0;
   out_4799445358667755900[23] = 0;
   out_4799445358667755900[24] = 0;
   out_4799445358667755900[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4799445358667755900[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4799445358667755900[27] = 0;
   out_4799445358667755900[28] = 0;
   out_4799445358667755900[29] = 0;
   out_4799445358667755900[30] = 0;
   out_4799445358667755900[31] = 0;
   out_4799445358667755900[32] = 0;
   out_4799445358667755900[33] = 0;
   out_4799445358667755900[34] = 0;
   out_4799445358667755900[35] = 0;
   out_4799445358667755900[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4799445358667755900[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4799445358667755900[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4799445358667755900[39] = 0;
   out_4799445358667755900[40] = 0;
   out_4799445358667755900[41] = 0;
   out_4799445358667755900[42] = 0;
   out_4799445358667755900[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4799445358667755900[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4799445358667755900[45] = 0;
   out_4799445358667755900[46] = 0;
   out_4799445358667755900[47] = 0;
   out_4799445358667755900[48] = 0;
   out_4799445358667755900[49] = 0;
   out_4799445358667755900[50] = 0;
   out_4799445358667755900[51] = 0;
   out_4799445358667755900[52] = 0;
   out_4799445358667755900[53] = 0;
   out_4799445358667755900[54] = 0;
   out_4799445358667755900[55] = 0;
   out_4799445358667755900[56] = 0;
   out_4799445358667755900[57] = 1;
   out_4799445358667755900[58] = 0;
   out_4799445358667755900[59] = 0;
   out_4799445358667755900[60] = 0;
   out_4799445358667755900[61] = 0;
   out_4799445358667755900[62] = 0;
   out_4799445358667755900[63] = 0;
   out_4799445358667755900[64] = 0;
   out_4799445358667755900[65] = 0;
   out_4799445358667755900[66] = dt;
   out_4799445358667755900[67] = 0;
   out_4799445358667755900[68] = 0;
   out_4799445358667755900[69] = 0;
   out_4799445358667755900[70] = 0;
   out_4799445358667755900[71] = 0;
   out_4799445358667755900[72] = 0;
   out_4799445358667755900[73] = 0;
   out_4799445358667755900[74] = 0;
   out_4799445358667755900[75] = 0;
   out_4799445358667755900[76] = 1;
   out_4799445358667755900[77] = 0;
   out_4799445358667755900[78] = 0;
   out_4799445358667755900[79] = 0;
   out_4799445358667755900[80] = 0;
   out_4799445358667755900[81] = 0;
   out_4799445358667755900[82] = 0;
   out_4799445358667755900[83] = 0;
   out_4799445358667755900[84] = 0;
   out_4799445358667755900[85] = dt;
   out_4799445358667755900[86] = 0;
   out_4799445358667755900[87] = 0;
   out_4799445358667755900[88] = 0;
   out_4799445358667755900[89] = 0;
   out_4799445358667755900[90] = 0;
   out_4799445358667755900[91] = 0;
   out_4799445358667755900[92] = 0;
   out_4799445358667755900[93] = 0;
   out_4799445358667755900[94] = 0;
   out_4799445358667755900[95] = 1;
   out_4799445358667755900[96] = 0;
   out_4799445358667755900[97] = 0;
   out_4799445358667755900[98] = 0;
   out_4799445358667755900[99] = 0;
   out_4799445358667755900[100] = 0;
   out_4799445358667755900[101] = 0;
   out_4799445358667755900[102] = 0;
   out_4799445358667755900[103] = 0;
   out_4799445358667755900[104] = dt;
   out_4799445358667755900[105] = 0;
   out_4799445358667755900[106] = 0;
   out_4799445358667755900[107] = 0;
   out_4799445358667755900[108] = 0;
   out_4799445358667755900[109] = 0;
   out_4799445358667755900[110] = 0;
   out_4799445358667755900[111] = 0;
   out_4799445358667755900[112] = 0;
   out_4799445358667755900[113] = 0;
   out_4799445358667755900[114] = 1;
   out_4799445358667755900[115] = 0;
   out_4799445358667755900[116] = 0;
   out_4799445358667755900[117] = 0;
   out_4799445358667755900[118] = 0;
   out_4799445358667755900[119] = 0;
   out_4799445358667755900[120] = 0;
   out_4799445358667755900[121] = 0;
   out_4799445358667755900[122] = 0;
   out_4799445358667755900[123] = 0;
   out_4799445358667755900[124] = 0;
   out_4799445358667755900[125] = 0;
   out_4799445358667755900[126] = 0;
   out_4799445358667755900[127] = 0;
   out_4799445358667755900[128] = 0;
   out_4799445358667755900[129] = 0;
   out_4799445358667755900[130] = 0;
   out_4799445358667755900[131] = 0;
   out_4799445358667755900[132] = 0;
   out_4799445358667755900[133] = 1;
   out_4799445358667755900[134] = 0;
   out_4799445358667755900[135] = 0;
   out_4799445358667755900[136] = 0;
   out_4799445358667755900[137] = 0;
   out_4799445358667755900[138] = 0;
   out_4799445358667755900[139] = 0;
   out_4799445358667755900[140] = 0;
   out_4799445358667755900[141] = 0;
   out_4799445358667755900[142] = 0;
   out_4799445358667755900[143] = 0;
   out_4799445358667755900[144] = 0;
   out_4799445358667755900[145] = 0;
   out_4799445358667755900[146] = 0;
   out_4799445358667755900[147] = 0;
   out_4799445358667755900[148] = 0;
   out_4799445358667755900[149] = 0;
   out_4799445358667755900[150] = 0;
   out_4799445358667755900[151] = 0;
   out_4799445358667755900[152] = 1;
   out_4799445358667755900[153] = 0;
   out_4799445358667755900[154] = 0;
   out_4799445358667755900[155] = 0;
   out_4799445358667755900[156] = 0;
   out_4799445358667755900[157] = 0;
   out_4799445358667755900[158] = 0;
   out_4799445358667755900[159] = 0;
   out_4799445358667755900[160] = 0;
   out_4799445358667755900[161] = 0;
   out_4799445358667755900[162] = 0;
   out_4799445358667755900[163] = 0;
   out_4799445358667755900[164] = 0;
   out_4799445358667755900[165] = 0;
   out_4799445358667755900[166] = 0;
   out_4799445358667755900[167] = 0;
   out_4799445358667755900[168] = 0;
   out_4799445358667755900[169] = 0;
   out_4799445358667755900[170] = 0;
   out_4799445358667755900[171] = 1;
   out_4799445358667755900[172] = 0;
   out_4799445358667755900[173] = 0;
   out_4799445358667755900[174] = 0;
   out_4799445358667755900[175] = 0;
   out_4799445358667755900[176] = 0;
   out_4799445358667755900[177] = 0;
   out_4799445358667755900[178] = 0;
   out_4799445358667755900[179] = 0;
   out_4799445358667755900[180] = 0;
   out_4799445358667755900[181] = 0;
   out_4799445358667755900[182] = 0;
   out_4799445358667755900[183] = 0;
   out_4799445358667755900[184] = 0;
   out_4799445358667755900[185] = 0;
   out_4799445358667755900[186] = 0;
   out_4799445358667755900[187] = 0;
   out_4799445358667755900[188] = 0;
   out_4799445358667755900[189] = 0;
   out_4799445358667755900[190] = 1;
   out_4799445358667755900[191] = 0;
   out_4799445358667755900[192] = 0;
   out_4799445358667755900[193] = 0;
   out_4799445358667755900[194] = 0;
   out_4799445358667755900[195] = 0;
   out_4799445358667755900[196] = 0;
   out_4799445358667755900[197] = 0;
   out_4799445358667755900[198] = 0;
   out_4799445358667755900[199] = 0;
   out_4799445358667755900[200] = 0;
   out_4799445358667755900[201] = 0;
   out_4799445358667755900[202] = 0;
   out_4799445358667755900[203] = 0;
   out_4799445358667755900[204] = 0;
   out_4799445358667755900[205] = 0;
   out_4799445358667755900[206] = 0;
   out_4799445358667755900[207] = 0;
   out_4799445358667755900[208] = 0;
   out_4799445358667755900[209] = 1;
   out_4799445358667755900[210] = 0;
   out_4799445358667755900[211] = 0;
   out_4799445358667755900[212] = 0;
   out_4799445358667755900[213] = 0;
   out_4799445358667755900[214] = 0;
   out_4799445358667755900[215] = 0;
   out_4799445358667755900[216] = 0;
   out_4799445358667755900[217] = 0;
   out_4799445358667755900[218] = 0;
   out_4799445358667755900[219] = 0;
   out_4799445358667755900[220] = 0;
   out_4799445358667755900[221] = 0;
   out_4799445358667755900[222] = 0;
   out_4799445358667755900[223] = 0;
   out_4799445358667755900[224] = 0;
   out_4799445358667755900[225] = 0;
   out_4799445358667755900[226] = 0;
   out_4799445358667755900[227] = 0;
   out_4799445358667755900[228] = 1;
   out_4799445358667755900[229] = 0;
   out_4799445358667755900[230] = 0;
   out_4799445358667755900[231] = 0;
   out_4799445358667755900[232] = 0;
   out_4799445358667755900[233] = 0;
   out_4799445358667755900[234] = 0;
   out_4799445358667755900[235] = 0;
   out_4799445358667755900[236] = 0;
   out_4799445358667755900[237] = 0;
   out_4799445358667755900[238] = 0;
   out_4799445358667755900[239] = 0;
   out_4799445358667755900[240] = 0;
   out_4799445358667755900[241] = 0;
   out_4799445358667755900[242] = 0;
   out_4799445358667755900[243] = 0;
   out_4799445358667755900[244] = 0;
   out_4799445358667755900[245] = 0;
   out_4799445358667755900[246] = 0;
   out_4799445358667755900[247] = 1;
   out_4799445358667755900[248] = 0;
   out_4799445358667755900[249] = 0;
   out_4799445358667755900[250] = 0;
   out_4799445358667755900[251] = 0;
   out_4799445358667755900[252] = 0;
   out_4799445358667755900[253] = 0;
   out_4799445358667755900[254] = 0;
   out_4799445358667755900[255] = 0;
   out_4799445358667755900[256] = 0;
   out_4799445358667755900[257] = 0;
   out_4799445358667755900[258] = 0;
   out_4799445358667755900[259] = 0;
   out_4799445358667755900[260] = 0;
   out_4799445358667755900[261] = 0;
   out_4799445358667755900[262] = 0;
   out_4799445358667755900[263] = 0;
   out_4799445358667755900[264] = 0;
   out_4799445358667755900[265] = 0;
   out_4799445358667755900[266] = 1;
   out_4799445358667755900[267] = 0;
   out_4799445358667755900[268] = 0;
   out_4799445358667755900[269] = 0;
   out_4799445358667755900[270] = 0;
   out_4799445358667755900[271] = 0;
   out_4799445358667755900[272] = 0;
   out_4799445358667755900[273] = 0;
   out_4799445358667755900[274] = 0;
   out_4799445358667755900[275] = 0;
   out_4799445358667755900[276] = 0;
   out_4799445358667755900[277] = 0;
   out_4799445358667755900[278] = 0;
   out_4799445358667755900[279] = 0;
   out_4799445358667755900[280] = 0;
   out_4799445358667755900[281] = 0;
   out_4799445358667755900[282] = 0;
   out_4799445358667755900[283] = 0;
   out_4799445358667755900[284] = 0;
   out_4799445358667755900[285] = 1;
   out_4799445358667755900[286] = 0;
   out_4799445358667755900[287] = 0;
   out_4799445358667755900[288] = 0;
   out_4799445358667755900[289] = 0;
   out_4799445358667755900[290] = 0;
   out_4799445358667755900[291] = 0;
   out_4799445358667755900[292] = 0;
   out_4799445358667755900[293] = 0;
   out_4799445358667755900[294] = 0;
   out_4799445358667755900[295] = 0;
   out_4799445358667755900[296] = 0;
   out_4799445358667755900[297] = 0;
   out_4799445358667755900[298] = 0;
   out_4799445358667755900[299] = 0;
   out_4799445358667755900[300] = 0;
   out_4799445358667755900[301] = 0;
   out_4799445358667755900[302] = 0;
   out_4799445358667755900[303] = 0;
   out_4799445358667755900[304] = 1;
   out_4799445358667755900[305] = 0;
   out_4799445358667755900[306] = 0;
   out_4799445358667755900[307] = 0;
   out_4799445358667755900[308] = 0;
   out_4799445358667755900[309] = 0;
   out_4799445358667755900[310] = 0;
   out_4799445358667755900[311] = 0;
   out_4799445358667755900[312] = 0;
   out_4799445358667755900[313] = 0;
   out_4799445358667755900[314] = 0;
   out_4799445358667755900[315] = 0;
   out_4799445358667755900[316] = 0;
   out_4799445358667755900[317] = 0;
   out_4799445358667755900[318] = 0;
   out_4799445358667755900[319] = 0;
   out_4799445358667755900[320] = 0;
   out_4799445358667755900[321] = 0;
   out_4799445358667755900[322] = 0;
   out_4799445358667755900[323] = 1;
}
void h_4(double *state, double *unused, double *out_5495389517486282336) {
   out_5495389517486282336[0] = state[6] + state[9];
   out_5495389517486282336[1] = state[7] + state[10];
   out_5495389517486282336[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_3963492951854285862) {
   out_3963492951854285862[0] = 0;
   out_3963492951854285862[1] = 0;
   out_3963492951854285862[2] = 0;
   out_3963492951854285862[3] = 0;
   out_3963492951854285862[4] = 0;
   out_3963492951854285862[5] = 0;
   out_3963492951854285862[6] = 1;
   out_3963492951854285862[7] = 0;
   out_3963492951854285862[8] = 0;
   out_3963492951854285862[9] = 1;
   out_3963492951854285862[10] = 0;
   out_3963492951854285862[11] = 0;
   out_3963492951854285862[12] = 0;
   out_3963492951854285862[13] = 0;
   out_3963492951854285862[14] = 0;
   out_3963492951854285862[15] = 0;
   out_3963492951854285862[16] = 0;
   out_3963492951854285862[17] = 0;
   out_3963492951854285862[18] = 0;
   out_3963492951854285862[19] = 0;
   out_3963492951854285862[20] = 0;
   out_3963492951854285862[21] = 0;
   out_3963492951854285862[22] = 0;
   out_3963492951854285862[23] = 0;
   out_3963492951854285862[24] = 0;
   out_3963492951854285862[25] = 1;
   out_3963492951854285862[26] = 0;
   out_3963492951854285862[27] = 0;
   out_3963492951854285862[28] = 1;
   out_3963492951854285862[29] = 0;
   out_3963492951854285862[30] = 0;
   out_3963492951854285862[31] = 0;
   out_3963492951854285862[32] = 0;
   out_3963492951854285862[33] = 0;
   out_3963492951854285862[34] = 0;
   out_3963492951854285862[35] = 0;
   out_3963492951854285862[36] = 0;
   out_3963492951854285862[37] = 0;
   out_3963492951854285862[38] = 0;
   out_3963492951854285862[39] = 0;
   out_3963492951854285862[40] = 0;
   out_3963492951854285862[41] = 0;
   out_3963492951854285862[42] = 0;
   out_3963492951854285862[43] = 0;
   out_3963492951854285862[44] = 1;
   out_3963492951854285862[45] = 0;
   out_3963492951854285862[46] = 0;
   out_3963492951854285862[47] = 1;
   out_3963492951854285862[48] = 0;
   out_3963492951854285862[49] = 0;
   out_3963492951854285862[50] = 0;
   out_3963492951854285862[51] = 0;
   out_3963492951854285862[52] = 0;
   out_3963492951854285862[53] = 0;
}
void h_10(double *state, double *unused, double *out_3796979241642905601) {
   out_3796979241642905601[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_3796979241642905601[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_3796979241642905601[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_2798195339350489049) {
   out_2798195339350489049[0] = 0;
   out_2798195339350489049[1] = 9.8100000000000005*cos(state[1]);
   out_2798195339350489049[2] = 0;
   out_2798195339350489049[3] = 0;
   out_2798195339350489049[4] = -state[8];
   out_2798195339350489049[5] = state[7];
   out_2798195339350489049[6] = 0;
   out_2798195339350489049[7] = state[5];
   out_2798195339350489049[8] = -state[4];
   out_2798195339350489049[9] = 0;
   out_2798195339350489049[10] = 0;
   out_2798195339350489049[11] = 0;
   out_2798195339350489049[12] = 1;
   out_2798195339350489049[13] = 0;
   out_2798195339350489049[14] = 0;
   out_2798195339350489049[15] = 1;
   out_2798195339350489049[16] = 0;
   out_2798195339350489049[17] = 0;
   out_2798195339350489049[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_2798195339350489049[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_2798195339350489049[20] = 0;
   out_2798195339350489049[21] = state[8];
   out_2798195339350489049[22] = 0;
   out_2798195339350489049[23] = -state[6];
   out_2798195339350489049[24] = -state[5];
   out_2798195339350489049[25] = 0;
   out_2798195339350489049[26] = state[3];
   out_2798195339350489049[27] = 0;
   out_2798195339350489049[28] = 0;
   out_2798195339350489049[29] = 0;
   out_2798195339350489049[30] = 0;
   out_2798195339350489049[31] = 1;
   out_2798195339350489049[32] = 0;
   out_2798195339350489049[33] = 0;
   out_2798195339350489049[34] = 1;
   out_2798195339350489049[35] = 0;
   out_2798195339350489049[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_2798195339350489049[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_2798195339350489049[38] = 0;
   out_2798195339350489049[39] = -state[7];
   out_2798195339350489049[40] = state[6];
   out_2798195339350489049[41] = 0;
   out_2798195339350489049[42] = state[4];
   out_2798195339350489049[43] = -state[3];
   out_2798195339350489049[44] = 0;
   out_2798195339350489049[45] = 0;
   out_2798195339350489049[46] = 0;
   out_2798195339350489049[47] = 0;
   out_2798195339350489049[48] = 0;
   out_2798195339350489049[49] = 0;
   out_2798195339350489049[50] = 1;
   out_2798195339350489049[51] = 0;
   out_2798195339350489049[52] = 0;
   out_2798195339350489049[53] = 1;
}
void h_13(double *state, double *unused, double *out_2405306930266407283) {
   out_2405306930266407283[0] = state[3];
   out_2405306930266407283[1] = state[4];
   out_2405306930266407283[2] = state[5];
}
void H_13(double *state, double *unused, double *out_751219126521953061) {
   out_751219126521953061[0] = 0;
   out_751219126521953061[1] = 0;
   out_751219126521953061[2] = 0;
   out_751219126521953061[3] = 1;
   out_751219126521953061[4] = 0;
   out_751219126521953061[5] = 0;
   out_751219126521953061[6] = 0;
   out_751219126521953061[7] = 0;
   out_751219126521953061[8] = 0;
   out_751219126521953061[9] = 0;
   out_751219126521953061[10] = 0;
   out_751219126521953061[11] = 0;
   out_751219126521953061[12] = 0;
   out_751219126521953061[13] = 0;
   out_751219126521953061[14] = 0;
   out_751219126521953061[15] = 0;
   out_751219126521953061[16] = 0;
   out_751219126521953061[17] = 0;
   out_751219126521953061[18] = 0;
   out_751219126521953061[19] = 0;
   out_751219126521953061[20] = 0;
   out_751219126521953061[21] = 0;
   out_751219126521953061[22] = 1;
   out_751219126521953061[23] = 0;
   out_751219126521953061[24] = 0;
   out_751219126521953061[25] = 0;
   out_751219126521953061[26] = 0;
   out_751219126521953061[27] = 0;
   out_751219126521953061[28] = 0;
   out_751219126521953061[29] = 0;
   out_751219126521953061[30] = 0;
   out_751219126521953061[31] = 0;
   out_751219126521953061[32] = 0;
   out_751219126521953061[33] = 0;
   out_751219126521953061[34] = 0;
   out_751219126521953061[35] = 0;
   out_751219126521953061[36] = 0;
   out_751219126521953061[37] = 0;
   out_751219126521953061[38] = 0;
   out_751219126521953061[39] = 0;
   out_751219126521953061[40] = 0;
   out_751219126521953061[41] = 1;
   out_751219126521953061[42] = 0;
   out_751219126521953061[43] = 0;
   out_751219126521953061[44] = 0;
   out_751219126521953061[45] = 0;
   out_751219126521953061[46] = 0;
   out_751219126521953061[47] = 0;
   out_751219126521953061[48] = 0;
   out_751219126521953061[49] = 0;
   out_751219126521953061[50] = 0;
   out_751219126521953061[51] = 0;
   out_751219126521953061[52] = 0;
   out_751219126521953061[53] = 0;
}
void h_14(double *state, double *unused, double *out_6937523326105035085) {
   out_6937523326105035085[0] = state[6];
   out_6937523326105035085[1] = state[7];
   out_6937523326105035085[2] = state[8];
}
void H_14(double *state, double *unused, double *out_252095514801333) {
   out_252095514801333[0] = 0;
   out_252095514801333[1] = 0;
   out_252095514801333[2] = 0;
   out_252095514801333[3] = 0;
   out_252095514801333[4] = 0;
   out_252095514801333[5] = 0;
   out_252095514801333[6] = 1;
   out_252095514801333[7] = 0;
   out_252095514801333[8] = 0;
   out_252095514801333[9] = 0;
   out_252095514801333[10] = 0;
   out_252095514801333[11] = 0;
   out_252095514801333[12] = 0;
   out_252095514801333[13] = 0;
   out_252095514801333[14] = 0;
   out_252095514801333[15] = 0;
   out_252095514801333[16] = 0;
   out_252095514801333[17] = 0;
   out_252095514801333[18] = 0;
   out_252095514801333[19] = 0;
   out_252095514801333[20] = 0;
   out_252095514801333[21] = 0;
   out_252095514801333[22] = 0;
   out_252095514801333[23] = 0;
   out_252095514801333[24] = 0;
   out_252095514801333[25] = 1;
   out_252095514801333[26] = 0;
   out_252095514801333[27] = 0;
   out_252095514801333[28] = 0;
   out_252095514801333[29] = 0;
   out_252095514801333[30] = 0;
   out_252095514801333[31] = 0;
   out_252095514801333[32] = 0;
   out_252095514801333[33] = 0;
   out_252095514801333[34] = 0;
   out_252095514801333[35] = 0;
   out_252095514801333[36] = 0;
   out_252095514801333[37] = 0;
   out_252095514801333[38] = 0;
   out_252095514801333[39] = 0;
   out_252095514801333[40] = 0;
   out_252095514801333[41] = 0;
   out_252095514801333[42] = 0;
   out_252095514801333[43] = 0;
   out_252095514801333[44] = 1;
   out_252095514801333[45] = 0;
   out_252095514801333[46] = 0;
   out_252095514801333[47] = 0;
   out_252095514801333[48] = 0;
   out_252095514801333[49] = 0;
   out_252095514801333[50] = 0;
   out_252095514801333[51] = 0;
   out_252095514801333[52] = 0;
   out_252095514801333[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_7449750869360596060) {
  err_fun(nom_x, delta_x, out_7449750869360596060);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9017488319834403806) {
  inv_err_fun(nom_x, true_x, out_9017488319834403806);
}
void pose_H_mod_fun(double *state, double *out_5161440433737183966) {
  H_mod_fun(state, out_5161440433737183966);
}
void pose_f_fun(double *state, double dt, double *out_4374988147753332159) {
  f_fun(state,  dt, out_4374988147753332159);
}
void pose_F_fun(double *state, double dt, double *out_4799445358667755900) {
  F_fun(state,  dt, out_4799445358667755900);
}
void pose_h_4(double *state, double *unused, double *out_5495389517486282336) {
  h_4(state, unused, out_5495389517486282336);
}
void pose_H_4(double *state, double *unused, double *out_3963492951854285862) {
  H_4(state, unused, out_3963492951854285862);
}
void pose_h_10(double *state, double *unused, double *out_3796979241642905601) {
  h_10(state, unused, out_3796979241642905601);
}
void pose_H_10(double *state, double *unused, double *out_2798195339350489049) {
  H_10(state, unused, out_2798195339350489049);
}
void pose_h_13(double *state, double *unused, double *out_2405306930266407283) {
  h_13(state, unused, out_2405306930266407283);
}
void pose_H_13(double *state, double *unused, double *out_751219126521953061) {
  H_13(state, unused, out_751219126521953061);
}
void pose_h_14(double *state, double *unused, double *out_6937523326105035085) {
  h_14(state, unused, out_6937523326105035085);
}
void pose_H_14(double *state, double *unused, double *out_252095514801333) {
  H_14(state, unused, out_252095514801333);
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
