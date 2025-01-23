#pragma once

#include <Arduino.h>

#include "Motor.h"
#include "ConnectionList.h"

/*===MOTORS===*/
Motor motorL, motorR;

float get_U_supply()
{
    return 5.0;
}

void init_motorL()
{
    pinMode(PIN_MOTOR_LEFT_PWM, OUTPUT);
    pinMode(PIN_MOTOR_LEFT_DIR, OUTPUT);
}

void drive_motor(int16_t pwm, int pin_dir, int pin_pwm)
{
    if(pwm >= 0)
    {
        digitalWrite(pin_dir, LOW);
        analogWrite(pin_pwm, pwm);
    }
    else
    {
        digitalWrite(pin_dir, HIGH);
        analogWrite(pin_pwm, 255 + pwm);
    }

    // Serial.print(" Motor pin_dir: ");
    // Serial.print(pin_dir);
    // Serial.print(" pin_pwm: ");
    // Serial.print(pin_pwm);
    // Serial.print(" pwm: ");
    Serial.print(" pwm: ");
    Serial.print(pwm);
}

void drive_motorL(int16_t pwm)
{
    drive_motor(pwm, PIN_MOTOR_LEFT_DIR, PIN_MOTOR_LEFT_PWM);
}


void init_motorR()
{
    pinMode(PIN_MOTOR_RIGHT_PWM, OUTPUT);
    pinMode(PIN_MOTOR_RIGHT_DIR, OUTPUT);
}

void drive_motorR(int16_t pwm)
{
    drive_motor(pwm, PIN_MOTOR_RIGHT_DIR, PIN_MOTOR_RIGHT_PWM);
}

MotorParams motorL_par = {
    .init_M = init_motorL,
    .drive_M = drive_motorL,
    .get_U_supply = get_U_supply,
    .max_pwm = MAX_PWM_VALUE
};

MotorParams motorR_par = {
    .init_M = init_motorR,
    .drive_M = drive_motorR,
    .get_U_supply = get_U_supply,
    .max_pwm = MAX_PWM_VALUE
};

void init_motors()
{
    motorL.init(motorL_par);
    motorR.init(motorR_par);
}