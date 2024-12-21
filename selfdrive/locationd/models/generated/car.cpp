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
void err_fun(double *nom_x, double *delta_x, double *out_4585888895666118916) {
   out_4585888895666118916[0] = delta_x[0] + nom_x[0];
   out_4585888895666118916[1] = delta_x[1] + nom_x[1];
   out_4585888895666118916[2] = delta_x[2] + nom_x[2];
   out_4585888895666118916[3] = delta_x[3] + nom_x[3];
   out_4585888895666118916[4] = delta_x[4] + nom_x[4];
   out_4585888895666118916[5] = delta_x[5] + nom_x[5];
   out_4585888895666118916[6] = delta_x[6] + nom_x[6];
   out_4585888895666118916[7] = delta_x[7] + nom_x[7];
   out_4585888895666118916[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_533703381334724490) {
   out_533703381334724490[0] = -nom_x[0] + true_x[0];
   out_533703381334724490[1] = -nom_x[1] + true_x[1];
   out_533703381334724490[2] = -nom_x[2] + true_x[2];
   out_533703381334724490[3] = -nom_x[3] + true_x[3];
   out_533703381334724490[4] = -nom_x[4] + true_x[4];
   out_533703381334724490[5] = -nom_x[5] + true_x[5];
   out_533703381334724490[6] = -nom_x[6] + true_x[6];
   out_533703381334724490[7] = -nom_x[7] + true_x[7];
   out_533703381334724490[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1455180481405070034) {
   out_1455180481405070034[0] = 1.0;
   out_1455180481405070034[1] = 0.0;
   out_1455180481405070034[2] = 0.0;
   out_1455180481405070034[3] = 0.0;
   out_1455180481405070034[4] = 0.0;
   out_1455180481405070034[5] = 0.0;
   out_1455180481405070034[6] = 0.0;
   out_1455180481405070034[7] = 0.0;
   out_1455180481405070034[8] = 0.0;
   out_1455180481405070034[9] = 0.0;
   out_1455180481405070034[10] = 1.0;
   out_1455180481405070034[11] = 0.0;
   out_1455180481405070034[12] = 0.0;
   out_1455180481405070034[13] = 0.0;
   out_1455180481405070034[14] = 0.0;
   out_1455180481405070034[15] = 0.0;
   out_1455180481405070034[16] = 0.0;
   out_1455180481405070034[17] = 0.0;
   out_1455180481405070034[18] = 0.0;
   out_1455180481405070034[19] = 0.0;
   out_1455180481405070034[20] = 1.0;
   out_1455180481405070034[21] = 0.0;
   out_1455180481405070034[22] = 0.0;
   out_1455180481405070034[23] = 0.0;
   out_1455180481405070034[24] = 0.0;
   out_1455180481405070034[25] = 0.0;
   out_1455180481405070034[26] = 0.0;
   out_1455180481405070034[27] = 0.0;
   out_1455180481405070034[28] = 0.0;
   out_1455180481405070034[29] = 0.0;
   out_1455180481405070034[30] = 1.0;
   out_1455180481405070034[31] = 0.0;
   out_1455180481405070034[32] = 0.0;
   out_1455180481405070034[33] = 0.0;
   out_1455180481405070034[34] = 0.0;
   out_1455180481405070034[35] = 0.0;
   out_1455180481405070034[36] = 0.0;
   out_1455180481405070034[37] = 0.0;
   out_1455180481405070034[38] = 0.0;
   out_1455180481405070034[39] = 0.0;
   out_1455180481405070034[40] = 1.0;
   out_1455180481405070034[41] = 0.0;
   out_1455180481405070034[42] = 0.0;
   out_1455180481405070034[43] = 0.0;
   out_1455180481405070034[44] = 0.0;
   out_1455180481405070034[45] = 0.0;
   out_1455180481405070034[46] = 0.0;
   out_1455180481405070034[47] = 0.0;
   out_1455180481405070034[48] = 0.0;
   out_1455180481405070034[49] = 0.0;
   out_1455180481405070034[50] = 1.0;
   out_1455180481405070034[51] = 0.0;
   out_1455180481405070034[52] = 0.0;
   out_1455180481405070034[53] = 0.0;
   out_1455180481405070034[54] = 0.0;
   out_1455180481405070034[55] = 0.0;
   out_1455180481405070034[56] = 0.0;
   out_1455180481405070034[57] = 0.0;
   out_1455180481405070034[58] = 0.0;
   out_1455180481405070034[59] = 0.0;
   out_1455180481405070034[60] = 1.0;
   out_1455180481405070034[61] = 0.0;
   out_1455180481405070034[62] = 0.0;
   out_1455180481405070034[63] = 0.0;
   out_1455180481405070034[64] = 0.0;
   out_1455180481405070034[65] = 0.0;
   out_1455180481405070034[66] = 0.0;
   out_1455180481405070034[67] = 0.0;
   out_1455180481405070034[68] = 0.0;
   out_1455180481405070034[69] = 0.0;
   out_1455180481405070034[70] = 1.0;
   out_1455180481405070034[71] = 0.0;
   out_1455180481405070034[72] = 0.0;
   out_1455180481405070034[73] = 0.0;
   out_1455180481405070034[74] = 0.0;
   out_1455180481405070034[75] = 0.0;
   out_1455180481405070034[76] = 0.0;
   out_1455180481405070034[77] = 0.0;
   out_1455180481405070034[78] = 0.0;
   out_1455180481405070034[79] = 0.0;
   out_1455180481405070034[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1890959688261970036) {
   out_1890959688261970036[0] = state[0];
   out_1890959688261970036[1] = state[1];
   out_1890959688261970036[2] = state[2];
   out_1890959688261970036[3] = state[3];
   out_1890959688261970036[4] = state[4];
   out_1890959688261970036[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1890959688261970036[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1890959688261970036[7] = state[7];
   out_1890959688261970036[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1775561225881524917) {
   out_1775561225881524917[0] = 1;
   out_1775561225881524917[1] = 0;
   out_1775561225881524917[2] = 0;
   out_1775561225881524917[3] = 0;
   out_1775561225881524917[4] = 0;
   out_1775561225881524917[5] = 0;
   out_1775561225881524917[6] = 0;
   out_1775561225881524917[7] = 0;
   out_1775561225881524917[8] = 0;
   out_1775561225881524917[9] = 0;
   out_1775561225881524917[10] = 1;
   out_1775561225881524917[11] = 0;
   out_1775561225881524917[12] = 0;
   out_1775561225881524917[13] = 0;
   out_1775561225881524917[14] = 0;
   out_1775561225881524917[15] = 0;
   out_1775561225881524917[16] = 0;
   out_1775561225881524917[17] = 0;
   out_1775561225881524917[18] = 0;
   out_1775561225881524917[19] = 0;
   out_1775561225881524917[20] = 1;
   out_1775561225881524917[21] = 0;
   out_1775561225881524917[22] = 0;
   out_1775561225881524917[23] = 0;
   out_1775561225881524917[24] = 0;
   out_1775561225881524917[25] = 0;
   out_1775561225881524917[26] = 0;
   out_1775561225881524917[27] = 0;
   out_1775561225881524917[28] = 0;
   out_1775561225881524917[29] = 0;
   out_1775561225881524917[30] = 1;
   out_1775561225881524917[31] = 0;
   out_1775561225881524917[32] = 0;
   out_1775561225881524917[33] = 0;
   out_1775561225881524917[34] = 0;
   out_1775561225881524917[35] = 0;
   out_1775561225881524917[36] = 0;
   out_1775561225881524917[37] = 0;
   out_1775561225881524917[38] = 0;
   out_1775561225881524917[39] = 0;
   out_1775561225881524917[40] = 1;
   out_1775561225881524917[41] = 0;
   out_1775561225881524917[42] = 0;
   out_1775561225881524917[43] = 0;
   out_1775561225881524917[44] = 0;
   out_1775561225881524917[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1775561225881524917[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1775561225881524917[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1775561225881524917[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1775561225881524917[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1775561225881524917[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1775561225881524917[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1775561225881524917[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1775561225881524917[53] = -9.8000000000000007*dt;
   out_1775561225881524917[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1775561225881524917[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1775561225881524917[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1775561225881524917[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1775561225881524917[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1775561225881524917[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1775561225881524917[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1775561225881524917[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1775561225881524917[62] = 0;
   out_1775561225881524917[63] = 0;
   out_1775561225881524917[64] = 0;
   out_1775561225881524917[65] = 0;
   out_1775561225881524917[66] = 0;
   out_1775561225881524917[67] = 0;
   out_1775561225881524917[68] = 0;
   out_1775561225881524917[69] = 0;
   out_1775561225881524917[70] = 1;
   out_1775561225881524917[71] = 0;
   out_1775561225881524917[72] = 0;
   out_1775561225881524917[73] = 0;
   out_1775561225881524917[74] = 0;
   out_1775561225881524917[75] = 0;
   out_1775561225881524917[76] = 0;
   out_1775561225881524917[77] = 0;
   out_1775561225881524917[78] = 0;
   out_1775561225881524917[79] = 0;
   out_1775561225881524917[80] = 1;
}
void h_25(double *state, double *unused, double *out_294409584145781914) {
   out_294409584145781914[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7798375366392244345) {
   out_7798375366392244345[0] = 0;
   out_7798375366392244345[1] = 0;
   out_7798375366392244345[2] = 0;
   out_7798375366392244345[3] = 0;
   out_7798375366392244345[4] = 0;
   out_7798375366392244345[5] = 0;
   out_7798375366392244345[6] = 1;
   out_7798375366392244345[7] = 0;
   out_7798375366392244345[8] = 0;
}
void h_24(double *state, double *unused, double *out_4279064028871003452) {
   out_4279064028871003452[0] = state[4];
   out_4279064028871003452[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8422660923338438709) {
   out_8422660923338438709[0] = 0;
   out_8422660923338438709[1] = 0;
   out_8422660923338438709[2] = 0;
   out_8422660923338438709[3] = 0;
   out_8422660923338438709[4] = 1;
   out_8422660923338438709[5] = 0;
   out_8422660923338438709[6] = 0;
   out_8422660923338438709[7] = 0;
   out_8422660923338438709[8] = 0;
   out_8422660923338438709[9] = 0;
   out_8422660923338438709[10] = 0;
   out_8422660923338438709[11] = 0;
   out_8422660923338438709[12] = 0;
   out_8422660923338438709[13] = 0;
   out_8422660923338438709[14] = 1;
   out_8422660923338438709[15] = 0;
   out_8422660923338438709[16] = 0;
   out_8422660923338438709[17] = 0;
}
void h_30(double *state, double *unused, double *out_3172795115046316330) {
   out_3172795115046316330[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3731678365825690516) {
   out_3731678365825690516[0] = 0;
   out_3731678365825690516[1] = 0;
   out_3731678365825690516[2] = 0;
   out_3731678365825690516[3] = 0;
   out_3731678365825690516[4] = 1;
   out_3731678365825690516[5] = 0;
   out_3731678365825690516[6] = 0;
   out_3731678365825690516[7] = 0;
   out_3731678365825690516[8] = 0;
}
void h_26(double *state, double *unused, double *out_4652723219620663290) {
   out_4652723219620663290[0] = state[7];
}
void H_26(double *state, double *unused, double *out_8455229430502556249) {
   out_8455229430502556249[0] = 0;
   out_8455229430502556249[1] = 0;
   out_8455229430502556249[2] = 0;
   out_8455229430502556249[3] = 0;
   out_8455229430502556249[4] = 0;
   out_8455229430502556249[5] = 0;
   out_8455229430502556249[6] = 0;
   out_8455229430502556249[7] = 1;
   out_8455229430502556249[8] = 0;
}
void h_27(double *state, double *unused, double *out_6052394498139519870) {
   out_6052394498139519870[0] = state[3];
}
void H_27(double *state, double *unused, double *out_5906441677626115427) {
   out_5906441677626115427[0] = 0;
   out_5906441677626115427[1] = 0;
   out_5906441677626115427[2] = 0;
   out_5906441677626115427[3] = 1;
   out_5906441677626115427[4] = 0;
   out_5906441677626115427[5] = 0;
   out_5906441677626115427[6] = 0;
   out_5906441677626115427[7] = 0;
   out_5906441677626115427[8] = 0;
}
void h_29(double *state, double *unused, double *out_1828878152614529315) {
   out_1828878152614529315[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3221447021511298332) {
   out_3221447021511298332[0] = 0;
   out_3221447021511298332[1] = 1;
   out_3221447021511298332[2] = 0;
   out_3221447021511298332[3] = 0;
   out_3221447021511298332[4] = 0;
   out_3221447021511298332[5] = 0;
   out_3221447021511298332[6] = 0;
   out_3221447021511298332[7] = 0;
   out_3221447021511298332[8] = 0;
}
void h_28(double *state, double *unused, double *out_826487282304194923) {
   out_826487282304194923[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8303846038580828906) {
   out_8303846038580828906[0] = 1;
   out_8303846038580828906[1] = 0;
   out_8303846038580828906[2] = 0;
   out_8303846038580828906[3] = 0;
   out_8303846038580828906[4] = 0;
   out_8303846038580828906[5] = 0;
   out_8303846038580828906[6] = 0;
   out_8303846038580828906[7] = 0;
   out_8303846038580828906[8] = 0;
}
void h_31(double *state, double *unused, double *out_3290802508979693074) {
   out_3290802508979693074[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7829021328269204773) {
   out_7829021328269204773[0] = 0;
   out_7829021328269204773[1] = 0;
   out_7829021328269204773[2] = 0;
   out_7829021328269204773[3] = 0;
   out_7829021328269204773[4] = 0;
   out_7829021328269204773[5] = 0;
   out_7829021328269204773[6] = 0;
   out_7829021328269204773[7] = 0;
   out_7829021328269204773[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_4585888895666118916) {
  err_fun(nom_x, delta_x, out_4585888895666118916);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_533703381334724490) {
  inv_err_fun(nom_x, true_x, out_533703381334724490);
}
void car_H_mod_fun(double *state, double *out_1455180481405070034) {
  H_mod_fun(state, out_1455180481405070034);
}
void car_f_fun(double *state, double dt, double *out_1890959688261970036) {
  f_fun(state,  dt, out_1890959688261970036);
}
void car_F_fun(double *state, double dt, double *out_1775561225881524917) {
  F_fun(state,  dt, out_1775561225881524917);
}
void car_h_25(double *state, double *unused, double *out_294409584145781914) {
  h_25(state, unused, out_294409584145781914);
}
void car_H_25(double *state, double *unused, double *out_7798375366392244345) {
  H_25(state, unused, out_7798375366392244345);
}
void car_h_24(double *state, double *unused, double *out_4279064028871003452) {
  h_24(state, unused, out_4279064028871003452);
}
void car_H_24(double *state, double *unused, double *out_8422660923338438709) {
  H_24(state, unused, out_8422660923338438709);
}
void car_h_30(double *state, double *unused, double *out_3172795115046316330) {
  h_30(state, unused, out_3172795115046316330);
}
void car_H_30(double *state, double *unused, double *out_3731678365825690516) {
  H_30(state, unused, out_3731678365825690516);
}
void car_h_26(double *state, double *unused, double *out_4652723219620663290) {
  h_26(state, unused, out_4652723219620663290);
}
void car_H_26(double *state, double *unused, double *out_8455229430502556249) {
  H_26(state, unused, out_8455229430502556249);
}
void car_h_27(double *state, double *unused, double *out_6052394498139519870) {
  h_27(state, unused, out_6052394498139519870);
}
void car_H_27(double *state, double *unused, double *out_5906441677626115427) {
  H_27(state, unused, out_5906441677626115427);
}
void car_h_29(double *state, double *unused, double *out_1828878152614529315) {
  h_29(state, unused, out_1828878152614529315);
}
void car_H_29(double *state, double *unused, double *out_3221447021511298332) {
  H_29(state, unused, out_3221447021511298332);
}
void car_h_28(double *state, double *unused, double *out_826487282304194923) {
  h_28(state, unused, out_826487282304194923);
}
void car_H_28(double *state, double *unused, double *out_8303846038580828906) {
  H_28(state, unused, out_8303846038580828906);
}
void car_h_31(double *state, double *unused, double *out_3290802508979693074) {
  h_31(state, unused, out_3290802508979693074);
}
void car_H_31(double *state, double *unused, double *out_7829021328269204773) {
  H_31(state, unused, out_7829021328269204773);
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
