/*

  SensorLib.h - Library for Sensoren auslesen.

  Created by Jakob Hawlat, January 11 2026.

  Not released into the public domain.

*/

#include "SensorLib.h"

UltrasonicSensor ultra(9, 10);  // Trigger, Echo
TOFSensor tof(Wire, 0x29);      // I2C, Address

void setup() {
  Serial.begin(9600);
  ultra.begin();
  tof.begin();
}

void loop() {
  ultra.update();
  tof.update();

  if (ultra.isReady()) {
    float uval = ultra.getValue();
    if (uval > 0) Serial.println("Ultra: " + String(uval));
  }

  if (tof.isReady()) {
    float tval = tof.getValue();
    if (tval > 0) Serial.println("TOF: " + String(tval));
  }

  // Other code runs without blocking
}
