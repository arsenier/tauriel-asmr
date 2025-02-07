#pragma once

#include "inttypes.h"

struct MotorParams
{
    void (*init_M)();
    void (*drive_M)(int16_t pwm);
    float (*get_U_supply)();
    int16_t max_pwm;
};

class Motor
{
private:
    MotorParams m_par;
public:
    void init(MotorParams m_par_);
    void tick(float u);
};

void Motor::init(MotorParams m_par_)
{
    m_par = m_par_;
    m_par.init_M();
}

void Motor::tick(float u)
{
    int16_t pwm_req = 1.0 * m_par.max_pwm * (u / m_par.get_U_supply());
    pwm_req = constrain(pwm_req, -m_par.max_pwm, m_par.max_pwm);
    m_par.drive_M(pwm_req);
}
