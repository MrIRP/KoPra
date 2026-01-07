/*

  Fahren.h - Library for fahren das Fahrzeug.

  Created by Jakob Hawlat, November 24 2025.

  Not released into the public domain.

*/

#ifndef FAHREN_H
#define FAHREN_H

#include <Arduino.h>

class Fahren
{
  public:
   
    #define FAST 255
    #define SLOW 50
    //Constructor
    Fahren();
    //Methoden
    void setup();
    void moveGerade(bool direction, short speed);
    void turn(bool direction, short speed);
    void rotate(bool direction, short speed);
    void stop();
  private:
    short pinLinksVor;
    short pinRechtsVor;
    short pinLinksRueck;
    short pinRechtsRueck; 
};

#endif