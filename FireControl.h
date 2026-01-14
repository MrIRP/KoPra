class FireControl{

  public: 
    void    Zielerfassung(TOF_sensor sensor, Turm_servo servo);
    void    Feuern(short firingAngle);
    uint16_t calculateFiringAngle(uint16_t distance, uint8_t phi, uint16_t muzzle_vel)
  private:
    #define G_EARTH 9.81
    void    sprayNPray(short* firingAngle);

}