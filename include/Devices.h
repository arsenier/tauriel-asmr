#pragma once

#include "dev/Encoder.d.h"
#include "dev/Motor.d.h"

/*===Servo===*/
#include "VelEstimator.h"
#include "PIreg.h"
#include "Servo.h"

VelEstimator vel_est_L(Ts_sec, VEL_ESTIMATOR_T_CONST);
VelEstimator vel_est_R(Ts_sec, VEL_ESTIMATOR_T_CONST);

PIregParams PIreg_params_L = {Ts_sec, PI_REG_KP, PI_REG_KI, get_U_supply};
PIregParams PIreg_params_R = {Ts_sec, PI_REG_KP, PI_REG_KI, get_U_supply};

PIreg vel_reg_L(PIreg_params_L);
PIreg vel_reg_R(PIreg_params_R);

Servo servoL, servoR;

void init_servos()
{
    servoL.init(&encL, &motorL, &vel_est_L, &vel_reg_L);
    servoR.init(&encR, &motorR, &vel_est_R, &vel_reg_R);
}

/*===MOTION CONTROL===*/
#include "MotionControl.h"

MotionControl motion_control;

void init_motion_control()
{
    motion_control.init(&servoL, &servoR);
}

/*===ASMR===*/
#include "ASMR.h"

ASMR asmr;

void init_asmr()
{
    asmr.init(&motion_control);
}
