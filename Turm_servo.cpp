#include <Servo.h>
#include "fastop.h"

Servo   myServo;
uint8_t currentPos;

#define ServoPin     2
#define maxPitch    +90
#define minPitch    -90
#define zeroPitch   +1

void initServo ()
{
  myServo.attach(ServoPin);
  currentPos = zeroServo;
}

uint8_t readPos()
{
  return currentPos;
}

uint8_t pitch (uint8_t pitch)
{
  uint8_t pitchDiff = currentPos - pitch;

  //positiver Pitch
  if(pitchDiff >= 0)
  { 
    if(pitchDiff < pitch){pitch = pitchDiff;}

    while (currentPos != pitch)
    {
      myServo.write(currentPos);
      currentPos++;
      fastop::delay(10);
    }

    return currentPos;
  }

  //negativer Pitch
  if(pitchDiff > pitch){pitch = pitchDiff;}
  
  while (currentPos != pitch)
  {
    myServo.write(currentPos);
    currentPos--;
    fastop::delay(10);
  }

  return currentPos;
}