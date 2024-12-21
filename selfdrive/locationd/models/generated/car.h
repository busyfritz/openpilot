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
void car_err_fun(double *nom_x, double *delta_x, double *out_5015521908408383086);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6124113840288658246);
void car_H_mod_fun(double *state, double *out_3707532353601669203);
void car_f_fun(double *state, double dt, double *out_5658290391619995454);
void car_F_fun(double *state, double dt, double *out_6882967969335663215);
void car_h_25(double *state, double *unused, double *out_1123652428534175966);
void car_H_25(double *state, double *unused, double *out_6817873687134022608);
void car_h_24(double *state, double *unused, double *out_3477721535892924344);
void car_H_24(double *state, double *unused, double *out_3259948088331699472);
void car_h_30(double *state, double *unused, double *out_8826430963911635658);
void car_H_30(double *state, double *unused, double *out_4299540728626773981);
void car_h_26(double *state, double *unused, double *out_5550583151164179069);
void car_H_26(double *state, double *unused, double *out_7887367067701472784);
void car_h_27(double *state, double *unused, double *out_2067162281435838401);
void car_H_27(double *state, double *unused, double *out_4926410744647495899);
void car_h_29(double *state, double *unused, double *out_7739704315555171331);
void car_H_29(double *state, double *unused, double *out_7611405400762312994);
void car_h_28(double *state, double *unused, double *out_5103948332880171086);
void car_H_28(double *state, double *unused, double *out_8871708401381912371);
void car_h_31(double *state, double *unused, double *out_8090967593215208460);
void car_H_31(double *state, double *unused, double *out_7261158965468121308);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}