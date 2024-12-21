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
void car_err_fun(double *nom_x, double *delta_x, double *out_2770383136982745722);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1833603901144997724);
void car_H_mod_fun(double *state, double *out_4081607929894336817);
void car_f_fun(double *state, double dt, double *out_8331951914791866149);
void car_F_fun(double *state, double dt, double *out_5109431088893287780);
void car_h_25(double *state, double *unused, double *out_1573818349272192953);
void car_H_25(double *state, double *unused, double *out_5171947917902977562);
void car_h_24(double *state, double *unused, double *out_1070798443338542178);
void car_H_24(double *state, double *unused, double *out_2999298318897477996);
void car_h_30(double *state, double *unused, double *out_2287865127156308699);
void car_H_30(double *state, double *unused, double *out_6358105814314957299);
void car_h_26(double *state, double *unused, double *out_2778552391562034024);
void car_H_26(double *state, double *unused, double *out_1430444599028921338);
void car_h_27(double *state, double *unused, double *out_8740169787446037511);
void car_H_27(double *state, double *unused, double *out_8532869126115382210);
void car_h_29(double *state, double *unused, double *out_7289538394817154344);
void car_H_29(double *state, double *unused, double *out_8200512220724618373);
void car_h_28(double *state, double *unused, double *out_6301052235452206598);
void car_H_28(double *state, double *unused, double *out_3118113203655087799);
void car_h_31(double *state, double *unused, double *out_7937412327574008814);
void car_H_31(double *state, double *unused, double *out_5202593879779937990);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}