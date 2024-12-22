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
void err_fun(double *nom_x, double *delta_x, double *out_289849441806978041) {
   out_289849441806978041[0] = delta_x[0] + nom_x[0];
   out_289849441806978041[1] = delta_x[1] + nom_x[1];
   out_289849441806978041[2] = delta_x[2] + nom_x[2];
   out_289849441806978041[3] = delta_x[3] + nom_x[3];
   out_289849441806978041[4] = delta_x[4] + nom_x[4];
   out_289849441806978041[5] = delta_x[5] + nom_x[5];
   out_289849441806978041[6] = delta_x[6] + nom_x[6];
   out_289849441806978041[7] = delta_x[7] + nom_x[7];
   out_289849441806978041[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5397291325702136028) {
   out_5397291325702136028[0] = -nom_x[0] + true_x[0];
   out_5397291325702136028[1] = -nom_x[1] + true_x[1];
   out_5397291325702136028[2] = -nom_x[2] + true_x[2];
   out_5397291325702136028[3] = -nom_x[3] + true_x[3];
   out_5397291325702136028[4] = -nom_x[4] + true_x[4];
   out_5397291325702136028[5] = -nom_x[5] + true_x[5];
   out_5397291325702136028[6] = -nom_x[6] + true_x[6];
   out_5397291325702136028[7] = -nom_x[7] + true_x[7];
   out_5397291325702136028[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1990892277934241204) {
   out_1990892277934241204[0] = 1.0;
   out_1990892277934241204[1] = 0.0;
   out_1990892277934241204[2] = 0.0;
   out_1990892277934241204[3] = 0.0;
   out_1990892277934241204[4] = 0.0;
   out_1990892277934241204[5] = 0.0;
   out_1990892277934241204[6] = 0.0;
   out_1990892277934241204[7] = 0.0;
   out_1990892277934241204[8] = 0.0;
   out_1990892277934241204[9] = 0.0;
   out_1990892277934241204[10] = 1.0;
   out_1990892277934241204[11] = 0.0;
   out_1990892277934241204[12] = 0.0;
   out_1990892277934241204[13] = 0.0;
   out_1990892277934241204[14] = 0.0;
   out_1990892277934241204[15] = 0.0;
   out_1990892277934241204[16] = 0.0;
   out_1990892277934241204[17] = 0.0;
   out_1990892277934241204[18] = 0.0;
   out_1990892277934241204[19] = 0.0;
   out_1990892277934241204[20] = 1.0;
   out_1990892277934241204[21] = 0.0;
   out_1990892277934241204[22] = 0.0;
   out_1990892277934241204[23] = 0.0;
   out_1990892277934241204[24] = 0.0;
   out_1990892277934241204[25] = 0.0;
   out_1990892277934241204[26] = 0.0;
   out_1990892277934241204[27] = 0.0;
   out_1990892277934241204[28] = 0.0;
   out_1990892277934241204[29] = 0.0;
   out_1990892277934241204[30] = 1.0;
   out_1990892277934241204[31] = 0.0;
   out_1990892277934241204[32] = 0.0;
   out_1990892277934241204[33] = 0.0;
   out_1990892277934241204[34] = 0.0;
   out_1990892277934241204[35] = 0.0;
   out_1990892277934241204[36] = 0.0;
   out_1990892277934241204[37] = 0.0;
   out_1990892277934241204[38] = 0.0;
   out_1990892277934241204[39] = 0.0;
   out_1990892277934241204[40] = 1.0;
   out_1990892277934241204[41] = 0.0;
   out_1990892277934241204[42] = 0.0;
   out_1990892277934241204[43] = 0.0;
   out_1990892277934241204[44] = 0.0;
   out_1990892277934241204[45] = 0.0;
   out_1990892277934241204[46] = 0.0;
   out_1990892277934241204[47] = 0.0;
   out_1990892277934241204[48] = 0.0;
   out_1990892277934241204[49] = 0.0;
   out_1990892277934241204[50] = 1.0;
   out_1990892277934241204[51] = 0.0;
   out_1990892277934241204[52] = 0.0;
   out_1990892277934241204[53] = 0.0;
   out_1990892277934241204[54] = 0.0;
   out_1990892277934241204[55] = 0.0;
   out_1990892277934241204[56] = 0.0;
   out_1990892277934241204[57] = 0.0;
   out_1990892277934241204[58] = 0.0;
   out_1990892277934241204[59] = 0.0;
   out_1990892277934241204[60] = 1.0;
   out_1990892277934241204[61] = 0.0;
   out_1990892277934241204[62] = 0.0;
   out_1990892277934241204[63] = 0.0;
   out_1990892277934241204[64] = 0.0;
   out_1990892277934241204[65] = 0.0;
   out_1990892277934241204[66] = 0.0;
   out_1990892277934241204[67] = 0.0;
   out_1990892277934241204[68] = 0.0;
   out_1990892277934241204[69] = 0.0;
   out_1990892277934241204[70] = 1.0;
   out_1990892277934241204[71] = 0.0;
   out_1990892277934241204[72] = 0.0;
   out_1990892277934241204[73] = 0.0;
   out_1990892277934241204[74] = 0.0;
   out_1990892277934241204[75] = 0.0;
   out_1990892277934241204[76] = 0.0;
   out_1990892277934241204[77] = 0.0;
   out_1990892277934241204[78] = 0.0;
   out_1990892277934241204[79] = 0.0;
   out_1990892277934241204[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3261501180825990020) {
   out_3261501180825990020[0] = state[0];
   out_3261501180825990020[1] = state[1];
   out_3261501180825990020[2] = state[2];
   out_3261501180825990020[3] = state[3];
   out_3261501180825990020[4] = state[4];
   out_3261501180825990020[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3261501180825990020[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3261501180825990020[7] = state[7];
   out_3261501180825990020[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2361970306278506267) {
   out_2361970306278506267[0] = 1;
   out_2361970306278506267[1] = 0;
   out_2361970306278506267[2] = 0;
   out_2361970306278506267[3] = 0;
   out_2361970306278506267[4] = 0;
   out_2361970306278506267[5] = 0;
   out_2361970306278506267[6] = 0;
   out_2361970306278506267[7] = 0;
   out_2361970306278506267[8] = 0;
   out_2361970306278506267[9] = 0;
   out_2361970306278506267[10] = 1;
   out_2361970306278506267[11] = 0;
   out_2361970306278506267[12] = 0;
   out_2361970306278506267[13] = 0;
   out_2361970306278506267[14] = 0;
   out_2361970306278506267[15] = 0;
   out_2361970306278506267[16] = 0;
   out_2361970306278506267[17] = 0;
   out_2361970306278506267[18] = 0;
   out_2361970306278506267[19] = 0;
   out_2361970306278506267[20] = 1;
   out_2361970306278506267[21] = 0;
   out_2361970306278506267[22] = 0;
   out_2361970306278506267[23] = 0;
   out_2361970306278506267[24] = 0;
   out_2361970306278506267[25] = 0;
   out_2361970306278506267[26] = 0;
   out_2361970306278506267[27] = 0;
   out_2361970306278506267[28] = 0;
   out_2361970306278506267[29] = 0;
   out_2361970306278506267[30] = 1;
   out_2361970306278506267[31] = 0;
   out_2361970306278506267[32] = 0;
   out_2361970306278506267[33] = 0;
   out_2361970306278506267[34] = 0;
   out_2361970306278506267[35] = 0;
   out_2361970306278506267[36] = 0;
   out_2361970306278506267[37] = 0;
   out_2361970306278506267[38] = 0;
   out_2361970306278506267[39] = 0;
   out_2361970306278506267[40] = 1;
   out_2361970306278506267[41] = 0;
   out_2361970306278506267[42] = 0;
   out_2361970306278506267[43] = 0;
   out_2361970306278506267[44] = 0;
   out_2361970306278506267[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2361970306278506267[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2361970306278506267[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2361970306278506267[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2361970306278506267[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2361970306278506267[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2361970306278506267[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2361970306278506267[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2361970306278506267[53] = -9.8000000000000007*dt;
   out_2361970306278506267[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2361970306278506267[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2361970306278506267[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2361970306278506267[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2361970306278506267[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2361970306278506267[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2361970306278506267[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2361970306278506267[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2361970306278506267[62] = 0;
   out_2361970306278506267[63] = 0;
   out_2361970306278506267[64] = 0;
   out_2361970306278506267[65] = 0;
   out_2361970306278506267[66] = 0;
   out_2361970306278506267[67] = 0;
   out_2361970306278506267[68] = 0;
   out_2361970306278506267[69] = 0;
   out_2361970306278506267[70] = 1;
   out_2361970306278506267[71] = 0;
   out_2361970306278506267[72] = 0;
   out_2361970306278506267[73] = 0;
   out_2361970306278506267[74] = 0;
   out_2361970306278506267[75] = 0;
   out_2361970306278506267[76] = 0;
   out_2361970306278506267[77] = 0;
   out_2361970306278506267[78] = 0;
   out_2361970306278506267[79] = 0;
   out_2361970306278506267[80] = 1;
}
void h_25(double *state, double *unused, double *out_1300550002286695650) {
   out_1300550002286695650[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7262663569863073175) {
   out_7262663569863073175[0] = 0;
   out_7262663569863073175[1] = 0;
   out_7262663569863073175[2] = 0;
   out_7262663569863073175[3] = 0;
   out_7262663569863073175[4] = 0;
   out_7262663569863073175[5] = 0;
   out_7262663569863073175[6] = 1;
   out_7262663569863073175[7] = 0;
   out_7262663569863073175[8] = 0;
}
void h_24(double *state, double *unused, double *out_1636726945547299812) {
   out_1636726945547299812[0] = state[4];
   out_1636726945547299812[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2442342065207084912) {
   out_2442342065207084912[0] = 0;
   out_2442342065207084912[1] = 0;
   out_2442342065207084912[2] = 0;
   out_2442342065207084912[3] = 0;
   out_2442342065207084912[4] = 1;
   out_2442342065207084912[5] = 0;
   out_2442342065207084912[6] = 0;
   out_2442342065207084912[7] = 0;
   out_2442342065207084912[8] = 0;
   out_2442342065207084912[9] = 0;
   out_2442342065207084912[10] = 0;
   out_2442342065207084912[11] = 0;
   out_2442342065207084912[12] = 0;
   out_2442342065207084912[13] = 0;
   out_2442342065207084912[14] = 1;
   out_2442342065207084912[15] = 0;
   out_2442342065207084912[16] = 0;
   out_2442342065207084912[17] = 0;
}
void h_30(double *state, double *unused, double *out_7691897990432309255) {
   out_7691897990432309255[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7133324622719833105) {
   out_7133324622719833105[0] = 0;
   out_7133324622719833105[1] = 0;
   out_7133324622719833105[2] = 0;
   out_7133324622719833105[3] = 0;
   out_7133324622719833105[4] = 1;
   out_7133324622719833105[5] = 0;
   out_7133324622719833105[6] = 0;
   out_7133324622719833105[7] = 0;
   out_7133324622719833105[8] = 0;
}
void h_26(double *state, double *unused, double *out_6524154189356167518) {
   out_6524154189356167518[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3521160250989016951) {
   out_3521160250989016951[0] = 0;
   out_3521160250989016951[1] = 0;
   out_3521160250989016951[2] = 0;
   out_3521160250989016951[3] = 0;
   out_3521160250989016951[4] = 0;
   out_3521160250989016951[5] = 0;
   out_3521160250989016951[6] = 0;
   out_3521160250989016951[7] = 1;
   out_3521160250989016951[8] = 0;
}
void h_27(double *state, double *unused, double *out_3701123265374149995) {
   out_3701123265374149995[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4958561310919408194) {
   out_4958561310919408194[0] = 0;
   out_4958561310919408194[1] = 0;
   out_4958561310919408194[2] = 0;
   out_4958561310919408194[3] = 1;
   out_4958561310919408194[4] = 0;
   out_4958561310919408194[5] = 0;
   out_4958561310919408194[6] = 0;
   out_4958561310919408194[7] = 0;
   out_4958561310919408194[8] = 0;
}
void h_29(double *state, double *unused, double *out_3976317327658655884) {
   out_3976317327658655884[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7643555967034225289) {
   out_7643555967034225289[0] = 0;
   out_7643555967034225289[1] = 1;
   out_7643555967034225289[2] = 0;
   out_7643555967034225289[3] = 0;
   out_7643555967034225289[4] = 0;
   out_7643555967034225289[5] = 0;
   out_7643555967034225289[6] = 0;
   out_7643555967034225289[7] = 0;
   out_7643555967034225289[8] = 0;
}
void h_28(double *state, double *unused, double *out_8702796533034995640) {
   out_8702796533034995640[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8839557835110000076) {
   out_8839557835110000076[0] = 1;
   out_8839557835110000076[1] = 0;
   out_8839557835110000076[2] = 0;
   out_8839557835110000076[3] = 0;
   out_8839557835110000076[4] = 0;
   out_8839557835110000076[5] = 0;
   out_8839557835110000076[6] = 0;
   out_8839557835110000076[7] = 0;
   out_8839557835110000076[8] = 0;
}
void h_31(double *state, double *unused, double *out_150081390342187517) {
   out_150081390342187517[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7293309531740033603) {
   out_7293309531740033603[0] = 0;
   out_7293309531740033603[1] = 0;
   out_7293309531740033603[2] = 0;
   out_7293309531740033603[3] = 0;
   out_7293309531740033603[4] = 0;
   out_7293309531740033603[5] = 0;
   out_7293309531740033603[6] = 0;
   out_7293309531740033603[7] = 0;
   out_7293309531740033603[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_289849441806978041) {
  err_fun(nom_x, delta_x, out_289849441806978041);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5397291325702136028) {
  inv_err_fun(nom_x, true_x, out_5397291325702136028);
}
void car_H_mod_fun(double *state, double *out_1990892277934241204) {
  H_mod_fun(state, out_1990892277934241204);
}
void car_f_fun(double *state, double dt, double *out_3261501180825990020) {
  f_fun(state,  dt, out_3261501180825990020);
}
void car_F_fun(double *state, double dt, double *out_2361970306278506267) {
  F_fun(state,  dt, out_2361970306278506267);
}
void car_h_25(double *state, double *unused, double *out_1300550002286695650) {
  h_25(state, unused, out_1300550002286695650);
}
void car_H_25(double *state, double *unused, double *out_7262663569863073175) {
  H_25(state, unused, out_7262663569863073175);
}
void car_h_24(double *state, double *unused, double *out_1636726945547299812) {
  h_24(state, unused, out_1636726945547299812);
}
void car_H_24(double *state, double *unused, double *out_2442342065207084912) {
  H_24(state, unused, out_2442342065207084912);
}
void car_h_30(double *state, double *unused, double *out_7691897990432309255) {
  h_30(state, unused, out_7691897990432309255);
}
void car_H_30(double *state, double *unused, double *out_7133324622719833105) {
  H_30(state, unused, out_7133324622719833105);
}
void car_h_26(double *state, double *unused, double *out_6524154189356167518) {
  h_26(state, unused, out_6524154189356167518);
}
void car_H_26(double *state, double *unused, double *out_3521160250989016951) {
  H_26(state, unused, out_3521160250989016951);
}
void car_h_27(double *state, double *unused, double *out_3701123265374149995) {
  h_27(state, unused, out_3701123265374149995);
}
void car_H_27(double *state, double *unused, double *out_4958561310919408194) {
  H_27(state, unused, out_4958561310919408194);
}
void car_h_29(double *state, double *unused, double *out_3976317327658655884) {
  h_29(state, unused, out_3976317327658655884);
}
void car_H_29(double *state, double *unused, double *out_7643555967034225289) {
  H_29(state, unused, out_7643555967034225289);
}
void car_h_28(double *state, double *unused, double *out_8702796533034995640) {
  h_28(state, unused, out_8702796533034995640);
}
void car_H_28(double *state, double *unused, double *out_8839557835110000076) {
  H_28(state, unused, out_8839557835110000076);
}
void car_h_31(double *state, double *unused, double *out_150081390342187517) {
  h_31(state, unused, out_150081390342187517);
}
void car_H_31(double *state, double *unused, double *out_7293309531740033603) {
  H_31(state, unused, out_7293309531740033603);
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
