#include <VL53L0X.h>

class TOF_sensor
{
  public:

    void      TurnOn ();
    void      TurnOff ();
    int       readDistance ();  //returns range in mm
    void      initSensor();    //before using this fcn Wire.begin needs to be conducted
    void      initXSHUT(); //initiates the XSHUT

  private:

    VL53L0X   sensor;
    bool      powerON;
    
};