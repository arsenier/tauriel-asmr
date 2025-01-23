#pragma once

#include "inttypes.h"

struct MotorParams
{
    void (*init_M)();
    void (*drive_M)(int16_t pwm);
    float (*get_U_supply)();
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
    int16_t pwm_req = 255.0 * (u / m_par.get_U_supply());
    m_par.drive_M(pwm_req);
}
