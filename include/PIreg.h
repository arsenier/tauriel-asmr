#pragma once

#include <Arduino.h>

struct PIregParams
{
    float Ts;
    float Kp;
    float Ki;
    float (*max_out)();
};

class PIreg
{
private:
    float I = 0;
    PIregParams params;

    float out;

public:
    const float &q_out = out;

    PIreg(PIregParams params_)
    {
        params = params_;
    }

    PIreg& tick(float in)
    {
        out = params.Kp * in + params.Ki * I;

        float max_out = params.max_out();

        if (out == constrain(out, -max_out, max_out) || (in * out) < 0)
            I += in * params.Ts;

        return *this;
    }
};
