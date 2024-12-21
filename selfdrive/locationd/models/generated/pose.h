#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8396481004364689900);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6933496826796386374);
void pose_H_mod_fun(double *state, double *out_2065338275713658617);
void pose_f_fun(double *state, double dt, double *out_47123365585028398);
void pose_F_fun(double *state, double dt, double *out_3459660374911670949);
void pose_h_4(double *state, double *unused, double *out_3884991371756977501);
void pose_H_4(double *state, double *unused, double *out_7913420082465617338);
void pose_h_10(double *state, double *unused, double *out_6800379684859322945);
void pose_H_10(double *state, double *unused, double *out_8355631585322469337);
void pose_h_13(double *state, double *unused, double *out_19955561701487762);
void pose_H_13(double *state, double *unused, double *out_4701146257133284537);
void pose_h_14(double *state, double *unused, double *out_5485320499311477942);
void pose_H_14(double *state, double *unused, double *out_3950179226126132809);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}