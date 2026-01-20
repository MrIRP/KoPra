#include <Servo.h>
#include "fastop.h"

class Turm_servo{
  public:
    Servo     myServo;
    void      initServo();
    uint8_t   readPos();
    uint8_t   pitch(uint8_t pitch);
  private:
    uint8_t   currentPos; 
};