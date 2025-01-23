#pragma once

class VelEstimator
{
private:
    float I = 0;
    float T;
    float Ts;
    float out;

public:
    const float &q_out = out;

    VelEstimator(float Ts_, float T_)
    {
        Ts = Ts_;
        T = T_;
    }

    VelEstimator& tick(float in)
    {
        out = (in - I) / T;
        I += out * Ts;
        return *this;
    }
};
