#pragma once

#include "dev/Encoder.d.h"
#include "dev/Motor.d.h"

/*===Servo===*/
#include "VelEstimator.h"
#include "PIreg.h"
#include "Servo.h"

VelEstimator vel_est_L(0.001, 0.1);
VelEstimator vel_est_R(0.001, 0.1);

PIreg vel_reg_L(0.001, 1, 1, 5);
PIreg vel_reg_R(0.001, 1, 1, 5);

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
