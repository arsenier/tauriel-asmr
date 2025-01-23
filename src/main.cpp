#include <Arduino.h>

#include "ASMR.h"

#include "Devices.h"

void setup()
{
    Serial.begin(9600);

    Serial.println("Serial init");

    init_encoders();
    init_motors();
    init_servos();
    init_motion_control();
    init_asmr();

    Serial.println("Init done");

    asmr.addAction(FWD);
    asmr.addAction(SS90EL);
    asmr.addAction(SS90EL);
    asmr.addAction(FWD);
    asmr.addAction(STOP);

    Serial.println("Setup done");
}

void loop()
{
    delay(50);
    asmr.exec();
    Serial.println();
}
