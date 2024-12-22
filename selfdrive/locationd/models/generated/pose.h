#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_5204449900680294893);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3166247841107463599);
void pose_H_mod_fun(double *state, double *out_4757463840714288630);
void pose_f_fun(double *state, double dt, double *out_1433841035840211);
void pose_F_fun(double *state, double dt, double *out_3040199541343879647);
void pose_h_4(double *state, double *unused, double *out_942367839052342030);
void pose_H_4(double *state, double *unused, double *out_4003302838659576023);
void pose_h_10(double *state, double *unused, double *out_8571903349869224763);
void pose_H_10(double *state, double *unused, double *out_3736312142713721773);
void pose_h_13(double *state, double *unused, double *out_2382082840634836033);
void pose_H_13(double *state, double *unused, double *out_3607328369657124906);
void pose_h_14(double *state, double *unused, double *out_4846369707862575646);
void pose_H_14(double *state, double *unused, double *out_7086091270954948319);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}