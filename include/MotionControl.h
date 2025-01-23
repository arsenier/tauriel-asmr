#pragma once

#include "Servo.h"
#include "Config.h"

class MotionControl
{
private:
    Servo *servoL, *servoR;

public:
    void init(Servo *servoL_, Servo *servoR_)
    {
        servoL = servoL_;
        servoR = servoR_;
    }

    MotionControl& tick(float v_f0, float theta_i0)
    {
        // VEL TRANSFORMER
        float w_f0 = v_f0 / WHEEL_RADIUS_M;
        float w_delta0 = theta_i0 * WHEEL_BASE_M / WHEEL_RADIUS_M;

        // MIXER
        float w_L0 = w_f0 - w_delta0 / 2;
        float w_R0 = w_f0 + w_delta0 / 2;

        servoL->tick(w_L0);
        servoR->tick(w_R0);

        return *this;
    }
};
