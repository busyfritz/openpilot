#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7449750869360596060);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9017488319834403806);
void pose_H_mod_fun(double *state, double *out_5161440433737183966);
void pose_f_fun(double *state, double dt, double *out_4374988147753332159);
void pose_F_fun(double *state, double dt, double *out_4799445358667755900);
void pose_h_4(double *state, double *unused, double *out_5495389517486282336);
void pose_H_4(double *state, double *unused, double *out_3963492951854285862);
void pose_h_10(double *state, double *unused, double *out_3796979241642905601);
void pose_H_10(double *state, double *unused, double *out_2798195339350489049);
void pose_h_13(double *state, double *unused, double *out_2405306930266407283);
void pose_H_13(double *state, double *unused, double *out_751219126521953061);
void pose_h_14(double *state, double *unused, double *out_6937523326105035085);
void pose_H_14(double *state, double *unused, double *out_252095514801333);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}