/*

  Fahren.cpp - Library for fahren das Fahrzeug.

  Created by Jakob Hawlat, November 24 2025.

  Not released into the public domain.

*/

#include "Fahren.h"

#define pinLinksVor     12
#define pinRechtsVor    10
#define pinLinksRueck   11
#define pinRechtsRueck  9
#define speedPinLinks   13
#define speedPinRechts  8

#define FAST 255
#define SLOW 50


Fahren::Fahren() {
  
}

void Fahren::setup(){
  pinMode(pinLinksVor,    OUTPUT);
  pinMode(pinRechtsVor,   OUTPUT);
  pinMode(pinLinksRueck,  OUTPUT);
  pinMode(pinRechtsRueck, OUTPUT);
  pinMode(speedPinLinks,  OUTPUT);
  pinMode(speedPinRechts, OUTPUT);
}


//true -> vorwärts || false -> rückwärts (sollte man aber nicht verwenden)
void Fahren::moveGerade(bool direction, short speed) {
  int vor = direction ? HIGH : LOW;
  int rueck = direction ? LOW : HIGH;

  digitalWrite(pinLinksVor, vor);
  digitalWrite(pinRechtsVor, vor);
  digitalWrite(pinLinksRueck, rueck);
  digitalWrite(pinRechtsRueck, rueck);
  analogWrite(speedPinLinks, speed);
  analogWrite(speedPinRechts, speed);
}


//true -> links || false -> rechts
//schaltet eines der Räder am um zu steuern
void Fahren::turn(bool direction, short speed) {
  int rechts = direction ? HIGH : LOW;
  int links = direction ? LOW : HIGH;

  digitalWrite(pinLinksVor, links);
  digitalWrite(pinRechtsVor, rechts);
  digitalWrite(pinLinksRueck, LOW);
  digitalWrite(pinRechtsRueck, LOW);
  digitalWrite(speedPinLinks, speed);
  digitalWrite(speedPinRechts, speed);
}

//true -> links || false -> rechts
//dreht die Räder in entgegengesetzte Richtungen um auf der Stelle zu drehen
void Fahren::rotate(bool direction, short speed) {
  int rechts = direction ? HIGH : LOW;
  int links = direction ? LOW : HIGH;

  digitalWrite(pinLinksVor, links);
  digitalWrite(pinRechtsVor, rechts);
  digitalWrite(pinLinksRueck, rechts);
  digitalWrite(pinRechtsRueck, links);
  digitalWrite(speedPinLinks, speed);
  digitalWrite(speedPinRechts, speed);
}

void Fahren::stop() {
  digitalWrite(pinLinksVor, LOW);
  digitalWrite(pinRechtsVor, LOW);
  digitalWrite(pinLinksRueck, LOW);
  digitalWrite(pinRechtsRueck, LOW);
  digitalWrite(speedPinLinks, LOW);
  digitalWrite(speedPinRechts, LOW);
}