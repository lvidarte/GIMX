/*
 Copyright (c) 2015 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#ifndef G920_XONE_H_
#define G920_XONE_H_

#include <defs.h>

typedef enum
{
  g920Xonea_wheel = rel_axis_lstick_x,

  g920Xonea_view = abs_axis_0,
  g920Xonea_menu = abs_axis_1,
  g920Xonea_guide = abs_axis_2,
  g920Xonea_up = abs_axis_3,
  g920Xonea_right = abs_axis_4,
  g920Xonea_down = abs_axis_5,
  g920Xonea_left = abs_axis_6,
  g920Xonea_Y = abs_axis_7,
  g920Xonea_B = abs_axis_8,
  g920Xonea_A = abs_axis_9,
  g920Xonea_X = abs_axis_10,
  g920Xonea_LB = abs_axis_11,
  g920Xonea_RB = abs_axis_12,
  g920Xonea_LT = abs_axis_13,
  g920Xonea_RT = abs_axis_14,
  g920Xonea_LS = abs_axis_15,
  g920Xonea_RS = abs_axis_16,

  g920Xonea_gasPedal = abs_axis_17,
  g920Xonea_brakePedal = abs_axis_18,
  g920Xonea_clutchPedal = abs_axis_19,

  g920Xonea_gearShifter1 = abs_axis_20,
  g920Xonea_gearShifter2 = abs_axis_21,
  g920Xonea_gearShifter3 = abs_axis_22,
  g920Xonea_gearShifter4 = abs_axis_23,
  g920Xonea_gearShifter5 = abs_axis_24,
  g920Xonea_gearShifter6 = abs_axis_25,
  g920Xonea_gearShifterR = abs_axis_26,

} e_g920Xone_axis_index;

/*
 *
 */
typedef struct GIMX_PACKED
{
  unsigned char reportId; //0x01
  unsigned char hatAndButtons;
  unsigned char buttons1;
  unsigned char buttons2; //2 MSB are unused
  unsigned short wheel;
  unsigned char gas;
  unsigned char brake;
  unsigned char clutch;
  unsigned char buttons3;
} s_report_g920Xone;

#endif /* G920_XONE_H_ */
