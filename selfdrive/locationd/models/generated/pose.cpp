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
void err_fun(double *nom_x, double *delta_x, double *out_5204449900680294893) {
   out_5204449900680294893[0] = delta_x[0] + nom_x[0];
   out_5204449900680294893[1] = delta_x[1] + nom_x[1];
   out_5204449900680294893[2] = delta_x[2] + nom_x[2];
   out_5204449900680294893[3] = delta_x[3] + nom_x[3];
   out_5204449900680294893[4] = delta_x[4] + nom_x[4];
   out_5204449900680294893[5] = delta_x[5] + nom_x[5];
   out_5204449900680294893[6] = delta_x[6] + nom_x[6];
   out_5204449900680294893[7] = delta_x[7] + nom_x[7];
   out_5204449900680294893[8] = delta_x[8] + nom_x[8];
   out_5204449900680294893[9] = delta_x[9] + nom_x[9];
   out_5204449900680294893[10] = delta_x[10] + nom_x[10];
   out_5204449900680294893[11] = delta_x[11] + nom_x[11];
   out_5204449900680294893[12] = delta_x[12] + nom_x[12];
   out_5204449900680294893[13] = delta_x[13] + nom_x[13];
   out_5204449900680294893[14] = delta_x[14] + nom_x[14];
   out_5204449900680294893[15] = delta_x[15] + nom_x[15];
   out_5204449900680294893[16] = delta_x[16] + nom_x[16];
   out_5204449900680294893[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3166247841107463599) {
   out_3166247841107463599[0] = -nom_x[0] + true_x[0];
   out_3166247841107463599[1] = -nom_x[1] + true_x[1];
   out_3166247841107463599[2] = -nom_x[2] + true_x[2];
   out_3166247841107463599[3] = -nom_x[3] + true_x[3];
   out_3166247841107463599[4] = -nom_x[4] + true_x[4];
   out_3166247841107463599[5] = -nom_x[5] + true_x[5];
   out_3166247841107463599[6] = -nom_x[6] + true_x[6];
   out_3166247841107463599[7] = -nom_x[7] + true_x[7];
   out_3166247841107463599[8] = -nom_x[8] + true_x[8];
   out_3166247841107463599[9] = -nom_x[9] + true_x[9];
   out_3166247841107463599[10] = -nom_x[10] + true_x[10];
   out_3166247841107463599[11] = -nom_x[11] + true_x[11];
   out_3166247841107463599[12] = -nom_x[12] + true_x[12];
   out_3166247841107463599[13] = -nom_x[13] + true_x[13];
   out_3166247841107463599[14] = -nom_x[14] + true_x[14];
   out_3166247841107463599[15] = -nom_x[15] + true_x[15];
   out_3166247841107463599[16] = -nom_x[16] + true_x[16];
   out_3166247841107463599[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4757463840714288630) {
   out_4757463840714288630[0] = 1.0;
   out_4757463840714288630[1] = 0.0;
   out_4757463840714288630[2] = 0.0;
   out_4757463840714288630[3] = 0.0;
   out_4757463840714288630[4] = 0.0;
   out_4757463840714288630[5] = 0.0;
   out_4757463840714288630[6] = 0.0;
   out_4757463840714288630[7] = 0.0;
   out_4757463840714288630[8] = 0.0;
   out_4757463840714288630[9] = 0.0;
   out_4757463840714288630[10] = 0.0;
   out_4757463840714288630[11] = 0.0;
   out_4757463840714288630[12] = 0.0;
   out_4757463840714288630[13] = 0.0;
   out_4757463840714288630[14] = 0.0;
   out_4757463840714288630[15] = 0.0;
   out_4757463840714288630[16] = 0.0;
   out_4757463840714288630[17] = 0.0;
   out_4757463840714288630[18] = 0.0;
   out_4757463840714288630[19] = 1.0;
   out_4757463840714288630[20] = 0.0;
   out_4757463840714288630[21] = 0.0;
   out_4757463840714288630[22] = 0.0;
   out_4757463840714288630[23] = 0.0;
   out_4757463840714288630[24] = 0.0;
   out_4757463840714288630[25] = 0.0;
   out_4757463840714288630[26] = 0.0;
   out_4757463840714288630[27] = 0.0;
   out_4757463840714288630[28] = 0.0;
   out_4757463840714288630[29] = 0.0;
   out_4757463840714288630[30] = 0.0;
   out_4757463840714288630[31] = 0.0;
   out_4757463840714288630[32] = 0.0;
   out_4757463840714288630[33] = 0.0;
   out_4757463840714288630[34] = 0.0;
   out_4757463840714288630[35] = 0.0;
   out_4757463840714288630[36] = 0.0;
   out_4757463840714288630[37] = 0.0;
   out_4757463840714288630[38] = 1.0;
   out_4757463840714288630[39] = 0.0;
   out_4757463840714288630[40] = 0.0;
   out_4757463840714288630[41] = 0.0;
   out_4757463840714288630[42] = 0.0;
   out_4757463840714288630[43] = 0.0;
   out_4757463840714288630[44] = 0.0;
   out_4757463840714288630[45] = 0.0;
   out_4757463840714288630[46] = 0.0;
   out_4757463840714288630[47] = 0.0;
   out_4757463840714288630[48] = 0.0;
   out_4757463840714288630[49] = 0.0;
   out_4757463840714288630[50] = 0.0;
   out_4757463840714288630[51] = 0.0;
   out_4757463840714288630[52] = 0.0;
   out_4757463840714288630[53] = 0.0;
   out_4757463840714288630[54] = 0.0;
   out_4757463840714288630[55] = 0.0;
   out_4757463840714288630[56] = 0.0;
   out_4757463840714288630[57] = 1.0;
   out_4757463840714288630[58] = 0.0;
   out_4757463840714288630[59] = 0.0;
   out_4757463840714288630[60] = 0.0;
   out_4757463840714288630[61] = 0.0;
   out_4757463840714288630[62] = 0.0;
   out_4757463840714288630[63] = 0.0;
   out_4757463840714288630[64] = 0.0;
   out_4757463840714288630[65] = 0.0;
   out_4757463840714288630[66] = 0.0;
   out_4757463840714288630[67] = 0.0;
   out_4757463840714288630[68] = 0.0;
   out_4757463840714288630[69] = 0.0;
   out_4757463840714288630[70] = 0.0;
   out_4757463840714288630[71] = 0.0;
   out_4757463840714288630[72] = 0.0;
   out_4757463840714288630[73] = 0.0;
   out_4757463840714288630[74] = 0.0;
   out_4757463840714288630[75] = 0.0;
   out_4757463840714288630[76] = 1.0;
   out_4757463840714288630[77] = 0.0;
   out_4757463840714288630[78] = 0.0;
   out_4757463840714288630[79] = 0.0;
   out_4757463840714288630[80] = 0.0;
   out_4757463840714288630[81] = 0.0;
   out_4757463840714288630[82] = 0.0;
   out_4757463840714288630[83] = 0.0;
   out_4757463840714288630[84] = 0.0;
   out_4757463840714288630[85] = 0.0;
   out_4757463840714288630[86] = 0.0;
   out_4757463840714288630[87] = 0.0;
   out_4757463840714288630[88] = 0.0;
   out_4757463840714288630[89] = 0.0;
   out_4757463840714288630[90] = 0.0;
   out_4757463840714288630[91] = 0.0;
   out_4757463840714288630[92] = 0.0;
   out_4757463840714288630[93] = 0.0;
   out_4757463840714288630[94] = 0.0;
   out_4757463840714288630[95] = 1.0;
   out_4757463840714288630[96] = 0.0;
   out_4757463840714288630[97] = 0.0;
   out_4757463840714288630[98] = 0.0;
   out_4757463840714288630[99] = 0.0;
   out_4757463840714288630[100] = 0.0;
   out_4757463840714288630[101] = 0.0;
   out_4757463840714288630[102] = 0.0;
   out_4757463840714288630[103] = 0.0;
   out_4757463840714288630[104] = 0.0;
   out_4757463840714288630[105] = 0.0;
   out_4757463840714288630[106] = 0.0;
   out_4757463840714288630[107] = 0.0;
   out_4757463840714288630[108] = 0.0;
   out_4757463840714288630[109] = 0.0;
   out_4757463840714288630[110] = 0.0;
   out_4757463840714288630[111] = 0.0;
   out_4757463840714288630[112] = 0.0;
   out_4757463840714288630[113] = 0.0;
   out_4757463840714288630[114] = 1.0;
   out_4757463840714288630[115] = 0.0;
   out_4757463840714288630[116] = 0.0;
   out_4757463840714288630[117] = 0.0;
   out_4757463840714288630[118] = 0.0;
   out_4757463840714288630[119] = 0.0;
   out_4757463840714288630[120] = 0.0;
   out_4757463840714288630[121] = 0.0;
   out_4757463840714288630[122] = 0.0;
   out_4757463840714288630[123] = 0.0;
   out_4757463840714288630[124] = 0.0;
   out_4757463840714288630[125] = 0.0;
   out_4757463840714288630[126] = 0.0;
   out_4757463840714288630[127] = 0.0;
   out_4757463840714288630[128] = 0.0;
   out_4757463840714288630[129] = 0.0;
   out_4757463840714288630[130] = 0.0;
   out_4757463840714288630[131] = 0.0;
   out_4757463840714288630[132] = 0.0;
   out_4757463840714288630[133] = 1.0;
   out_4757463840714288630[134] = 0.0;
   out_4757463840714288630[135] = 0.0;
   out_4757463840714288630[136] = 0.0;
   out_4757463840714288630[137] = 0.0;
   out_4757463840714288630[138] = 0.0;
   out_4757463840714288630[139] = 0.0;
   out_4757463840714288630[140] = 0.0;
   out_4757463840714288630[141] = 0.0;
   out_4757463840714288630[142] = 0.0;
   out_4757463840714288630[143] = 0.0;
   out_4757463840714288630[144] = 0.0;
   out_4757463840714288630[145] = 0.0;
   out_4757463840714288630[146] = 0.0;
   out_4757463840714288630[147] = 0.0;
   out_4757463840714288630[148] = 0.0;
   out_4757463840714288630[149] = 0.0;
   out_4757463840714288630[150] = 0.0;
   out_4757463840714288630[151] = 0.0;
   out_4757463840714288630[152] = 1.0;
   out_4757463840714288630[153] = 0.0;
   out_4757463840714288630[154] = 0.0;
   out_4757463840714288630[155] = 0.0;
   out_4757463840714288630[156] = 0.0;
   out_4757463840714288630[157] = 0.0;
   out_4757463840714288630[158] = 0.0;
   out_4757463840714288630[159] = 0.0;
   out_4757463840714288630[160] = 0.0;
   out_4757463840714288630[161] = 0.0;
   out_4757463840714288630[162] = 0.0;
   out_4757463840714288630[163] = 0.0;
   out_4757463840714288630[164] = 0.0;
   out_4757463840714288630[165] = 0.0;
   out_4757463840714288630[166] = 0.0;
   out_4757463840714288630[167] = 0.0;
   out_4757463840714288630[168] = 0.0;
   out_4757463840714288630[169] = 0.0;
   out_4757463840714288630[170] = 0.0;
   out_4757463840714288630[171] = 1.0;
   out_4757463840714288630[172] = 0.0;
   out_4757463840714288630[173] = 0.0;
   out_4757463840714288630[174] = 0.0;
   out_4757463840714288630[175] = 0.0;
   out_4757463840714288630[176] = 0.0;
   out_4757463840714288630[177] = 0.0;
   out_4757463840714288630[178] = 0.0;
   out_4757463840714288630[179] = 0.0;
   out_4757463840714288630[180] = 0.0;
   out_4757463840714288630[181] = 0.0;
   out_4757463840714288630[182] = 0.0;
   out_4757463840714288630[183] = 0.0;
   out_4757463840714288630[184] = 0.0;
   out_4757463840714288630[185] = 0.0;
   out_4757463840714288630[186] = 0.0;
   out_4757463840714288630[187] = 0.0;
   out_4757463840714288630[188] = 0.0;
   out_4757463840714288630[189] = 0.0;
   out_4757463840714288630[190] = 1.0;
   out_4757463840714288630[191] = 0.0;
   out_4757463840714288630[192] = 0.0;
   out_4757463840714288630[193] = 0.0;
   out_4757463840714288630[194] = 0.0;
   out_4757463840714288630[195] = 0.0;
   out_4757463840714288630[196] = 0.0;
   out_4757463840714288630[197] = 0.0;
   out_4757463840714288630[198] = 0.0;
   out_4757463840714288630[199] = 0.0;
   out_4757463840714288630[200] = 0.0;
   out_4757463840714288630[201] = 0.0;
   out_4757463840714288630[202] = 0.0;
   out_4757463840714288630[203] = 0.0;
   out_4757463840714288630[204] = 0.0;
   out_4757463840714288630[205] = 0.0;
   out_4757463840714288630[206] = 0.0;
   out_4757463840714288630[207] = 0.0;
   out_4757463840714288630[208] = 0.0;
   out_4757463840714288630[209] = 1.0;
   out_4757463840714288630[210] = 0.0;
   out_4757463840714288630[211] = 0.0;
   out_4757463840714288630[212] = 0.0;
   out_4757463840714288630[213] = 0.0;
   out_4757463840714288630[214] = 0.0;
   out_4757463840714288630[215] = 0.0;
   out_4757463840714288630[216] = 0.0;
   out_4757463840714288630[217] = 0.0;
   out_4757463840714288630[218] = 0.0;
   out_4757463840714288630[219] = 0.0;
   out_4757463840714288630[220] = 0.0;
   out_4757463840714288630[221] = 0.0;
   out_4757463840714288630[222] = 0.0;
   out_4757463840714288630[223] = 0.0;
   out_4757463840714288630[224] = 0.0;
   out_4757463840714288630[225] = 0.0;
   out_4757463840714288630[226] = 0.0;
   out_4757463840714288630[227] = 0.0;
   out_4757463840714288630[228] = 1.0;
   out_4757463840714288630[229] = 0.0;
   out_4757463840714288630[230] = 0.0;
   out_4757463840714288630[231] = 0.0;
   out_4757463840714288630[232] = 0.0;
   out_4757463840714288630[233] = 0.0;
   out_4757463840714288630[234] = 0.0;
   out_4757463840714288630[235] = 0.0;
   out_4757463840714288630[236] = 0.0;
   out_4757463840714288630[237] = 0.0;
   out_4757463840714288630[238] = 0.0;
   out_4757463840714288630[239] = 0.0;
   out_4757463840714288630[240] = 0.0;
   out_4757463840714288630[241] = 0.0;
   out_4757463840714288630[242] = 0.0;
   out_4757463840714288630[243] = 0.0;
   out_4757463840714288630[244] = 0.0;
   out_4757463840714288630[245] = 0.0;
   out_4757463840714288630[246] = 0.0;
   out_4757463840714288630[247] = 1.0;
   out_4757463840714288630[248] = 0.0;
   out_4757463840714288630[249] = 0.0;
   out_4757463840714288630[250] = 0.0;
   out_4757463840714288630[251] = 0.0;
   out_4757463840714288630[252] = 0.0;
   out_4757463840714288630[253] = 0.0;
   out_4757463840714288630[254] = 0.0;
   out_4757463840714288630[255] = 0.0;
   out_4757463840714288630[256] = 0.0;
   out_4757463840714288630[257] = 0.0;
   out_4757463840714288630[258] = 0.0;
   out_4757463840714288630[259] = 0.0;
   out_4757463840714288630[260] = 0.0;
   out_4757463840714288630[261] = 0.0;
   out_4757463840714288630[262] = 0.0;
   out_4757463840714288630[263] = 0.0;
   out_4757463840714288630[264] = 0.0;
   out_4757463840714288630[265] = 0.0;
   out_4757463840714288630[266] = 1.0;
   out_4757463840714288630[267] = 0.0;
   out_4757463840714288630[268] = 0.0;
   out_4757463840714288630[269] = 0.0;
   out_4757463840714288630[270] = 0.0;
   out_4757463840714288630[271] = 0.0;
   out_4757463840714288630[272] = 0.0;
   out_4757463840714288630[273] = 0.0;
   out_4757463840714288630[274] = 0.0;
   out_4757463840714288630[275] = 0.0;
   out_4757463840714288630[276] = 0.0;
   out_4757463840714288630[277] = 0.0;
   out_4757463840714288630[278] = 0.0;
   out_4757463840714288630[279] = 0.0;
   out_4757463840714288630[280] = 0.0;
   out_4757463840714288630[281] = 0.0;
   out_4757463840714288630[282] = 0.0;
   out_4757463840714288630[283] = 0.0;
   out_4757463840714288630[284] = 0.0;
   out_4757463840714288630[285] = 1.0;
   out_4757463840714288630[286] = 0.0;
   out_4757463840714288630[287] = 0.0;
   out_4757463840714288630[288] = 0.0;
   out_4757463840714288630[289] = 0.0;
   out_4757463840714288630[290] = 0.0;
   out_4757463840714288630[291] = 0.0;
   out_4757463840714288630[292] = 0.0;
   out_4757463840714288630[293] = 0.0;
   out_4757463840714288630[294] = 0.0;
   out_4757463840714288630[295] = 0.0;
   out_4757463840714288630[296] = 0.0;
   out_4757463840714288630[297] = 0.0;
   out_4757463840714288630[298] = 0.0;
   out_4757463840714288630[299] = 0.0;
   out_4757463840714288630[300] = 0.0;
   out_4757463840714288630[301] = 0.0;
   out_4757463840714288630[302] = 0.0;
   out_4757463840714288630[303] = 0.0;
   out_4757463840714288630[304] = 1.0;
   out_4757463840714288630[305] = 0.0;
   out_4757463840714288630[306] = 0.0;
   out_4757463840714288630[307] = 0.0;
   out_4757463840714288630[308] = 0.0;
   out_4757463840714288630[309] = 0.0;
   out_4757463840714288630[310] = 0.0;
   out_4757463840714288630[311] = 0.0;
   out_4757463840714288630[312] = 0.0;
   out_4757463840714288630[313] = 0.0;
   out_4757463840714288630[314] = 0.0;
   out_4757463840714288630[315] = 0.0;
   out_4757463840714288630[316] = 0.0;
   out_4757463840714288630[317] = 0.0;
   out_4757463840714288630[318] = 0.0;
   out_4757463840714288630[319] = 0.0;
   out_4757463840714288630[320] = 0.0;
   out_4757463840714288630[321] = 0.0;
   out_4757463840714288630[322] = 0.0;
   out_4757463840714288630[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_1433841035840211) {
   out_1433841035840211[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_1433841035840211[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_1433841035840211[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_1433841035840211[3] = dt*state[12] + state[3];
   out_1433841035840211[4] = dt*state[13] + state[4];
   out_1433841035840211[5] = dt*state[14] + state[5];
   out_1433841035840211[6] = state[6];
   out_1433841035840211[7] = state[7];
   out_1433841035840211[8] = state[8];
   out_1433841035840211[9] = state[9];
   out_1433841035840211[10] = state[10];
   out_1433841035840211[11] = state[11];
   out_1433841035840211[12] = state[12];
   out_1433841035840211[13] = state[13];
   out_1433841035840211[14] = state[14];
   out_1433841035840211[15] = state[15];
   out_1433841035840211[16] = state[16];
   out_1433841035840211[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3040199541343879647) {
   out_3040199541343879647[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3040199541343879647[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3040199541343879647[2] = 0;
   out_3040199541343879647[3] = 0;
   out_3040199541343879647[4] = 0;
   out_3040199541343879647[5] = 0;
   out_3040199541343879647[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3040199541343879647[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3040199541343879647[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3040199541343879647[9] = 0;
   out_3040199541343879647[10] = 0;
   out_3040199541343879647[11] = 0;
   out_3040199541343879647[12] = 0;
   out_3040199541343879647[13] = 0;
   out_3040199541343879647[14] = 0;
   out_3040199541343879647[15] = 0;
   out_3040199541343879647[16] = 0;
   out_3040199541343879647[17] = 0;
   out_3040199541343879647[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3040199541343879647[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3040199541343879647[20] = 0;
   out_3040199541343879647[21] = 0;
   out_3040199541343879647[22] = 0;
   out_3040199541343879647[23] = 0;
   out_3040199541343879647[24] = 0;
   out_3040199541343879647[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3040199541343879647[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3040199541343879647[27] = 0;
   out_3040199541343879647[28] = 0;
   out_3040199541343879647[29] = 0;
   out_3040199541343879647[30] = 0;
   out_3040199541343879647[31] = 0;
   out_3040199541343879647[32] = 0;
   out_3040199541343879647[33] = 0;
   out_3040199541343879647[34] = 0;
   out_3040199541343879647[35] = 0;
   out_3040199541343879647[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3040199541343879647[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3040199541343879647[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3040199541343879647[39] = 0;
   out_3040199541343879647[40] = 0;
   out_3040199541343879647[41] = 0;
   out_3040199541343879647[42] = 0;
   out_3040199541343879647[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3040199541343879647[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3040199541343879647[45] = 0;
   out_3040199541343879647[46] = 0;
   out_3040199541343879647[47] = 0;
   out_3040199541343879647[48] = 0;
   out_3040199541343879647[49] = 0;
   out_3040199541343879647[50] = 0;
   out_3040199541343879647[51] = 0;
   out_3040199541343879647[52] = 0;
   out_3040199541343879647[53] = 0;
   out_3040199541343879647[54] = 0;
   out_3040199541343879647[55] = 0;
   out_3040199541343879647[56] = 0;
   out_3040199541343879647[57] = 1;
   out_3040199541343879647[58] = 0;
   out_3040199541343879647[59] = 0;
   out_3040199541343879647[60] = 0;
   out_3040199541343879647[61] = 0;
   out_3040199541343879647[62] = 0;
   out_3040199541343879647[63] = 0;
   out_3040199541343879647[64] = 0;
   out_3040199541343879647[65] = 0;
   out_3040199541343879647[66] = dt;
   out_3040199541343879647[67] = 0;
   out_3040199541343879647[68] = 0;
   out_3040199541343879647[69] = 0;
   out_3040199541343879647[70] = 0;
   out_3040199541343879647[71] = 0;
   out_3040199541343879647[72] = 0;
   out_3040199541343879647[73] = 0;
   out_3040199541343879647[74] = 0;
   out_3040199541343879647[75] = 0;
   out_3040199541343879647[76] = 1;
   out_3040199541343879647[77] = 0;
   out_3040199541343879647[78] = 0;
   out_3040199541343879647[79] = 0;
   out_3040199541343879647[80] = 0;
   out_3040199541343879647[81] = 0;
   out_3040199541343879647[82] = 0;
   out_3040199541343879647[83] = 0;
   out_3040199541343879647[84] = 0;
   out_3040199541343879647[85] = dt;
   out_3040199541343879647[86] = 0;
   out_3040199541343879647[87] = 0;
   out_3040199541343879647[88] = 0;
   out_3040199541343879647[89] = 0;
   out_3040199541343879647[90] = 0;
   out_3040199541343879647[91] = 0;
   out_3040199541343879647[92] = 0;
   out_3040199541343879647[93] = 0;
   out_3040199541343879647[94] = 0;
   out_3040199541343879647[95] = 1;
   out_3040199541343879647[96] = 0;
   out_3040199541343879647[97] = 0;
   out_3040199541343879647[98] = 0;
   out_3040199541343879647[99] = 0;
   out_3040199541343879647[100] = 0;
   out_3040199541343879647[101] = 0;
   out_3040199541343879647[102] = 0;
   out_3040199541343879647[103] = 0;
   out_3040199541343879647[104] = dt;
   out_3040199541343879647[105] = 0;
   out_3040199541343879647[106] = 0;
   out_3040199541343879647[107] = 0;
   out_3040199541343879647[108] = 0;
   out_3040199541343879647[109] = 0;
   out_3040199541343879647[110] = 0;
   out_3040199541343879647[111] = 0;
   out_3040199541343879647[112] = 0;
   out_3040199541343879647[113] = 0;
   out_3040199541343879647[114] = 1;
   out_3040199541343879647[115] = 0;
   out_3040199541343879647[116] = 0;
   out_3040199541343879647[117] = 0;
   out_3040199541343879647[118] = 0;
   out_3040199541343879647[119] = 0;
   out_3040199541343879647[120] = 0;
   out_3040199541343879647[121] = 0;
   out_3040199541343879647[122] = 0;
   out_3040199541343879647[123] = 0;
   out_3040199541343879647[124] = 0;
   out_3040199541343879647[125] = 0;
   out_3040199541343879647[126] = 0;
   out_3040199541343879647[127] = 0;
   out_3040199541343879647[128] = 0;
   out_3040199541343879647[129] = 0;
   out_3040199541343879647[130] = 0;
   out_3040199541343879647[131] = 0;
   out_3040199541343879647[132] = 0;
   out_3040199541343879647[133] = 1;
   out_3040199541343879647[134] = 0;
   out_3040199541343879647[135] = 0;
   out_3040199541343879647[136] = 0;
   out_3040199541343879647[137] = 0;
   out_3040199541343879647[138] = 0;
   out_3040199541343879647[139] = 0;
   out_3040199541343879647[140] = 0;
   out_3040199541343879647[141] = 0;
   out_3040199541343879647[142] = 0;
   out_3040199541343879647[143] = 0;
   out_3040199541343879647[144] = 0;
   out_3040199541343879647[145] = 0;
   out_3040199541343879647[146] = 0;
   out_3040199541343879647[147] = 0;
   out_3040199541343879647[148] = 0;
   out_3040199541343879647[149] = 0;
   out_3040199541343879647[150] = 0;
   out_3040199541343879647[151] = 0;
   out_3040199541343879647[152] = 1;
   out_3040199541343879647[153] = 0;
   out_3040199541343879647[154] = 0;
   out_3040199541343879647[155] = 0;
   out_3040199541343879647[156] = 0;
   out_3040199541343879647[157] = 0;
   out_3040199541343879647[158] = 0;
   out_3040199541343879647[159] = 0;
   out_3040199541343879647[160] = 0;
   out_3040199541343879647[161] = 0;
   out_3040199541343879647[162] = 0;
   out_3040199541343879647[163] = 0;
   out_3040199541343879647[164] = 0;
   out_3040199541343879647[165] = 0;
   out_3040199541343879647[166] = 0;
   out_3040199541343879647[167] = 0;
   out_3040199541343879647[168] = 0;
   out_3040199541343879647[169] = 0;
   out_3040199541343879647[170] = 0;
   out_3040199541343879647[171] = 1;
   out_3040199541343879647[172] = 0;
   out_3040199541343879647[173] = 0;
   out_3040199541343879647[174] = 0;
   out_3040199541343879647[175] = 0;
   out_3040199541343879647[176] = 0;
   out_3040199541343879647[177] = 0;
   out_3040199541343879647[178] = 0;
   out_3040199541343879647[179] = 0;
   out_3040199541343879647[180] = 0;
   out_3040199541343879647[181] = 0;
   out_3040199541343879647[182] = 0;
   out_3040199541343879647[183] = 0;
   out_3040199541343879647[184] = 0;
   out_3040199541343879647[185] = 0;
   out_3040199541343879647[186] = 0;
   out_3040199541343879647[187] = 0;
   out_3040199541343879647[188] = 0;
   out_3040199541343879647[189] = 0;
   out_3040199541343879647[190] = 1;
   out_3040199541343879647[191] = 0;
   out_3040199541343879647[192] = 0;
   out_3040199541343879647[193] = 0;
   out_3040199541343879647[194] = 0;
   out_3040199541343879647[195] = 0;
   out_3040199541343879647[196] = 0;
   out_3040199541343879647[197] = 0;
   out_3040199541343879647[198] = 0;
   out_3040199541343879647[199] = 0;
   out_3040199541343879647[200] = 0;
   out_3040199541343879647[201] = 0;
   out_3040199541343879647[202] = 0;
   out_3040199541343879647[203] = 0;
   out_3040199541343879647[204] = 0;
   out_3040199541343879647[205] = 0;
   out_3040199541343879647[206] = 0;
   out_3040199541343879647[207] = 0;
   out_3040199541343879647[208] = 0;
   out_3040199541343879647[209] = 1;
   out_3040199541343879647[210] = 0;
   out_3040199541343879647[211] = 0;
   out_3040199541343879647[212] = 0;
   out_3040199541343879647[213] = 0;
   out_3040199541343879647[214] = 0;
   out_3040199541343879647[215] = 0;
   out_3040199541343879647[216] = 0;
   out_3040199541343879647[217] = 0;
   out_3040199541343879647[218] = 0;
   out_3040199541343879647[219] = 0;
   out_3040199541343879647[220] = 0;
   out_3040199541343879647[221] = 0;
   out_3040199541343879647[222] = 0;
   out_3040199541343879647[223] = 0;
   out_3040199541343879647[224] = 0;
   out_3040199541343879647[225] = 0;
   out_3040199541343879647[226] = 0;
   out_3040199541343879647[227] = 0;
   out_3040199541343879647[228] = 1;
   out_3040199541343879647[229] = 0;
   out_3040199541343879647[230] = 0;
   out_3040199541343879647[231] = 0;
   out_3040199541343879647[232] = 0;
   out_3040199541343879647[233] = 0;
   out_3040199541343879647[234] = 0;
   out_3040199541343879647[235] = 0;
   out_3040199541343879647[236] = 0;
   out_3040199541343879647[237] = 0;
   out_3040199541343879647[238] = 0;
   out_3040199541343879647[239] = 0;
   out_3040199541343879647[240] = 0;
   out_3040199541343879647[241] = 0;
   out_3040199541343879647[242] = 0;
   out_3040199541343879647[243] = 0;
   out_3040199541343879647[244] = 0;
   out_3040199541343879647[245] = 0;
   out_3040199541343879647[246] = 0;
   out_3040199541343879647[247] = 1;
   out_3040199541343879647[248] = 0;
   out_3040199541343879647[249] = 0;
   out_3040199541343879647[250] = 0;
   out_3040199541343879647[251] = 0;
   out_3040199541343879647[252] = 0;
   out_3040199541343879647[253] = 0;
   out_3040199541343879647[254] = 0;
   out_3040199541343879647[255] = 0;
   out_3040199541343879647[256] = 0;
   out_3040199541343879647[257] = 0;
   out_3040199541343879647[258] = 0;
   out_3040199541343879647[259] = 0;
   out_3040199541343879647[260] = 0;
   out_3040199541343879647[261] = 0;
   out_3040199541343879647[262] = 0;
   out_3040199541343879647[263] = 0;
   out_3040199541343879647[264] = 0;
   out_3040199541343879647[265] = 0;
   out_3040199541343879647[266] = 1;
   out_3040199541343879647[267] = 0;
   out_3040199541343879647[268] = 0;
   out_3040199541343879647[269] = 0;
   out_3040199541343879647[270] = 0;
   out_3040199541343879647[271] = 0;
   out_3040199541343879647[272] = 0;
   out_3040199541343879647[273] = 0;
   out_3040199541343879647[274] = 0;
   out_3040199541343879647[275] = 0;
   out_3040199541343879647[276] = 0;
   out_3040199541343879647[277] = 0;
   out_3040199541343879647[278] = 0;
   out_3040199541343879647[279] = 0;
   out_3040199541343879647[280] = 0;
   out_3040199541343879647[281] = 0;
   out_3040199541343879647[282] = 0;
   out_3040199541343879647[283] = 0;
   out_3040199541343879647[284] = 0;
   out_3040199541343879647[285] = 1;
   out_3040199541343879647[286] = 0;
   out_3040199541343879647[287] = 0;
   out_3040199541343879647[288] = 0;
   out_3040199541343879647[289] = 0;
   out_3040199541343879647[290] = 0;
   out_3040199541343879647[291] = 0;
   out_3040199541343879647[292] = 0;
   out_3040199541343879647[293] = 0;
   out_3040199541343879647[294] = 0;
   out_3040199541343879647[295] = 0;
   out_3040199541343879647[296] = 0;
   out_3040199541343879647[297] = 0;
   out_3040199541343879647[298] = 0;
   out_3040199541343879647[299] = 0;
   out_3040199541343879647[300] = 0;
   out_3040199541343879647[301] = 0;
   out_3040199541343879647[302] = 0;
   out_3040199541343879647[303] = 0;
   out_3040199541343879647[304] = 1;
   out_3040199541343879647[305] = 0;
   out_3040199541343879647[306] = 0;
   out_3040199541343879647[307] = 0;
   out_3040199541343879647[308] = 0;
   out_3040199541343879647[309] = 0;
   out_3040199541343879647[310] = 0;
   out_3040199541343879647[311] = 0;
   out_3040199541343879647[312] = 0;
   out_3040199541343879647[313] = 0;
   out_3040199541343879647[314] = 0;
   out_3040199541343879647[315] = 0;
   out_3040199541343879647[316] = 0;
   out_3040199541343879647[317] = 0;
   out_3040199541343879647[318] = 0;
   out_3040199541343879647[319] = 0;
   out_3040199541343879647[320] = 0;
   out_3040199541343879647[321] = 0;
   out_3040199541343879647[322] = 0;
   out_3040199541343879647[323] = 1;
}
void h_4(double *state, double *unused, double *out_942367839052342030) {
   out_942367839052342030[0] = state[6] + state[9];
   out_942367839052342030[1] = state[7] + state[10];
   out_942367839052342030[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_4003302838659576023) {
   out_4003302838659576023[0] = 0;
   out_4003302838659576023[1] = 0;
   out_4003302838659576023[2] = 0;
   out_4003302838659576023[3] = 0;
   out_4003302838659576023[4] = 0;
   out_4003302838659576023[5] = 0;
   out_4003302838659576023[6] = 1;
   out_4003302838659576023[7] = 0;
   out_4003302838659576023[8] = 0;
   out_4003302838659576023[9] = 1;
   out_4003302838659576023[10] = 0;
   out_4003302838659576023[11] = 0;
   out_4003302838659576023[12] = 0;
   out_4003302838659576023[13] = 0;
   out_4003302838659576023[14] = 0;
   out_4003302838659576023[15] = 0;
   out_4003302838659576023[16] = 0;
   out_4003302838659576023[17] = 0;
   out_4003302838659576023[18] = 0;
   out_4003302838659576023[19] = 0;
   out_4003302838659576023[20] = 0;
   out_4003302838659576023[21] = 0;
   out_4003302838659576023[22] = 0;
   out_4003302838659576023[23] = 0;
   out_4003302838659576023[24] = 0;
   out_4003302838659576023[25] = 1;
   out_4003302838659576023[26] = 0;
   out_4003302838659576023[27] = 0;
   out_4003302838659576023[28] = 1;
   out_4003302838659576023[29] = 0;
   out_4003302838659576023[30] = 0;
   out_4003302838659576023[31] = 0;
   out_4003302838659576023[32] = 0;
   out_4003302838659576023[33] = 0;
   out_4003302838659576023[34] = 0;
   out_4003302838659576023[35] = 0;
   out_4003302838659576023[36] = 0;
   out_4003302838659576023[37] = 0;
   out_4003302838659576023[38] = 0;
   out_4003302838659576023[39] = 0;
   out_4003302838659576023[40] = 0;
   out_4003302838659576023[41] = 0;
   out_4003302838659576023[42] = 0;
   out_4003302838659576023[43] = 0;
   out_4003302838659576023[44] = 1;
   out_4003302838659576023[45] = 0;
   out_4003302838659576023[46] = 0;
   out_4003302838659576023[47] = 1;
   out_4003302838659576023[48] = 0;
   out_4003302838659576023[49] = 0;
   out_4003302838659576023[50] = 0;
   out_4003302838659576023[51] = 0;
   out_4003302838659576023[52] = 0;
   out_4003302838659576023[53] = 0;
}
void h_10(double *state, double *unused, double *out_8571903349869224763) {
   out_8571903349869224763[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8571903349869224763[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8571903349869224763[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_3736312142713721773) {
   out_3736312142713721773[0] = 0;
   out_3736312142713721773[1] = 9.8100000000000005*cos(state[1]);
   out_3736312142713721773[2] = 0;
   out_3736312142713721773[3] = 0;
   out_3736312142713721773[4] = -state[8];
   out_3736312142713721773[5] = state[7];
   out_3736312142713721773[6] = 0;
   out_3736312142713721773[7] = state[5];
   out_3736312142713721773[8] = -state[4];
   out_3736312142713721773[9] = 0;
   out_3736312142713721773[10] = 0;
   out_3736312142713721773[11] = 0;
   out_3736312142713721773[12] = 1;
   out_3736312142713721773[13] = 0;
   out_3736312142713721773[14] = 0;
   out_3736312142713721773[15] = 1;
   out_3736312142713721773[16] = 0;
   out_3736312142713721773[17] = 0;
   out_3736312142713721773[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_3736312142713721773[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_3736312142713721773[20] = 0;
   out_3736312142713721773[21] = state[8];
   out_3736312142713721773[22] = 0;
   out_3736312142713721773[23] = -state[6];
   out_3736312142713721773[24] = -state[5];
   out_3736312142713721773[25] = 0;
   out_3736312142713721773[26] = state[3];
   out_3736312142713721773[27] = 0;
   out_3736312142713721773[28] = 0;
   out_3736312142713721773[29] = 0;
   out_3736312142713721773[30] = 0;
   out_3736312142713721773[31] = 1;
   out_3736312142713721773[32] = 0;
   out_3736312142713721773[33] = 0;
   out_3736312142713721773[34] = 1;
   out_3736312142713721773[35] = 0;
   out_3736312142713721773[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_3736312142713721773[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_3736312142713721773[38] = 0;
   out_3736312142713721773[39] = -state[7];
   out_3736312142713721773[40] = state[6];
   out_3736312142713721773[41] = 0;
   out_3736312142713721773[42] = state[4];
   out_3736312142713721773[43] = -state[3];
   out_3736312142713721773[44] = 0;
   out_3736312142713721773[45] = 0;
   out_3736312142713721773[46] = 0;
   out_3736312142713721773[47] = 0;
   out_3736312142713721773[48] = 0;
   out_3736312142713721773[49] = 0;
   out_3736312142713721773[50] = 1;
   out_3736312142713721773[51] = 0;
   out_3736312142713721773[52] = 0;
   out_3736312142713721773[53] = 1;
}
void h_13(double *state, double *unused, double *out_2382082840634836033) {
   out_2382082840634836033[0] = state[3];
   out_2382082840634836033[1] = state[4];
   out_2382082840634836033[2] = state[5];
}
void H_13(double *state, double *unused, double *out_3607328369657124906) {
   out_3607328369657124906[0] = 0;
   out_3607328369657124906[1] = 0;
   out_3607328369657124906[2] = 0;
   out_3607328369657124906[3] = 1;
   out_3607328369657124906[4] = 0;
   out_3607328369657124906[5] = 0;
   out_3607328369657124906[6] = 0;
   out_3607328369657124906[7] = 0;
   out_3607328369657124906[8] = 0;
   out_3607328369657124906[9] = 0;
   out_3607328369657124906[10] = 0;
   out_3607328369657124906[11] = 0;
   out_3607328369657124906[12] = 0;
   out_3607328369657124906[13] = 0;
   out_3607328369657124906[14] = 0;
   out_3607328369657124906[15] = 0;
   out_3607328369657124906[16] = 0;
   out_3607328369657124906[17] = 0;
   out_3607328369657124906[18] = 0;
   out_3607328369657124906[19] = 0;
   out_3607328369657124906[20] = 0;
   out_3607328369657124906[21] = 0;
   out_3607328369657124906[22] = 1;
   out_3607328369657124906[23] = 0;
   out_3607328369657124906[24] = 0;
   out_3607328369657124906[25] = 0;
   out_3607328369657124906[26] = 0;
   out_3607328369657124906[27] = 0;
   out_3607328369657124906[28] = 0;
   out_3607328369657124906[29] = 0;
   out_3607328369657124906[30] = 0;
   out_3607328369657124906[31] = 0;
   out_3607328369657124906[32] = 0;
   out_3607328369657124906[33] = 0;
   out_3607328369657124906[34] = 0;
   out_3607328369657124906[35] = 0;
   out_3607328369657124906[36] = 0;
   out_3607328369657124906[37] = 0;
   out_3607328369657124906[38] = 0;
   out_3607328369657124906[39] = 0;
   out_3607328369657124906[40] = 0;
   out_3607328369657124906[41] = 1;
   out_3607328369657124906[42] = 0;
   out_3607328369657124906[43] = 0;
   out_3607328369657124906[44] = 0;
   out_3607328369657124906[45] = 0;
   out_3607328369657124906[46] = 0;
   out_3607328369657124906[47] = 0;
   out_3607328369657124906[48] = 0;
   out_3607328369657124906[49] = 0;
   out_3607328369657124906[50] = 0;
   out_3607328369657124906[51] = 0;
   out_3607328369657124906[52] = 0;
   out_3607328369657124906[53] = 0;
}
void h_14(double *state, double *unused, double *out_4846369707862575646) {
   out_4846369707862575646[0] = state[6];
   out_4846369707862575646[1] = state[7];
   out_4846369707862575646[2] = state[8];
}
void H_14(double *state, double *unused, double *out_7086091270954948319) {
   out_7086091270954948319[0] = 0;
   out_7086091270954948319[1] = 0;
   out_7086091270954948319[2] = 0;
   out_7086091270954948319[3] = 0;
   out_7086091270954948319[4] = 0;
   out_7086091270954948319[5] = 0;
   out_7086091270954948319[6] = 1;
   out_7086091270954948319[7] = 0;
   out_7086091270954948319[8] = 0;
   out_7086091270954948319[9] = 0;
   out_7086091270954948319[10] = 0;
   out_7086091270954948319[11] = 0;
   out_7086091270954948319[12] = 0;
   out_7086091270954948319[13] = 0;
   out_7086091270954948319[14] = 0;
   out_7086091270954948319[15] = 0;
   out_7086091270954948319[16] = 0;
   out_7086091270954948319[17] = 0;
   out_7086091270954948319[18] = 0;
   out_7086091270954948319[19] = 0;
   out_7086091270954948319[20] = 0;
   out_7086091270954948319[21] = 0;
   out_7086091270954948319[22] = 0;
   out_7086091270954948319[23] = 0;
   out_7086091270954948319[24] = 0;
   out_7086091270954948319[25] = 1;
   out_7086091270954948319[26] = 0;
   out_7086091270954948319[27] = 0;
   out_7086091270954948319[28] = 0;
   out_7086091270954948319[29] = 0;
   out_7086091270954948319[30] = 0;
   out_7086091270954948319[31] = 0;
   out_7086091270954948319[32] = 0;
   out_7086091270954948319[33] = 0;
   out_7086091270954948319[34] = 0;
   out_7086091270954948319[35] = 0;
   out_7086091270954948319[36] = 0;
   out_7086091270954948319[37] = 0;
   out_7086091270954948319[38] = 0;
   out_7086091270954948319[39] = 0;
   out_7086091270954948319[40] = 0;
   out_7086091270954948319[41] = 0;
   out_7086091270954948319[42] = 0;
   out_7086091270954948319[43] = 0;
   out_7086091270954948319[44] = 1;
   out_7086091270954948319[45] = 0;
   out_7086091270954948319[46] = 0;
   out_7086091270954948319[47] = 0;
   out_7086091270954948319[48] = 0;
   out_7086091270954948319[49] = 0;
   out_7086091270954948319[50] = 0;
   out_7086091270954948319[51] = 0;
   out_7086091270954948319[52] = 0;
   out_7086091270954948319[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_5204449900680294893) {
  err_fun(nom_x, delta_x, out_5204449900680294893);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3166247841107463599) {
  inv_err_fun(nom_x, true_x, out_3166247841107463599);
}
void pose_H_mod_fun(double *state, double *out_4757463840714288630) {
  H_mod_fun(state, out_4757463840714288630);
}
void pose_f_fun(double *state, double dt, double *out_1433841035840211) {
  f_fun(state,  dt, out_1433841035840211);
}
void pose_F_fun(double *state, double dt, double *out_3040199541343879647) {
  F_fun(state,  dt, out_3040199541343879647);
}
void pose_h_4(double *state, double *unused, double *out_942367839052342030) {
  h_4(state, unused, out_942367839052342030);
}
void pose_H_4(double *state, double *unused, double *out_4003302838659576023) {
  H_4(state, unused, out_4003302838659576023);
}
void pose_h_10(double *state, double *unused, double *out_8571903349869224763) {
  h_10(state, unused, out_8571903349869224763);
}
void pose_H_10(double *state, double *unused, double *out_3736312142713721773) {
  H_10(state, unused, out_3736312142713721773);
}
void pose_h_13(double *state, double *unused, double *out_2382082840634836033) {
  h_13(state, unused, out_2382082840634836033);
}
void pose_H_13(double *state, double *unused, double *out_3607328369657124906) {
  H_13(state, unused, out_3607328369657124906);
}
void pose_h_14(double *state, double *unused, double *out_4846369707862575646) {
  h_14(state, unused, out_4846369707862575646);
}
void pose_H_14(double *state, double *unused, double *out_7086091270954948319) {
  H_14(state, unused, out_7086091270954948319);
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
