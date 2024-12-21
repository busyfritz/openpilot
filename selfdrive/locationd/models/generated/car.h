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
void car_err_fun(double *nom_x, double *delta_x, double *out_4585888895666118916);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_533703381334724490);
void car_H_mod_fun(double *state, double *out_1455180481405070034);
void car_f_fun(double *state, double dt, double *out_1890959688261970036);
void car_F_fun(double *state, double dt, double *out_1775561225881524917);
void car_h_25(double *state, double *unused, double *out_294409584145781914);
void car_H_25(double *state, double *unused, double *out_7798375366392244345);
void car_h_24(double *state, double *unused, double *out_4279064028871003452);
void car_H_24(double *state, double *unused, double *out_8422660923338438709);
void car_h_30(double *state, double *unused, double *out_3172795115046316330);
void car_H_30(double *state, double *unused, double *out_3731678365825690516);
void car_h_26(double *state, double *unused, double *out_4652723219620663290);
void car_H_26(double *state, double *unused, double *out_8455229430502556249);
void car_h_27(double *state, double *unused, double *out_6052394498139519870);
void car_H_27(double *state, double *unused, double *out_5906441677626115427);
void car_h_29(double *state, double *unused, double *out_1828878152614529315);
void car_H_29(double *state, double *unused, double *out_3221447021511298332);
void car_h_28(double *state, double *unused, double *out_826487282304194923);
void car_H_28(double *state, double *unused, double *out_8303846038580828906);
void car_h_31(double *state, double *unused, double *out_3290802508979693074);
void car_H_31(double *state, double *unused, double *out_7829021328269204773);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}