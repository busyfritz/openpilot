#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.13.2                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2770383136982745722) {
   out_2770383136982745722[0] = delta_x[0] + nom_x[0];
   out_2770383136982745722[1] = delta_x[1] + nom_x[1];
   out_2770383136982745722[2] = delta_x[2] + nom_x[2];
   out_2770383136982745722[3] = delta_x[3] + nom_x[3];
   out_2770383136982745722[4] = delta_x[4] + nom_x[4];
   out_2770383136982745722[5] = delta_x[5] + nom_x[5];
   out_2770383136982745722[6] = delta_x[6] + nom_x[6];
   out_2770383136982745722[7] = delta_x[7] + nom_x[7];
   out_2770383136982745722[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1833603901144997724) {
   out_1833603901144997724[0] = -nom_x[0] + true_x[0];
   out_1833603901144997724[1] = -nom_x[1] + true_x[1];
   out_1833603901144997724[2] = -nom_x[2] + true_x[2];
   out_1833603901144997724[3] = -nom_x[3] + true_x[3];
   out_1833603901144997724[4] = -nom_x[4] + true_x[4];
   out_1833603901144997724[5] = -nom_x[5] + true_x[5];
   out_1833603901144997724[6] = -nom_x[6] + true_x[6];
   out_1833603901144997724[7] = -nom_x[7] + true_x[7];
   out_1833603901144997724[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_4081607929894336817) {
   out_4081607929894336817[0] = 1.0;
   out_4081607929894336817[1] = 0.0;
   out_4081607929894336817[2] = 0.0;
   out_4081607929894336817[3] = 0.0;
   out_4081607929894336817[4] = 0.0;
   out_4081607929894336817[5] = 0.0;
   out_4081607929894336817[6] = 0.0;
   out_4081607929894336817[7] = 0.0;
   out_4081607929894336817[8] = 0.0;
   out_4081607929894336817[9] = 0.0;
   out_4081607929894336817[10] = 1.0;
   out_4081607929894336817[11] = 0.0;
   out_4081607929894336817[12] = 0.0;
   out_4081607929894336817[13] = 0.0;
   out_4081607929894336817[14] = 0.0;
   out_4081607929894336817[15] = 0.0;
   out_4081607929894336817[16] = 0.0;
   out_4081607929894336817[17] = 0.0;
   out_4081607929894336817[18] = 0.0;
   out_4081607929894336817[19] = 0.0;
   out_4081607929894336817[20] = 1.0;
   out_4081607929894336817[21] = 0.0;
   out_4081607929894336817[22] = 0.0;
   out_4081607929894336817[23] = 0.0;
   out_4081607929894336817[24] = 0.0;
   out_4081607929894336817[25] = 0.0;
   out_4081607929894336817[26] = 0.0;
   out_4081607929894336817[27] = 0.0;
   out_4081607929894336817[28] = 0.0;
   out_4081607929894336817[29] = 0.0;
   out_4081607929894336817[30] = 1.0;
   out_4081607929894336817[31] = 0.0;
   out_4081607929894336817[32] = 0.0;
   out_4081607929894336817[33] = 0.0;
   out_4081607929894336817[34] = 0.0;
   out_4081607929894336817[35] = 0.0;
   out_4081607929894336817[36] = 0.0;
   out_4081607929894336817[37] = 0.0;
   out_4081607929894336817[38] = 0.0;
   out_4081607929894336817[39] = 0.0;
   out_4081607929894336817[40] = 1.0;
   out_4081607929894336817[41] = 0.0;
   out_4081607929894336817[42] = 0.0;
   out_4081607929894336817[43] = 0.0;
   out_4081607929894336817[44] = 0.0;
   out_4081607929894336817[45] = 0.0;
   out_4081607929894336817[46] = 0.0;
   out_4081607929894336817[47] = 0.0;
   out_4081607929894336817[48] = 0.0;
   out_4081607929894336817[49] = 0.0;
   out_4081607929894336817[50] = 1.0;
   out_4081607929894336817[51] = 0.0;
   out_4081607929894336817[52] = 0.0;
   out_4081607929894336817[53] = 0.0;
   out_4081607929894336817[54] = 0.0;
   out_4081607929894336817[55] = 0.0;
   out_4081607929894336817[56] = 0.0;
   out_4081607929894336817[57] = 0.0;
   out_4081607929894336817[58] = 0.0;
   out_4081607929894336817[59] = 0.0;
   out_4081607929894336817[60] = 1.0;
   out_4081607929894336817[61] = 0.0;
   out_4081607929894336817[62] = 0.0;
   out_4081607929894336817[63] = 0.0;
   out_4081607929894336817[64] = 0.0;
   out_4081607929894336817[65] = 0.0;
   out_4081607929894336817[66] = 0.0;
   out_4081607929894336817[67] = 0.0;
   out_4081607929894336817[68] = 0.0;
   out_4081607929894336817[69] = 0.0;
   out_4081607929894336817[70] = 1.0;
   out_4081607929894336817[71] = 0.0;
   out_4081607929894336817[72] = 0.0;
   out_4081607929894336817[73] = 0.0;
   out_4081607929894336817[74] = 0.0;
   out_4081607929894336817[75] = 0.0;
   out_4081607929894336817[76] = 0.0;
   out_4081607929894336817[77] = 0.0;
   out_4081607929894336817[78] = 0.0;
   out_4081607929894336817[79] = 0.0;
   out_4081607929894336817[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_8331951914791866149) {
   out_8331951914791866149[0] = state[0];
   out_8331951914791866149[1] = state[1];
   out_8331951914791866149[2] = state[2];
   out_8331951914791866149[3] = state[3];
   out_8331951914791866149[4] = state[4];
   out_8331951914791866149[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_8331951914791866149[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_8331951914791866149[7] = state[7];
   out_8331951914791866149[8] = state[8];
}
void F_fun(double *state, double dt, double *out_5109431088893287780) {
   out_5109431088893287780[0] = 1;
   out_5109431088893287780[1] = 0;
   out_5109431088893287780[2] = 0;
   out_5109431088893287780[3] = 0;
   out_5109431088893287780[4] = 0;
   out_5109431088893287780[5] = 0;
   out_5109431088893287780[6] = 0;
   out_5109431088893287780[7] = 0;
   out_5109431088893287780[8] = 0;
   out_5109431088893287780[9] = 0;
   out_5109431088893287780[10] = 1;
   out_5109431088893287780[11] = 0;
   out_5109431088893287780[12] = 0;
   out_5109431088893287780[13] = 0;
   out_5109431088893287780[14] = 0;
   out_5109431088893287780[15] = 0;
   out_5109431088893287780[16] = 0;
   out_5109431088893287780[17] = 0;
   out_5109431088893287780[18] = 0;
   out_5109431088893287780[19] = 0;
   out_5109431088893287780[20] = 1;
   out_5109431088893287780[21] = 0;
   out_5109431088893287780[22] = 0;
   out_5109431088893287780[23] = 0;
   out_5109431088893287780[24] = 0;
   out_5109431088893287780[25] = 0;
   out_5109431088893287780[26] = 0;
   out_5109431088893287780[27] = 0;
   out_5109431088893287780[28] = 0;
   out_5109431088893287780[29] = 0;
   out_5109431088893287780[30] = 1;
   out_5109431088893287780[31] = 0;
   out_5109431088893287780[32] = 0;
   out_5109431088893287780[33] = 0;
   out_5109431088893287780[34] = 0;
   out_5109431088893287780[35] = 0;
   out_5109431088893287780[36] = 0;
   out_5109431088893287780[37] = 0;
   out_5109431088893287780[38] = 0;
   out_5109431088893287780[39] = 0;
   out_5109431088893287780[40] = 1;
   out_5109431088893287780[41] = 0;
   out_5109431088893287780[42] = 0;
   out_5109431088893287780[43] = 0;
   out_5109431088893287780[44] = 0;
   out_5109431088893287780[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_5109431088893287780[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_5109431088893287780[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5109431088893287780[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5109431088893287780[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_5109431088893287780[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_5109431088893287780[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_5109431088893287780[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_5109431088893287780[53] = -9.8000000000000007*dt;
   out_5109431088893287780[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_5109431088893287780[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_5109431088893287780[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5109431088893287780[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5109431088893287780[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_5109431088893287780[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_5109431088893287780[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_5109431088893287780[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5109431088893287780[62] = 0;
   out_5109431088893287780[63] = 0;
   out_5109431088893287780[64] = 0;
   out_5109431088893287780[65] = 0;
   out_5109431088893287780[66] = 0;
   out_5109431088893287780[67] = 0;
   out_5109431088893287780[68] = 0;
   out_5109431088893287780[69] = 0;
   out_5109431088893287780[70] = 1;
   out_5109431088893287780[71] = 0;
   out_5109431088893287780[72] = 0;
   out_5109431088893287780[73] = 0;
   out_5109431088893287780[74] = 0;
   out_5109431088893287780[75] = 0;
   out_5109431088893287780[76] = 0;
   out_5109431088893287780[77] = 0;
   out_5109431088893287780[78] = 0;
   out_5109431088893287780[79] = 0;
   out_5109431088893287780[80] = 1;
}
void h_25(double *state, double *unused, double *out_1573818349272192953) {
   out_1573818349272192953[0] = state[6];
}
void H_25(double *state, double *unused, double *out_5171947917902977562) {
   out_5171947917902977562[0] = 0;
   out_5171947917902977562[1] = 0;
   out_5171947917902977562[2] = 0;
   out_5171947917902977562[3] = 0;
   out_5171947917902977562[4] = 0;
   out_5171947917902977562[5] = 0;
   out_5171947917902977562[6] = 1;
   out_5171947917902977562[7] = 0;
   out_5171947917902977562[8] = 0;
}
void h_24(double *state, double *unused, double *out_1070798443338542178) {
   out_1070798443338542178[0] = state[4];
   out_1070798443338542178[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2999298318897477996) {
   out_2999298318897477996[0] = 0;
   out_2999298318897477996[1] = 0;
   out_2999298318897477996[2] = 0;
   out_2999298318897477996[3] = 0;
   out_2999298318897477996[4] = 1;
   out_2999298318897477996[5] = 0;
   out_2999298318897477996[6] = 0;
   out_2999298318897477996[7] = 0;
   out_2999298318897477996[8] = 0;
   out_2999298318897477996[9] = 0;
   out_2999298318897477996[10] = 0;
   out_2999298318897477996[11] = 0;
   out_2999298318897477996[12] = 0;
   out_2999298318897477996[13] = 0;
   out_2999298318897477996[14] = 1;
   out_2999298318897477996[15] = 0;
   out_2999298318897477996[16] = 0;
   out_2999298318897477996[17] = 0;
}
void h_30(double *state, double *unused, double *out_2287865127156308699) {
   out_2287865127156308699[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6358105814314957299) {
   out_6358105814314957299[0] = 0;
   out_6358105814314957299[1] = 0;
   out_6358105814314957299[2] = 0;
   out_6358105814314957299[3] = 0;
   out_6358105814314957299[4] = 1;
   out_6358105814314957299[5] = 0;
   out_6358105814314957299[6] = 0;
   out_6358105814314957299[7] = 0;
   out_6358105814314957299[8] = 0;
}
void h_26(double *state, double *unused, double *out_2778552391562034024) {
   out_2778552391562034024[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1430444599028921338) {
   out_1430444599028921338[0] = 0;
   out_1430444599028921338[1] = 0;
   out_1430444599028921338[2] = 0;
   out_1430444599028921338[3] = 0;
   out_1430444599028921338[4] = 0;
   out_1430444599028921338[5] = 0;
   out_1430444599028921338[6] = 0;
   out_1430444599028921338[7] = 1;
   out_1430444599028921338[8] = 0;
}
void h_27(double *state, double *unused, double *out_8740169787446037511) {
   out_8740169787446037511[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8532869126115382210) {
   out_8532869126115382210[0] = 0;
   out_8532869126115382210[1] = 0;
   out_8532869126115382210[2] = 0;
   out_8532869126115382210[3] = 1;
   out_8532869126115382210[4] = 0;
   out_8532869126115382210[5] = 0;
   out_8532869126115382210[6] = 0;
   out_8532869126115382210[7] = 0;
   out_8532869126115382210[8] = 0;
}
void h_29(double *state, double *unused, double *out_7289538394817154344) {
   out_7289538394817154344[0] = state[1];
}
void H_29(double *state, double *unused, double *out_8200512220724618373) {
   out_8200512220724618373[0] = 0;
   out_8200512220724618373[1] = 1;
   out_8200512220724618373[2] = 0;
   out_8200512220724618373[3] = 0;
   out_8200512220724618373[4] = 0;
   out_8200512220724618373[5] = 0;
   out_8200512220724618373[6] = 0;
   out_8200512220724618373[7] = 0;
   out_8200512220724618373[8] = 0;
}
void h_28(double *state, double *unused, double *out_6301052235452206598) {
   out_6301052235452206598[0] = state[0];
}
void H_28(double *state, double *unused, double *out_3118113203655087799) {
   out_3118113203655087799[0] = 1;
   out_3118113203655087799[1] = 0;
   out_3118113203655087799[2] = 0;
   out_3118113203655087799[3] = 0;
   out_3118113203655087799[4] = 0;
   out_3118113203655087799[5] = 0;
   out_3118113203655087799[6] = 0;
   out_3118113203655087799[7] = 0;
   out_3118113203655087799[8] = 0;
}
void h_31(double *state, double *unused, double *out_7937412327574008814) {
   out_7937412327574008814[0] = state[8];
}
void H_31(double *state, double *unused, double *out_5202593879779937990) {
   out_5202593879779937990[0] = 0;
   out_5202593879779937990[1] = 0;
   out_5202593879779937990[2] = 0;
   out_5202593879779937990[3] = 0;
   out_5202593879779937990[4] = 0;
   out_5202593879779937990[5] = 0;
   out_5202593879779937990[6] = 0;
   out_5202593879779937990[7] = 0;
   out_5202593879779937990[8] = 1;
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

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_2770383136982745722) {
  err_fun(nom_x, delta_x, out_2770383136982745722);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1833603901144997724) {
  inv_err_fun(nom_x, true_x, out_1833603901144997724);
}
void car_H_mod_fun(double *state, double *out_4081607929894336817) {
  H_mod_fun(state, out_4081607929894336817);
}
void car_f_fun(double *state, double dt, double *out_8331951914791866149) {
  f_fun(state,  dt, out_8331951914791866149);
}
void car_F_fun(double *state, double dt, double *out_5109431088893287780) {
  F_fun(state,  dt, out_5109431088893287780);
}
void car_h_25(double *state, double *unused, double *out_1573818349272192953) {
  h_25(state, unused, out_1573818349272192953);
}
void car_H_25(double *state, double *unused, double *out_5171947917902977562) {
  H_25(state, unused, out_5171947917902977562);
}
void car_h_24(double *state, double *unused, double *out_1070798443338542178) {
  h_24(state, unused, out_1070798443338542178);
}
void car_H_24(double *state, double *unused, double *out_2999298318897477996) {
  H_24(state, unused, out_2999298318897477996);
}
void car_h_30(double *state, double *unused, double *out_2287865127156308699) {
  h_30(state, unused, out_2287865127156308699);
}
void car_H_30(double *state, double *unused, double *out_6358105814314957299) {
  H_30(state, unused, out_6358105814314957299);
}
void car_h_26(double *state, double *unused, double *out_2778552391562034024) {
  h_26(state, unused, out_2778552391562034024);
}
void car_H_26(double *state, double *unused, double *out_1430444599028921338) {
  H_26(state, unused, out_1430444599028921338);
}
void car_h_27(double *state, double *unused, double *out_8740169787446037511) {
  h_27(state, unused, out_8740169787446037511);
}
void car_H_27(double *state, double *unused, double *out_8532869126115382210) {
  H_27(state, unused, out_8532869126115382210);
}
void car_h_29(double *state, double *unused, double *out_7289538394817154344) {
  h_29(state, unused, out_7289538394817154344);
}
void car_H_29(double *state, double *unused, double *out_8200512220724618373) {
  H_29(state, unused, out_8200512220724618373);
}
void car_h_28(double *state, double *unused, double *out_6301052235452206598) {
  h_28(state, unused, out_6301052235452206598);
}
void car_H_28(double *state, double *unused, double *out_3118113203655087799) {
  H_28(state, unused, out_3118113203655087799);
}
void car_h_31(double *state, double *unused, double *out_7937412327574008814) {
  h_31(state, unused, out_7937412327574008814);
}
void car_H_31(double *state, double *unused, double *out_5202593879779937990) {
  H_31(state, unused, out_5202593879779937990);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
