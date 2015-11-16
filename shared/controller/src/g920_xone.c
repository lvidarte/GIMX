/*
 Copyright (c) 2015 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#include <g920_xone.h>
#include <report.h>
#include <controller2.h>
#include <string.h>

#define G920_X_MASK 0x10
#define G920_A_MASK 0x20
#define G920_B_MASK 0x40
#define G920_Y_MASK 0x80

#define G920_LB_MASK   0x01
#define G920_RB_MASK   0x02
#define G920_LT_MASK   0x04
#define G920_RT_MASK   0x08

#define G920_VIEW_MASK 0x10
#define G920_MENU_MASK 0x20
#define G920_LS_MASK   0x40
#define G920_RS_MASK   0x80

#define G920_GUIDE_MASK 0x01

#define G920_GEAR_SHIFTER_1_MASK  0x01
#define G920_GEAR_SHIFTER_2_MASK  0x02
#define G920_GEAR_SHIFTER_3_MASK  0x04
#define G920_GEAR_SHIFTER_4_MASK  0x08
#define G920_GEAR_SHIFTER_5_MASK  0x10
#define G920_GEAR_SHIFTER_6_MASK  0x20
#define G920_GEAR_SHIFTER_R_MASK  0x80

static s_axis axes[AXIS_MAX] =
{
  [g920Xonea_wheel]       = { .name = "wheel",    .max_unsigned_value = MAX_AXIS_VALUE_16BITS },
  
  [g920Xonea_gasPedal]    = { .name = "gas",      .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_brakePedal]  = { .name = "brake",    .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_clutchPedal] = { .name = "clutch",   .max_unsigned_value = MAX_AXIS_VALUE_8BITS },

  [g920Xonea_view]        = { .name = "share",    .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_menu]        = { .name = "options",  .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_guide]       = { .name = "PS",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },

  [g920Xonea_up]          = { .name = "up",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_right]       = { .name = "right",    .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_down]        = { .name = "down",     .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_left]        = { .name = "left",     .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_Y]           = { .name = "Y",        .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_B]           = { .name = "B",        .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_A]           = { .name = "A",        .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_X]           = { .name = "X",        .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_LB]          = { .name = "LB",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_RB]          = { .name = "RB",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_LT]          = { .name = "LT",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_RT]          = { .name = "RT",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_LS]          = { .name = "LS",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_RS]          = { .name = "RS",       .max_unsigned_value = MAX_AXIS_VALUE_8BITS },

  [g920Xonea_gearShifter1] = {.name = "gear shifter 1", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_gearShifter2] = {.name = "gear shifter 2", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_gearShifter3] = {.name = "gear shifter 3", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_gearShifter4] = {.name = "gear shifter 4", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_gearShifter5] = {.name = "gear shifter 5", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_gearShifter6] = {.name = "gear shifter 6", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
  [g920Xonea_gearShifterR] = {.name = "gear shifter R", .max_unsigned_value = MAX_AXIS_VALUE_8BITS },
};

static s_axis_name_dir axis_name_dirs[] =
{
  {.name = "wheel",        {.axis = g920Xonea_wheel,      .props = AXIS_PROP_CENTERED}},

  {.name = "gas",          {.axis = g920Xonea_gasPedal,   .props = AXIS_PROP_POSITIVE}},
  {.name = "brake",        {.axis = g920Xonea_brakePedal, .props = AXIS_PROP_POSITIVE}},
  {.name = "clutch",       {.axis = g920Xonea_clutchPedal,.props = AXIS_PROP_POSITIVE}},

  {.name = "RT",           {.axis = g920Xonea_RT,         .props = AXIS_PROP_TOGGLE}},
  {.name = "LT",           {.axis = g920Xonea_LT,         .props = AXIS_PROP_TOGGLE}},

  {.name = "view",         {.axis = g920Xonea_view,       .props = AXIS_PROP_TOGGLE}},
  {.name = "menu",         {.axis = g920Xonea_menu,       .props = AXIS_PROP_TOGGLE}},
  {.name = "guide",        {.axis = g920Xonea_guide,      .props = AXIS_PROP_TOGGLE}},
  {.name = "up",           {.axis = g920Xonea_up,         .props = AXIS_PROP_TOGGLE}},
  {.name = "down",         {.axis = g920Xonea_down,       .props = AXIS_PROP_TOGGLE}},
  {.name = "right",        {.axis = g920Xonea_right,      .props = AXIS_PROP_TOGGLE}},
  {.name = "left",         {.axis = g920Xonea_left,       .props = AXIS_PROP_TOGGLE}},
  {.name = "RB",           {.axis = g920Xonea_RB,         .props = AXIS_PROP_TOGGLE}},
  {.name = "RS",           {.axis = g920Xonea_RS,         .props = AXIS_PROP_TOGGLE}},
  {.name = "LB",           {.axis = g920Xonea_LB,         .props = AXIS_PROP_TOGGLE}},
  {.name = "LS",           {.axis = g920Xonea_LS,         .props = AXIS_PROP_TOGGLE}},
  {.name = "A",            {.axis = g920Xonea_A,          .props = AXIS_PROP_TOGGLE}},
  {.name = "B",            {.axis = g920Xonea_B,          .props = AXIS_PROP_TOGGLE}},
  {.name = "X",            {.axis = g920Xonea_X,          .props = AXIS_PROP_TOGGLE}},
  {.name = "Y",            {.axis = g920Xonea_Y,          .props = AXIS_PROP_TOGGLE}},

  {.name = "gear shifter 1",    {.axis = g920Xonea_gearShifter1,   .props = AXIS_PROP_TOGGLE}},
  {.name = "gear shifter 2",    {.axis = g920Xonea_gearShifter2,   .props = AXIS_PROP_TOGGLE}},
  {.name = "gear shifter 3",    {.axis = g920Xonea_gearShifter3,   .props = AXIS_PROP_TOGGLE}},
  {.name = "gear shifter 4",    {.axis = g920Xonea_gearShifter4,   .props = AXIS_PROP_TOGGLE}},
  {.name = "gear shifter 5",    {.axis = g920Xonea_gearShifter5,   .props = AXIS_PROP_TOGGLE}},
  {.name = "gear shifter 6",    {.axis = g920Xonea_gearShifter6,   .props = AXIS_PROP_TOGGLE}},
  {.name = "gear shifter R",    {.axis = g920Xonea_gearShifterR,   .props = AXIS_PROP_TOGGLE}},
};

static s_report_g920Xone default_report =
{
  .reportId = 0x01,
  .hatAndButtons = 0x08,
  .buttons1 = 0x00,
  .buttons2 = 0x00,
  .wheel = CENTER_AXIS_VALUE_16BITS,
  .gas = MAX_AXIS_VALUE_8BITS,
  .brake = MAX_AXIS_VALUE_8BITS,
  .clutch = MAX_AXIS_VALUE_8BITS,
  .buttons3 = 0x00,
};

static void init_report(s_report * report)
{
  memcpy(report, &default_report, sizeof(default_report));
}

/*
 * Work in progress...
 * Do not assume the code in the following function is right!
 */
static unsigned int build_report(int axis[AXIS_MAX], s_report_packet report[MAX_REPORTS])
{
  unsigned int index = 0;
  report[index].length = sizeof(s_report_g920Xone);
  s_report_g920Xone * g920Xone = &report[index].value.g920Xone;

  g920Xone->wheel = clamp(0, axis[g920Xonea_wheel] + CENTER_AXIS_VALUE_16BITS, MAX_AXIS_VALUE_16BITS);

  g920Xone->gas = clamp(0, MAX_AXIS_VALUE_8BITS - axis[g920Xonea_gasPedal], MAX_AXIS_VALUE_8BITS);
  g920Xone->brake = clamp(0, MAX_AXIS_VALUE_8BITS - axis[g920Xonea_brakePedal], MAX_AXIS_VALUE_8BITS);
  g920Xone->clutch = clamp(0, MAX_AXIS_VALUE_8BITS - axis[g920Xonea_clutchPedal], MAX_AXIS_VALUE_8BITS);

  if (axis[g920Xonea_right])
  {
    if (axis[g920Xonea_down])
    {
      g920Xone->hatAndButtons = 0x03;
    }
    else if (axis[g920Xonea_up])
    {
      g920Xone->hatAndButtons = 0x01;
    }
    else
    {
      g920Xone->hatAndButtons = 0x02;
    }
  }
  else if (axis[g920Xonea_left])
  {
    if (axis[g920Xonea_down])
    {
      g920Xone->hatAndButtons = 0x05;
    }
    else if (axis[g920Xonea_up])
    {
      g920Xone->hatAndButtons = 0x07;
    }
    else
    {
      g920Xone->hatAndButtons = 0x06;
    }
  }
  else if (axis[g920Xonea_down])
  {
    g920Xone->hatAndButtons = 0x04;
  }
  else if (axis[g920Xonea_up])
  {
    g920Xone->hatAndButtons = 0x00;
  }
  else
  {
    g920Xone->hatAndButtons = 0x08;
  }

  if (axis[g920Xonea_X])
  {
    g920Xone->hatAndButtons |= G920_X_MASK;
  }
  if (axis[g920Xonea_A])
  {
    g920Xone->hatAndButtons |= G920_A_MASK;
  }
  if (axis[g920Xonea_B])
  {
    g920Xone->hatAndButtons |= G920_B_MASK;
  }
  if (axis[g920Xonea_Y])
  {
    g920Xone->hatAndButtons |= G920_Y_MASK;
  }

  g920Xone->buttons1 = 0x00;

  if (axis[g920Xonea_LB])
  {
    g920Xone->buttons1 |= G920_LB_MASK;
  }
  if (axis[g920Xonea_RB])
  {
    g920Xone->buttons1 |= G920_RB_MASK;
  }
  if (axis[g920Xonea_LT])
  {
    g920Xone->buttons1 |= G920_LT_MASK;
  }
  if (axis[g920Xonea_RT])
  {
    g920Xone->buttons1 |= G920_RT_MASK;
  }
  if (axis[g920Xonea_view])
  {
    g920Xone->buttons1 |= G920_VIEW_MASK;
  }
  if (axis[g920Xonea_menu])
  {
    g920Xone->buttons1 |= G920_MENU_MASK;
  }
  if (axis[g920Xonea_LS])
  {
    g920Xone->buttons1 |= G920_LS_MASK;
  }
  if (axis[g920Xonea_RS])
  {
    g920Xone->buttons1 |= G920_RS_MASK;
  }

  g920Xone->buttons2 = 0x00;

  if (axis[g920Xonea_guide])
  {
    g920Xone->buttons2 |= G920_GUIDE_MASK;
  }

  g920Xone->buttons3 = 0x00;

  if (axis[g920Xonea_gearShifter1])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_1_MASK;
  }
  if (axis[g920Xonea_gearShifter2])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_2_MASK;
  }
  if (axis[g920Xonea_gearShifter3])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_3_MASK;
  }
  if (axis[g920Xonea_gearShifter4])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_4_MASK;
  }
  if (axis[g920Xonea_gearShifter5])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_5_MASK;
  }
  if (axis[g920Xonea_gearShifter6])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_6_MASK;
  }
  if (axis[g920Xonea_gearShifterR])
  {
    g920Xone->buttons3 |= G920_GEAR_SHIFTER_R_MASK;
  }

  return index;
}

static s_controller controller =
{
  .name = "G920 XONE",
  .refresh_period = { .min_value = 1000, .default_value = 5000 },
  .axes = axes,
  .axis_name_dirs = { .nb = sizeof(axis_name_dirs)/sizeof(*axis_name_dirs), .values = axis_name_dirs },
  .fp_build_report = build_report,
  .fp_init_report = init_report,
};

void g920Xone_init(void) __attribute__((constructor (101)));
void g920Xone_init(void)
{
  controller_register(C_TYPE_G920_XONE, &controller);
}

