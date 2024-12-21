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
void car_err_fun(double *nom_x, double *delta_x, double *out_4482578333221659286);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_9124397069198269285);
void car_H_mod_fun(double *state, double *out_4065454713260201204);
void car_f_fun(double *state, double dt, double *out_4421941963978299719);
void car_F_fun(double *state, double dt, double *out_147831669424064165);
void car_h_25(double *state, double *unused, double *out_6653270805194347831);
void car_H_25(double *state, double *unused, double *out_5127733512652036033);
void car_h_24(double *state, double *unused, double *out_2326587204835148486);
void car_H_24(double *state, double *unused, double *out_683652118370740932);
void car_h_30(double *state, double *unused, double *out_2785430221895203521);
void car_H_30(double *state, double *unused, double *out_2609400554144787406);
void car_h_26(double *state, double *unused, double *out_5389835542703901172);
void car_H_26(double *state, double *unused, double *out_1823207542891235432);
void car_h_27(double *state, double *unused, double *out_2974847871730134823);
void car_H_27(double *state, double *unused, double *out_385806482960844189);
void car_h_29(double *state, double *unused, double *out_8652394041150639658);
void car_H_29(double *state, double *unused, double *out_2099169209830395222);
void car_h_28(double *state, double *unused, double *out_2170042191385512714);
void car_H_28(double *state, double *unused, double *out_7181568226899925796);
void car_h_31(double *state, double *unused, double *out_213255882570308027);
void car_H_31(double *state, double *unused, double *out_1948941737859781220);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}