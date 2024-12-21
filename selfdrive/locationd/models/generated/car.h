#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_1490120506809532240);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3620533529372427460);
void car_H_mod_fun(double *state, double *out_5726274843943393527);
void car_f_fun(double *state, double dt, double *out_4807164323551011322);
void car_F_fun(double *state, double dt, double *out_2444431364812404128);
void car_h_25(double *state, double *unused, double *out_2954679385006955802);
void car_H_25(double *state, double *unused, double *out_822583018758728687);
void car_h_24(double *state, double *unused, double *out_8497828521175169741);
void car_H_24(double *state, double *unused, double *out_5752988787832789414);
void car_h_30(double *state, double *unused, double *out_569358957711143271);
void car_H_30(double *state, double *unused, double *out_6094107322732888068);
void car_h_26(double *state, double *unused, double *out_6968400475124992417);
void car_H_26(double *state, double *unused, double *out_4564086337632784911);
void car_h_27(double *state, double *unused, double *out_7086407869058369161);
void car_H_27(double *state, double *unused, double *out_3919344010932463157);
void car_h_29(double *state, double *unused, double *out_2051309415625482021);
void car_H_29(double *state, double *unused, double *out_6604338667047280252);
void car_h_28(double *state, double *unused, double *out_6358662936990336810);
void car_H_28(double *state, double *unused, double *out_2876417733006618450);
void car_h_31(double *state, double *unused, double *out_8598744215491355659);
void car_H_31(double *state, double *unused, double *out_791937056881768259);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}