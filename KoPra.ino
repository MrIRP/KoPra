//libraries
//-------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Pixy2.h>
#include <Wire.h>
#include <Servo.h>

#include "Fahren.h"
#include "Kieserkennung.h"
#include "Turm.h"

Fahren fahren;
Pixy2 mypixy;
Kieserkennung kieserkennung;
Turm myTurm;
int angle = 86;

//Variablen
//-------------------------------------------------------------------------
  //Längenangaben in mm

static short radAbstand = 250;
static short radUmfang = 200;
unsigned long kiesTime;
unsigned long moveTime;
unsigned long lTime;
uint8_t r, g, b;
uint8_t sollR, sollG, sollB;
bool kiesErkennung;
bool kiesErkannt;
bool autoL;

void setup() {

  kiesErkennung = false;
  kiesErkannt = false;
  autoL = false;
  lTime = 0;

  sollR = 100;
  sollG = 100;
  sollB = 100;

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
  myTurm.initTurm();
  myTurm.turnSensorOn();
}

void loop() {
  //getimete funktionen werden nicht über delay() sondern über eine Abfrage der timer Variable ausgesetzt, um multitasking zu ermöglichen
  //myTurm.pitch(86);
  //myTurm.printSensorReadings();
  //myTurm.scanForTarget(myTurm.target, 86);
  //myTurm.servoTestDrive();

  if(Serial.available() > 0) {
    char cmd = Serial.read();
    switch(cmd){
      case 'w':
        angle++;
        myTurm.pitch(angle);
        Serial.print("\nUP: ");
        myTurm.printServoReadings();
        break;
      case 's':
        angle--;
        myTurm.pitch(angle);
        Serial.print("\nDOWN: ");
        myTurm.printServoReadings();
        break;
    }
  }
  myTurm.printSensorReadings();
  delay(500);
  /*
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
  }*/
  /*
  //Kieserkennung
  //-----------------------------------------------------------------------
    //Kies wird von Sensoren: Gyro, Pixycam erkannt
  if(kiesErkennung) {
    if(kiesTime - millis() < -200) {
      //kies erkennen
      mypixy.changeProg("video");
      if(mypixy.video.getRGB(mypixy.frameWidth / 2, mypixy.frameHeight / 2, &r, &g, &b)) {
        kiesTime = millis();
      }
    }

    if(kieserkennung.weichtBodenAb(r, g, b, sollR, sollG, sollB)) {
      fahren.stop();
      Serial1.println("Boden wurde erkannt");
      //LED muss angeschaltet werden und lautsprecher muss Ton ausgeben
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
  */
}



