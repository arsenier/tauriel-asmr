#pragma once

#include <Arduino.h>

#include "Encoder.h"
#include "ConnectionList.h"

/*===ENCODERS===*/
Encoder encL, encR;

uint8_t get_AB(int PIN_CLK_A, int PIN_B)
{
    const uint8_t B = digitalRead(PIN_B);
    // 0000000B

    const uint8_t CLK_A = digitalRead(PIN_CLK_A);
    const uint8_t A = CLK_A ^ B;
    // 0000000A

    const uint8_t AB = (A << 1) | B;

    return AB;
}

/*==LEFT==*/
void isr_handle_left()
{
    encL.isr_handler();
}

void init_left()
{
    pinMode(PIN_ENC_LEFT_CLK_A, INPUT);
    pinMode(PIN_ENC_LEFT_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_ENC_LEFT_CLK_A), isr_handle_left, CHANGE);
}

int8_t get_AB_left()
{
    return get_AB(PIN_ENC_LEFT_CLK_A, PIN_ENC_LEFT_B);
}

/*==RIGHT==*/
void isr_handle_right()
{
    encR.isr_handler();
}

void init_right()
{
    pinMode(PIN_ENC_RIGHT_CLK_A, INPUT);
    pinMode(PIN_ENC_RIGHT_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_ENC_RIGHT_CLK_A), isr_handle_left, CHANGE);
}

int8_t get_AB_right()
{
    return get_AB(PIN_ENC_RIGHT_CLK_A, PIN_ENC_RIGHT_B);
}

EncoderParams encL_par = {
    .tick_to_rad = 1,
    .init_AB = init_left,
    .get_AB = get_AB_left,
    .dir = 1
};

EncoderParams encR_par = {
    .tick_to_rad = 1,
    .init_AB = init_right,
    .get_AB = get_AB_right,
    .dir = 1
};

void init_encoders()
{
    encL.init(encL_par);
    encR.init(encR_par);
}