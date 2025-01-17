#include <Arduino.h>

#include "ASMR.h"

ASMR asmr;

void setup() {
    Serial.begin(9600);

    asmr.addAction(FWD);
    asmr.addAction(SS90EL);
    asmr.addAction(SS90EL);
    asmr.addAction(FWD);
    asmr.addAction(STOP);
}

void loop() {
    delay(50);
    asmr.exec();
}
