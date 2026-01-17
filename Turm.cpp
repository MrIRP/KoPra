
//---------------------------------------------------------------------------------------------
//libraries
#include <Servo.h>
#include <Arduino.h>

#include "fastop.h"
#include "VL53L0X.h"

#include "Turm.h"


//Pin numbers on the board
#define XSHUT_PIN 52
#define SERVO_PIN 10
#define PUMP_PIN 10

//Constants
#define G_EARTH   9.81
#define Y_0       0

//jiggle angle amount and speed
#define SPRAY_ANGLE 2
#define SPRAY_TIMES 4
#define SPRAY_THROTTLE 10

//max scanning angle
#define SCAN_ANGLE 5

//Intervall of valid target distances in mm
#define VALID_MIN_DISTANCE 1200
#define VALID_MAX_DISTANCE 700

//muzzlevelocity in m/s
#define MUZZLE_VEL  3

//Servo parameters
#define ZERO_PITCH  0
#define MAX_PITCH   (90 - ZERO_PITCH)
#define MIN_PITCH   (90 + ZERO_PITCH)

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
  Serial.begin(10000);
  pinMode(XSHUT_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServoPitch = ZERO_PITCH;
}

//Zielerfassung und Feuermechanismus ----------------------------------------------------------

void Turm::printTargetDistance(){
  Serial1.println("Target distance: ");
  Serial1.print(target.range);
  Serial1.print(" mm");
  Serial1.println("Target angle:    ");
  Serial1.print(target.angle);
  Serial1.print(" mm");
}

void Turm::ScanForTarget(ZielPosition &target, uint8_t zeroAngle)
{
  uint16_t tempDistance;
  pitch(zeroAngle);
  for(int i = -SCAN_ANGLE; i > SCAN_ANGLE; i++)
  {
    tempDistance = sensorGetDistance();
    if (tempDistance < VALID_MAX_DISTANCE && tempDistance > VALID_MIN_DISTANCE)
    {
      target.range = tempDistance;
      target.angle = myServoPitch;
    }

    myServo.write(myServoPitch + i);
    myServoPitch += i;
    fastop::delay(20);
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
      fastop::delay(SPRAY_THROTTLE);
    }

    for(int i = -SPRAY_ANGLE; i > SPRAY_ANGLE; i--)
    {
      myServo.write(myServoPitch + i);
      myServoPitch += i;
      fastop::delay(SPRAY_THROTTLE);
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
  Serial.println("TOFsensor distance: ");
  Serial.print(sensorGetDistance());
  Serial.print(" mm");
}

uint16_t Turm::sensorGetDistance()
{
  if(!TOFstateOn) {return -1;}
  uint16_t distance = TOFsensor.readRangeContinuousMillimeters();
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

void Turm::printServoReadings(){
  Serial1.println("Servo angle theoretical: ");
  Serial1.print(myServoPitch);
  Serial1.print(" °");
}

//Änderung der Turmneigung
uint8_t Turm::pitch(uint8_t pitch)
{
  //calculate pitchDifference from current to new pitch
  uint8_t pitchDiff = myServoPitch - pitch;

  //for positive pitch
  if(pitchDiff >= 0)
  {
    if (pitchDiff < pitch) {pitch = pitchDiff;}

    while(myServoPitch < pitch)
    {
      myServo.write(myServoPitch);
      myServoPitch++;
      fastop::delay(10);
    }

    return myServoPitch;
  }

  //for negative pitch
  if(pitchDiff > pitch){pitch = pitchDiff;}

  while (myServoPitch > pitch)
  {
    myServo.write(myServoPitch);
    myServoPitch--; 
    fastop::delay(10);
  }
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
