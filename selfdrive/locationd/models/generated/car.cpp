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
void err_fun(double *nom_x, double *delta_x, double *out_5015521908408383086) {
   out_5015521908408383086[0] = delta_x[0] + nom_x[0];
   out_5015521908408383086[1] = delta_x[1] + nom_x[1];
   out_5015521908408383086[2] = delta_x[2] + nom_x[2];
   out_5015521908408383086[3] = delta_x[3] + nom_x[3];
   out_5015521908408383086[4] = delta_x[4] + nom_x[4];
   out_5015521908408383086[5] = delta_x[5] + nom_x[5];
   out_5015521908408383086[6] = delta_x[6] + nom_x[6];
   out_5015521908408383086[7] = delta_x[7] + nom_x[7];
   out_5015521908408383086[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6124113840288658246) {
   out_6124113840288658246[0] = -nom_x[0] + true_x[0];
   out_6124113840288658246[1] = -nom_x[1] + true_x[1];
   out_6124113840288658246[2] = -nom_x[2] + true_x[2];
   out_6124113840288658246[3] = -nom_x[3] + true_x[3];
   out_6124113840288658246[4] = -nom_x[4] + true_x[4];
   out_6124113840288658246[5] = -nom_x[5] + true_x[5];
   out_6124113840288658246[6] = -nom_x[6] + true_x[6];
   out_6124113840288658246[7] = -nom_x[7] + true_x[7];
   out_6124113840288658246[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_3707532353601669203) {
   out_3707532353601669203[0] = 1.0;
   out_3707532353601669203[1] = 0.0;
   out_3707532353601669203[2] = 0.0;
   out_3707532353601669203[3] = 0.0;
   out_3707532353601669203[4] = 0.0;
   out_3707532353601669203[5] = 0.0;
   out_3707532353601669203[6] = 0.0;
   out_3707532353601669203[7] = 0.0;
   out_3707532353601669203[8] = 0.0;
   out_3707532353601669203[9] = 0.0;
   out_3707532353601669203[10] = 1.0;
   out_3707532353601669203[11] = 0.0;
   out_3707532353601669203[12] = 0.0;
   out_3707532353601669203[13] = 0.0;
   out_3707532353601669203[14] = 0.0;
   out_3707532353601669203[15] = 0.0;
   out_3707532353601669203[16] = 0.0;
   out_3707532353601669203[17] = 0.0;
   out_3707532353601669203[18] = 0.0;
   out_3707532353601669203[19] = 0.0;
   out_3707532353601669203[20] = 1.0;
   out_3707532353601669203[21] = 0.0;
   out_3707532353601669203[22] = 0.0;
   out_3707532353601669203[23] = 0.0;
   out_3707532353601669203[24] = 0.0;
   out_3707532353601669203[25] = 0.0;
   out_3707532353601669203[26] = 0.0;
   out_3707532353601669203[27] = 0.0;
   out_3707532353601669203[28] = 0.0;
   out_3707532353601669203[29] = 0.0;
   out_3707532353601669203[30] = 1.0;
   out_3707532353601669203[31] = 0.0;
   out_3707532353601669203[32] = 0.0;
   out_3707532353601669203[33] = 0.0;
   out_3707532353601669203[34] = 0.0;
   out_3707532353601669203[35] = 0.0;
   out_3707532353601669203[36] = 0.0;
   out_3707532353601669203[37] = 0.0;
   out_3707532353601669203[38] = 0.0;
   out_3707532353601669203[39] = 0.0;
   out_3707532353601669203[40] = 1.0;
   out_3707532353601669203[41] = 0.0;
   out_3707532353601669203[42] = 0.0;
   out_3707532353601669203[43] = 0.0;
   out_3707532353601669203[44] = 0.0;
   out_3707532353601669203[45] = 0.0;
   out_3707532353601669203[46] = 0.0;
   out_3707532353601669203[47] = 0.0;
   out_3707532353601669203[48] = 0.0;
   out_3707532353601669203[49] = 0.0;
   out_3707532353601669203[50] = 1.0;
   out_3707532353601669203[51] = 0.0;
   out_3707532353601669203[52] = 0.0;
   out_3707532353601669203[53] = 0.0;
   out_3707532353601669203[54] = 0.0;
   out_3707532353601669203[55] = 0.0;
   out_3707532353601669203[56] = 0.0;
   out_3707532353601669203[57] = 0.0;
   out_3707532353601669203[58] = 0.0;
   out_3707532353601669203[59] = 0.0;
   out_3707532353601669203[60] = 1.0;
   out_3707532353601669203[61] = 0.0;
   out_3707532353601669203[62] = 0.0;
   out_3707532353601669203[63] = 0.0;
   out_3707532353601669203[64] = 0.0;
   out_3707532353601669203[65] = 0.0;
   out_3707532353601669203[66] = 0.0;
   out_3707532353601669203[67] = 0.0;
   out_3707532353601669203[68] = 0.0;
   out_3707532353601669203[69] = 0.0;
   out_3707532353601669203[70] = 1.0;
   out_3707532353601669203[71] = 0.0;
   out_3707532353601669203[72] = 0.0;
   out_3707532353601669203[73] = 0.0;
   out_3707532353601669203[74] = 0.0;
   out_3707532353601669203[75] = 0.0;
   out_3707532353601669203[76] = 0.0;
   out_3707532353601669203[77] = 0.0;
   out_3707532353601669203[78] = 0.0;
   out_3707532353601669203[79] = 0.0;
   out_3707532353601669203[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5658290391619995454) {
   out_5658290391619995454[0] = state[0];
   out_5658290391619995454[1] = state[1];
   out_5658290391619995454[2] = state[2];
   out_5658290391619995454[3] = state[3];
   out_5658290391619995454[4] = state[4];
   out_5658290391619995454[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5658290391619995454[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5658290391619995454[7] = state[7];
   out_5658290391619995454[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6882967969335663215) {
   out_6882967969335663215[0] = 1;
   out_6882967969335663215[1] = 0;
   out_6882967969335663215[2] = 0;
   out_6882967969335663215[3] = 0;
   out_6882967969335663215[4] = 0;
   out_6882967969335663215[5] = 0;
   out_6882967969335663215[6] = 0;
   out_6882967969335663215[7] = 0;
   out_6882967969335663215[8] = 0;
   out_6882967969335663215[9] = 0;
   out_6882967969335663215[10] = 1;
   out_6882967969335663215[11] = 0;
   out_6882967969335663215[12] = 0;
   out_6882967969335663215[13] = 0;
   out_6882967969335663215[14] = 0;
   out_6882967969335663215[15] = 0;
   out_6882967969335663215[16] = 0;
   out_6882967969335663215[17] = 0;
   out_6882967969335663215[18] = 0;
   out_6882967969335663215[19] = 0;
   out_6882967969335663215[20] = 1;
   out_6882967969335663215[21] = 0;
   out_6882967969335663215[22] = 0;
   out_6882967969335663215[23] = 0;
   out_6882967969335663215[24] = 0;
   out_6882967969335663215[25] = 0;
   out_6882967969335663215[26] = 0;
   out_6882967969335663215[27] = 0;
   out_6882967969335663215[28] = 0;
   out_6882967969335663215[29] = 0;
   out_6882967969335663215[30] = 1;
   out_6882967969335663215[31] = 0;
   out_6882967969335663215[32] = 0;
   out_6882967969335663215[33] = 0;
   out_6882967969335663215[34] = 0;
   out_6882967969335663215[35] = 0;
   out_6882967969335663215[36] = 0;
   out_6882967969335663215[37] = 0;
   out_6882967969335663215[38] = 0;
   out_6882967969335663215[39] = 0;
   out_6882967969335663215[40] = 1;
   out_6882967969335663215[41] = 0;
   out_6882967969335663215[42] = 0;
   out_6882967969335663215[43] = 0;
   out_6882967969335663215[44] = 0;
   out_6882967969335663215[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6882967969335663215[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6882967969335663215[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6882967969335663215[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6882967969335663215[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6882967969335663215[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6882967969335663215[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6882967969335663215[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6882967969335663215[53] = -9.8000000000000007*dt;
   out_6882967969335663215[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6882967969335663215[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6882967969335663215[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6882967969335663215[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6882967969335663215[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6882967969335663215[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6882967969335663215[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6882967969335663215[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6882967969335663215[62] = 0;
   out_6882967969335663215[63] = 0;
   out_6882967969335663215[64] = 0;
   out_6882967969335663215[65] = 0;
   out_6882967969335663215[66] = 0;
   out_6882967969335663215[67] = 0;
   out_6882967969335663215[68] = 0;
   out_6882967969335663215[69] = 0;
   out_6882967969335663215[70] = 1;
   out_6882967969335663215[71] = 0;
   out_6882967969335663215[72] = 0;
   out_6882967969335663215[73] = 0;
   out_6882967969335663215[74] = 0;
   out_6882967969335663215[75] = 0;
   out_6882967969335663215[76] = 0;
   out_6882967969335663215[77] = 0;
   out_6882967969335663215[78] = 0;
   out_6882967969335663215[79] = 0;
   out_6882967969335663215[80] = 1;
}
void h_25(double *state, double *unused, double *out_1123652428534175966) {
   out_1123652428534175966[0] = state[6];
}
void H_25(double *state, double *unused, double *out_6817873687134022608) {
   out_6817873687134022608[0] = 0;
   out_6817873687134022608[1] = 0;
   out_6817873687134022608[2] = 0;
   out_6817873687134022608[3] = 0;
   out_6817873687134022608[4] = 0;
   out_6817873687134022608[5] = 0;
   out_6817873687134022608[6] = 1;
   out_6817873687134022608[7] = 0;
   out_6817873687134022608[8] = 0;
}
void h_24(double *state, double *unused, double *out_3477721535892924344) {
   out_3477721535892924344[0] = state[4];
   out_3477721535892924344[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3259948088331699472) {
   out_3259948088331699472[0] = 0;
   out_3259948088331699472[1] = 0;
   out_3259948088331699472[2] = 0;
   out_3259948088331699472[3] = 0;
   out_3259948088331699472[4] = 1;
   out_3259948088331699472[5] = 0;
   out_3259948088331699472[6] = 0;
   out_3259948088331699472[7] = 0;
   out_3259948088331699472[8] = 0;
   out_3259948088331699472[9] = 0;
   out_3259948088331699472[10] = 0;
   out_3259948088331699472[11] = 0;
   out_3259948088331699472[12] = 0;
   out_3259948088331699472[13] = 0;
   out_3259948088331699472[14] = 1;
   out_3259948088331699472[15] = 0;
   out_3259948088331699472[16] = 0;
   out_3259948088331699472[17] = 0;
}
void h_30(double *state, double *unused, double *out_8826430963911635658) {
   out_8826430963911635658[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4299540728626773981) {
   out_4299540728626773981[0] = 0;
   out_4299540728626773981[1] = 0;
   out_4299540728626773981[2] = 0;
   out_4299540728626773981[3] = 0;
   out_4299540728626773981[4] = 1;
   out_4299540728626773981[5] = 0;
   out_4299540728626773981[6] = 0;
   out_4299540728626773981[7] = 0;
   out_4299540728626773981[8] = 0;
}
void h_26(double *state, double *unused, double *out_5550583151164179069) {
   out_5550583151164179069[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7887367067701472784) {
   out_7887367067701472784[0] = 0;
   out_7887367067701472784[1] = 0;
   out_7887367067701472784[2] = 0;
   out_7887367067701472784[3] = 0;
   out_7887367067701472784[4] = 0;
   out_7887367067701472784[5] = 0;
   out_7887367067701472784[6] = 0;
   out_7887367067701472784[7] = 1;
   out_7887367067701472784[8] = 0;
}
void h_27(double *state, double *unused, double *out_2067162281435838401) {
   out_2067162281435838401[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4926410744647495899) {
   out_4926410744647495899[0] = 0;
   out_4926410744647495899[1] = 0;
   out_4926410744647495899[2] = 0;
   out_4926410744647495899[3] = 1;
   out_4926410744647495899[4] = 0;
   out_4926410744647495899[5] = 0;
   out_4926410744647495899[6] = 0;
   out_4926410744647495899[7] = 0;
   out_4926410744647495899[8] = 0;
}
void h_29(double *state, double *unused, double *out_7739704315555171331) {
   out_7739704315555171331[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7611405400762312994) {
   out_7611405400762312994[0] = 0;
   out_7611405400762312994[1] = 1;
   out_7611405400762312994[2] = 0;
   out_7611405400762312994[3] = 0;
   out_7611405400762312994[4] = 0;
   out_7611405400762312994[5] = 0;
   out_7611405400762312994[6] = 0;
   out_7611405400762312994[7] = 0;
   out_7611405400762312994[8] = 0;
}
void h_28(double *state, double *unused, double *out_5103948332880171086) {
   out_5103948332880171086[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8871708401381912371) {
   out_8871708401381912371[0] = 1;
   out_8871708401381912371[1] = 0;
   out_8871708401381912371[2] = 0;
   out_8871708401381912371[3] = 0;
   out_8871708401381912371[4] = 0;
   out_8871708401381912371[5] = 0;
   out_8871708401381912371[6] = 0;
   out_8871708401381912371[7] = 0;
   out_8871708401381912371[8] = 0;
}
void h_31(double *state, double *unused, double *out_8090967593215208460) {
   out_8090967593215208460[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7261158965468121308) {
   out_7261158965468121308[0] = 0;
   out_7261158965468121308[1] = 0;
   out_7261158965468121308[2] = 0;
   out_7261158965468121308[3] = 0;
   out_7261158965468121308[4] = 0;
   out_7261158965468121308[5] = 0;
   out_7261158965468121308[6] = 0;
   out_7261158965468121308[7] = 0;
   out_7261158965468121308[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_5015521908408383086) {
  err_fun(nom_x, delta_x, out_5015521908408383086);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6124113840288658246) {
  inv_err_fun(nom_x, true_x, out_6124113840288658246);
}
void car_H_mod_fun(double *state, double *out_3707532353601669203) {
  H_mod_fun(state, out_3707532353601669203);
}
void car_f_fun(double *state, double dt, double *out_5658290391619995454) {
  f_fun(state,  dt, out_5658290391619995454);
}
void car_F_fun(double *state, double dt, double *out_6882967969335663215) {
  F_fun(state,  dt, out_6882967969335663215);
}
void car_h_25(double *state, double *unused, double *out_1123652428534175966) {
  h_25(state, unused, out_1123652428534175966);
}
void car_H_25(double *state, double *unused, double *out_6817873687134022608) {
  H_25(state, unused, out_6817873687134022608);
}
void car_h_24(double *state, double *unused, double *out_3477721535892924344) {
  h_24(state, unused, out_3477721535892924344);
}
void car_H_24(double *state, double *unused, double *out_3259948088331699472) {
  H_24(state, unused, out_3259948088331699472);
}
void car_h_30(double *state, double *unused, double *out_8826430963911635658) {
  h_30(state, unused, out_8826430963911635658);
}
void car_H_30(double *state, double *unused, double *out_4299540728626773981) {
  H_30(state, unused, out_4299540728626773981);
}
void car_h_26(double *state, double *unused, double *out_5550583151164179069) {
  h_26(state, unused, out_5550583151164179069);
}
void car_H_26(double *state, double *unused, double *out_7887367067701472784) {
  H_26(state, unused, out_7887367067701472784);
}
void car_h_27(double *state, double *unused, double *out_2067162281435838401) {
  h_27(state, unused, out_2067162281435838401);
}
void car_H_27(double *state, double *unused, double *out_4926410744647495899) {
  H_27(state, unused, out_4926410744647495899);
}
void car_h_29(double *state, double *unused, double *out_7739704315555171331) {
  h_29(state, unused, out_7739704315555171331);
}
void car_H_29(double *state, double *unused, double *out_7611405400762312994) {
  H_29(state, unused, out_7611405400762312994);
}
void car_h_28(double *state, double *unused, double *out_5103948332880171086) {
  h_28(state, unused, out_5103948332880171086);
}
void car_H_28(double *state, double *unused, double *out_8871708401381912371) {
  H_28(state, unused, out_8871708401381912371);
}
void car_h_31(double *state, double *unused, double *out_8090967593215208460) {
  h_31(state, unused, out_8090967593215208460);
}
void car_H_31(double *state, double *unused, double *out_7261158965468121308) {
  H_31(state, unused, out_7261158965468121308);
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
