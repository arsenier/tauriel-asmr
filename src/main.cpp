#include <Arduino.h>

#include "ASMR.h"

#include "Devices.h"

ASMR asmr;

void setup()
{
    Serial.begin(9600);

    init_encoders();
    init_motors();
    init_servos();
    init_motion_control();

    asmr.addAction(FWD);
    asmr.addAction(SS90EL);
    asmr.addAction(SS90EL);
    asmr.addAction(FWD);
    asmr.addAction(STOP);
}

void loop()
{
    delay(50);
    asmr.exec();
}
