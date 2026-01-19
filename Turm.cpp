
//---------------------------------------------------------------------------------------------
//libraries
#include <Servo.h>
#include <Arduino.h>

#include "VL53L0X.h"
#include "fastop.h"
#include "Turm.h"


//Pin numbers on the board
#define XSHUT_PIN 52
#define SERVO_PIN 2
#define PUMP_PIN  10

//Constants
#define G_EARTH   9.81
#define Y_0       0

//jiggle angle amount and speed
#define SPRAY_ANGLE 2
#define SPRAY_TIMES 4
#define SPRAY_THROTTLE 10

//max scanning angle
#define SCAN_ANGLE 10

//Intervall of valid target distances in mm
#define VALID_MIN_DISTANCE 1200
#define VALID_MAX_DISTANCE 700

//muzzlevelocity in m/s
#define MUZZLE_VEL  3

//Servo parameters
#define ZERO_PITCH  86
#define MAX_PITCH   160
#define MIN_PITCH   0

struct ZielPosition
{
  uint16_t  range;
  uint8_t   angle;
};

//variables
VL53L0X TOFsensor;
bool    TOFstateOn;

Servo   myServo;
uint8_t myServoPitch;

ZielPosition target = 
{
  .range = 1000,
  .angle = 0
};

//---------------------------------------------------------------------------------------------

//setup function
void Turm::initTurm(){
  pinMode(XSHUT_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  myServo.attach(SERVO_PIN, 550, 2370);
  myServoPitch = ZERO_PITCH;
}

//Zielerfassung und Feuermechanismus ----------------------------------------------------------

void Turm::printTargetDistance(){
  Serial1.print("\nTarget distance: ");
  Serial1.print(target.range);
  Serial1.print(" mm");
  Serial1.print(" -- Target angle:    ");
  Serial1.print(target.angle);
  Serial1.print(" mm");
}

void Turm::scanForTarget(ZielPosition &target, uint8_t const zeroAngle)
{
  uint16_t tempDistance = -1;
  pitch(zeroAngle - SCAN_ANGLE);
  for(int i = -SCAN_ANGLE; i <= SCAN_ANGLE; i++)
  {
    tempDistance = sensorGetDistance();
    if ((VALID_MAX_DISTANCE > tempDistance) && (VALID_MIN_DISTANCE < tempDistance))
    {
      target.range = tempDistance;
      target.angle = myServoPitch;
      Serial.print("VALID TARGET");
    }
    Serial.print("\n target values: ");
    Serial.print(tempDistance);
    Serial.print("mm -- angle: ");
    Serial.print(myServoPitch);
    pitch(zeroAngle + i);
    delay(1000);
  }
  pitch(zeroAngle);
}

void Turm::fireOnTarget(ZielPosition &target)
{
  uint8_t firingAngle = calculateFiringAngle(target, MUZZLE_VEL);

  pitch(firingAngle);

  pumpOn();
  jiggle(firingAngle);
  pumpOff();
}

//pitches the tower up and down
void Turm::jiggle (uint8_t firingAngle)
{

  for(int j = SPRAY_TIMES; j > 0; j--)
  {
    for(int i = SPRAY_ANGLE; i > -SPRAY_ANGLE; i--)
    {
      myServo.write(firingAngle + i);
      myServoPitch += i;
      delay(SPRAY_THROTTLE);
    }

    for(int i = -SPRAY_ANGLE; i > SPRAY_ANGLE; i--)
    {
      myServo.write(myServoPitch + i);
      myServoPitch += i;
      delay(SPRAY_THROTTLE);
    }
  }
  pitch(firingAngle);
}

//calculates the firing angle from a known target position and muzzlevelocity
uint8_t Turm::calculateFiringAngle (ZielPosition &target, uint8_t muzzleVelocity)
{
  float x_z = cos(target.angle) * target.range;
  float y_z = sin(target.angle) * target.range;
  float gxz = G_EARTH*x_z*x_z/(muzzleVelocity*muzzleVelocity*2);

  return (uint8_t) atan ( (x_z - sqrt( x_z*x_z - 4*gxz*(Y_0 - y_z - gxz) ) )/gxz );
}

//Sensor---------------------------------------------------------------------------------------

void Turm::printSensorReadings()
{
  int temp = sensorGetDistance();
  Serial.print("\nTOFsensor distance: ");
  Serial.print(temp);
  Serial.print(" mm");
}

uint16_t Turm::sensorGetDistance()
{
  if(!TOFstateOn) {return -1;}
  uint16_t distance = TOFsensor.readRangeSingleMillimeters();
  if(TOFsensor.timeoutOccurred()) {return -1;}
  return distance;
}
void Turm::turnSensorOn()
{
  if (!TOFstateOn) {digitalWrite(XSHUT_PIN, HIGH); TOFsensor.initSensor(); TOFstateOn = true;}
}

void Turm::turnSensorOff()
{
  if (TOFstateOn) {digitalWrite(XSHUT_PIN, LOW);}
}

//Servo----------------------------------------------------------------------------------------

void Turm::servoTestDrive()
{
  pitch(160);
  printServoReadings();
  delay(1000);
  pitch(86);
  printServoReadings();
  delay(1000);
  pitch(0);
  printServoReadings();
  delay(1000);
}

void Turm::printServoReadings()
{
  Serial.print("\nServo angle: ");
  Serial.print(myServoPitch);
}
int Turm::pitch(int targetPitch)
{
  targetPitch = constrain(targetPitch, MIN_PITCH, MAX_PITCH);

  while (myServoPitch != targetPitch)
  {
    if (myServoPitch < targetPitch)
      myServoPitch++;
    else
      myServoPitch--;
    myServo.write(myServoPitch);
    fastop::delay(1000000);
  }
  return myServoPitch;
}

//Pumpe----------------------------------------------------------------------------------------

//Pumpe an.
void Turm::pumpOn()
{
  digitalWrite(PUMP_PIN, HIGH);
}

//Pumpe aus.
void Turm::pumpOff()
{
  digitalWrite(PUMP_PIN, LOW);
}
