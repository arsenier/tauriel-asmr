#pragma once

#include <Arduino.h>

// Encoder.h

struct EncoderParams
{
    float tick_to_rad;
    void (*init_AB)();
    int8_t (*get_AB)();
    int8_t dir;
};

class Encoder
{
private:
    volatile int counter = 0;
    float angle = 0;
    EncoderParams en_par;
    int8_t old_AB = 0;

    int8_t ett[4][4] = {0};

public:
    const float &q_angle = angle;

    void init(EncoderParams en_par_);
    Encoder& tick();
    void isr_handler();
};

void Encoder::init(EncoderParams en_par_)
{
    en_par = en_par_;

    ett[0b00][0b10] = en_par.dir;
    ett[0b10][0b11] = en_par.dir;
    ett[0b11][0b01] = en_par.dir;
    ett[0b01][0b00] = en_par.dir;

    ett[0b00][0b01] = -en_par.dir;
    ett[0b01][0b11] = -en_par.dir;
    ett[0b11][0b10] = -en_par.dir;
    ett[0b10][0b00] = -en_par.dir;

    en_par.init_AB();
}

Encoder& Encoder::tick()
{
    noInterrupts();
    int counter_buf = counter;
    interrupts();

    angle += counter_buf * en_par.tick_to_rad;
}

void Encoder::isr_handler()
{
    int8_t AB = en_par.get_AB();

    int8_t dir = ett[AB][old_AB];
    old_AB = AB;

    counter += dir;
}
