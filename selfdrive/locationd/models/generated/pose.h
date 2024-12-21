#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8138643469158452515);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4486085605069433673);
void pose_H_mod_fun(double *state, double *out_4128386777611714386);
void pose_f_fun(double *state, double dt, double *out_3926695733139875668);
void pose_F_fun(double *state, double dt, double *out_3790507139778411869);
void pose_h_4(double *state, double *unused, double *out_7316226510605005089);
void pose_H_4(double *state, double *unused, double *out_3374225775557001779);
void pose_h_10(double *state, double *unused, double *out_2540837594140349822);
void pose_H_10(double *state, double *unused, double *out_6682096001196788588);
void pose_h_13(double *state, double *unused, double *out_1446024959322562397);
void pose_H_13(double *state, double *unused, double *out_161951950224668978);
void pose_h_14(double *state, double *unused, double *out_7758273995292918031);
void pose_H_14(double *state, double *unused, double *out_6457014207852374075);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}