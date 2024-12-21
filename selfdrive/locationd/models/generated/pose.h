#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_4278397644236826764);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_8989697841247581240);
void pose_H_mod_fun(double *state, double *out_2984951409191049856);
void pose_f_fun(double *state, double dt, double *out_5963344649312491371);
void pose_F_fun(double *state, double dt, double *out_4433429895516971321);
void pose_h_4(double *state, double *unused, double *out_8370372608422906031);
void pose_H_4(double *state, double *unused, double *out_3739112411245762463);
void pose_h_10(double *state, double *unused, double *out_1951244009672801524);
void pose_H_10(double *state, double *unused, double *out_431513790114911654);
void pose_h_13(double *state, double *unused, double *out_6412097572544315454);
void pose_H_13(double *state, double *unused, double *out_6951386236578095264);
void pose_h_14(double *state, double *unused, double *out_5552192962493310549);
void pose_H_14(double *state, double *unused, double *out_656323978950390167);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}