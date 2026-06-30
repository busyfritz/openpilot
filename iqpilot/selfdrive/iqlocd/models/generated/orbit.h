#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void orbit_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void orbit_H(double *in_vec, double *out_164077483659531076);
void orbit_err_fun(double *nom_x, double *delta_x, double *out_1459893306920135382);
void orbit_inv_err_fun(double *nom_x, double *true_x, double *out_843320162087724215);
void orbit_H_mod_fun(double *state, double *out_4666327563387133445);
void orbit_f_fun(double *state, double dt, double *out_2882145218265446966);
void orbit_F_fun(double *state, double dt, double *out_6362037949835617217);
void orbit_h_4(double *state, double *unused, double *out_2656201241305838254);
void orbit_H_4(double *state, double *unused, double *out_7920702571485236508);
void orbit_h_9(double *state, double *unused, double *out_4790980704302709136);
void orbit_H_9(double *state, double *unused, double *out_3238822566959867638);
void orbit_h_10(double *state, double *unused, double *out_9131648242454347110);
void orbit_H_10(double *state, double *unused, double *out_6410720850645248374);
void orbit_h_12(double *state, double *unused, double *out_925747894630456424);
void orbit_H_12(double *state, double *unused, double *out_2858913188541864616);
void orbit_h_35(double *state, double *unused, double *out_56993372431175208);
void orbit_H_35(double *state, double *unused, double *out_7159379444851707732);
void orbit_h_32(double *state, double *unused, double *out_2022442368957051596);
void orbit_H_32(double *state, double *unused, double *out_5218541107518698931);
void orbit_h_13(double *state, double *unused, double *out_2682992055017160560);
void orbit_H_13(double *state, double *unused, double *out_959037752515567077);
void orbit_h_14(double *state, double *unused, double *out_4790980704302709136);
void orbit_H_14(double *state, double *unused, double *out_3238822566959867638);
void orbit_h_33(double *state, double *unused, double *out_1050211698996295091);
void orbit_H_33(double *state, double *unused, double *out_3037206848422006697);
void orbit_predict(double *in_x, double *in_P, double *in_Q, double dt);
}