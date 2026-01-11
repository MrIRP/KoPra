/*

  SensorLib.cpp - Library for Sensoren auslesen.

  Created by Jakob Hawlat, January 11 2026.

  Not released into the public domain.

*/

#ifndef SENSORLIB_H
#define SENSORLIB_H

#include <Arduino.h>
#include <Wire.h>

// Base Sensor Class
class Sensor {
  public:
    virtual void begin() = 0;
    virtual bool update() = 0; // Non-blocking update
    virtual float getValue() = 0; // Get latest value
    virtual bool isReady() = 0; // Ready for new measurement
};

// HC-SR04 Ultrasonic Sensor
class UltrasonicSensor : public Sensor {
  private:
    uint8_t trigPin, echoPin;
    unsigned long lastMeasurement;
    float distance;
    bool measuring;

  public:
    UltrasonicSensor(uint8_t trigger, uint8_t echo) : 
      trigPin(trigger), echoPin(echo), lastMeasurement(0), distance(-1), measuring(false) {}

    void begin() override {
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
      digitalWrite(trigPin, LOW);
    }

    bool update() override {
      unsigned long now = millis();

      if (now - lastMeasurement >= 500) { // 20Hz sampling
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        lastMeasurement = now;
        measuring = true;

        // Schedule distance reading after pulse
        delayMicroseconds(750); // Wait for echo
        long duration = pulseIn(echoPin, HIGH, 30000);
        distance = (duration == 0) ? -1 : duration / 58.3f;
        measuring = false;

        return true;
      }
      return false;
    }

    float getValue() override { return distance; }
    bool isReady() override { return !measuring; }
};

// VL53L0X Time-of-Flight Sensor (Non-blocking)
class TOFSensor : public Sensor {
  private:
    TwoWire *wire;
    uint8_t address;
    unsigned long lastMeasurement;
    float distance;
    bool initialized;

    // Minimal VL53L0X registers (simplified)
    static const uint8_t REG_SYSRANGE_START = 0x00;
    static const uint8_t REG_RESULT_RANGE_STATUS = 0x14;

  public:
    TOFSensor(TwoWire &w = Wire, uint8_t addr = 0x29) : 
      wire(&w), address(addr), lastMeasurement(0), distance(-1), initialized(false) {}

    void begin() override {
      wire->begin();
      delay(10);

      // Check device ID (simplified initialization)
      wire->beginTransmission(address);
      if (wire->endTransmission() == 0) {
        initialized = true;
        // Start continuous mode
        wire->beginTransmission(address);
        wire->write(REG_SYSRANGE_START);
        wire->write(0x01); // Start single measurement
        wire->endTransmission();
        lastMeasurement = millis();
      }
    }

    bool update() override {
      if (!initialized) return false;

      unsigned long now = millis();
      if (now - lastMeasurement >= 50) { // Read every 50ms
        // Check if result is ready
        wire->beginTransmission(address);
        wire->write(REG_RESULT_RANGE_STATUS);
        wire->endTransmission();

        wire->requestFrom(address, (uint8_t)12);
        if (wire->available()) {
          wire->read(); // Status
          wire->read(); // Stream count
          wire->read(); // DSS config

          // Read distance (16-bit)
          uint8_t lsb = wire->read();
          uint8_t msb = wire->read();
          int dist = (msb << 8) | lsb;
          distance = (dist > 0) ? dist : -1;

          // Restart measurement
          wire->beginTransmission(address);
          wire->write(REG_SYSRANGE_START);
          wire->write(0x01);
          wire->endTransmission();

          lastMeasurement = now;
          return true;
        }
      }
      return false;
    }

    float getValue() override { return distance; }
    bool isReady() override { return (millis() - lastMeasurement >= 50); }
};

#endif
