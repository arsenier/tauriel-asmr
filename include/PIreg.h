#pragma once

#include <Arduino.h>

class PIreg
{
private:
    float I = 0;
    float Kp;
    float Ki;
    float Ts;

    float max_out;

    float out;

public:
    const float &q_out = out;

    PIreg(float Ts_, float Kp_, float Ki_, float max_out_)
    {
        Ts = Ts_;
        Kp = Kp_;
        Ki = Ki_;
        max_out = max_out_;
    }

    PIreg& tick(float in)
    {
        out = Kp * in + Ki * I;

        if (out == constrain(out, -max_out, max_out) || (in * out) < 0)
            I += in * Ts;

        return *this;
    }
};
