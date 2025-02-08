#include <Arduino.h>

#include "MazeVeirify.h"

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup done");

    mazeVeirify();
}

void loop()
{
}
