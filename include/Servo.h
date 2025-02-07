#pragma once

#include "Encoder.h"
#include "Motor.h"
#include "VelEstimator.h"
#include "PIreg.h"

class Servo
{
private:

    Encoder *enc;
    Motor *motor;
    VelEstimator *vel_estimator;

    PIreg *pi_reg;

    float phi;
    float w_0;
    float w;

public:
    const float &q_phi = phi;
    const float &q_w_0 = w_0;
    const float &q_w = w;

    void init(Encoder *enc_, Motor *motor_, VelEstimator *vel_estimator_, PIreg *pi_reg_)
    {
        enc = enc_;
        motor = motor_;
        vel_estimator = vel_estimator_;
        pi_reg = pi_reg_;

        w_0 = 0;
        w = 0;
    }

    Servo& tick(float w_0_)
    {
        w_0 = w_0_;
        phi = enc->tick().q_angle;
        w = vel_estimator->tick(phi).q_out;

        float err = w_0 - w;
        float u = pi_reg->tick(err).q_out;

        motor->tick(u);

        return *this;
    }
};
