#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2613128383328169093);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2805699213723397762);
void pose_H_mod_fun(double *state, double *out_4356096610437586896);
void pose_f_fun(double *state, double dt, double *out_838605051751895038);
void pose_F_fun(double *state, double dt, double *out_4469587873875989476);
void pose_h_4(double *state, double *unused, double *out_3292015623268015671);
void pose_H_4(double *state, double *unused, double *out_8242565656520005999);
void pose_h_10(double *state, double *unused, double *out_3738960453134783073);
void pose_H_10(double *state, double *unused, double *out_1772545011172825092);
void pose_h_13(double *state, double *unused, double *out_2179608341068223281);
void pose_H_13(double *state, double *unused, double *out_6991904591857212816);
void pose_h_14(double *state, double *unused, double *out_3603750172356241210);
void pose_H_14(double *state, double *unused, double *out_6240937560850061088);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}