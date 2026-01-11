//libraries
//-------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Pixy2.h>
#include <Wire.h>
#include <Servo.h>

#include "Fahren.h"
#include "SensorLib.h"

Fahren fahren;
Pixy2 mypixy;
Servo myservo;
UltrasonicSensor ultra(49, 48);

//Variablen
//-------------------------------------------------------------------------
  //Längenangaben in mm

static short radAbstand = 250;
static short radUmfang = 200;
unsigned long boardTime;
unsigned long moveTime;
unsigned long lTime;
bool kiesErkennung;
bool kiesErkannt;
bool autoL;

void setup() {

  kiesErkennung = false;
  kiesErkannt = false;
  autoL = false;
  lTime = 0;

  //Wire
  Wire.begin();
  Wire.setClock(100000);

  //Serial
  Serial1.begin(9600);
  Serial1.println("Serial Started");
  Serial.begin(9600);
  Serial.println("Serial Started");
  Serial.println(Serial1.available());

  //PinModes
    //LED
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    //Motoren
    fahren.setup();
    //Sensoren
    mypixy.init();
    ultra.begin();
  
  //Servomotor
  myservo.attach(6);

}

void loop() {
  //getimete funktionen werden nicht über delay() sondern über eine Abfrage der timer Variable ausgesetzt, um multitasking zu ermöglichen

  if(ultra.update()){
    float distance = ultra.getValue();

    if(distance >= 0){
      Serial1.print("Distance: ");
      Serial1.print(distance);
      Serail1.println(" cm");
    }
  }

  //SerialBluetooth 
  
  if(Serial1.available() > 0) {
    char DATA = Serial1.read();
    switch(DATA) {
      case 'w':
        fahren.moveGerade(true, 255);
        //Serial1.println("w");
        break;
      case 'a':
        fahren.turn(true, 255);
        //Serial1.println("a");
        break;
      case 'd':
        fahren.turn(false, 255);
        //Serial1.println("d");
        break;
      case 's':
        fahren.stop();
        //Serial1.println("s");
        break;
      case 'l':
        fahren.rotate(true, 255);
        break;
      case 'r':
        fahren.rotate(false, 255);
        break;
    }
  }

  //Kieserkennung
  //-----------------------------------------------------------------------
    //Kies wird von Sensoren: Gyro, Pixycam erkannt
  if(kiesErkennung) {
    //kies erkennen
    if(kiesErkannt) {
      fahren.stop();
      //Per led oder Bluetooth 
    }
    else {
      fahren.moveGerade(true, 255);
    }
  }

  //Slalom
  //-----------------------------------------------------------------------
    //Motorsteuerung manuell

  //AutoLöschen
  //-----------------------------------------------------------------------
  if(autoL) {
    //vier Phasen
    //Suchen || Zielen || Schießen || Fahren
    //Suchen (1)
      //sweeping von links nach rechts
      //aufzeichnung der Daten des Ultraschallsensors
      //Auswerten der Daten
      //zurückschwenken bis zu maximalem ausschlag
    //Zielen (2)
      //kleiner Schwenk mit tof sensor
      //Bis maximaler Wert gefunden
    //Schießen (3)
      //auwertung der Distanz
      //ausrichten der Düse
      //Schießen
    //Fahren (4)
      //ausrichten an evtl Bodenmarkierungen
      //Fahren bis zum nächsten markierungspunkt
      //Suchen starten
    if(lTime == 0) {
      lTime = millis();
    }
    int a = 1;
    int b = 1;
    switch(a) {
      //Suchen
      case 1:
        switch(b) {
          //Schwenken rechts
          case 1:
            break;
          //Schwenken links
          case 2:
            break;
        }
        break;
      case 2:
        a = 3;
        break;
      case 3:
        a = 4;
        break;
      case 4:
        a = 1;
        break;
    }

  }
  
}



