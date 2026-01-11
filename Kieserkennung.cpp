/*

  Kieserkennung.cpp - Library for erkennen das Kies.

  Created by Jakob Hawlat, December 04 2025.

  Not released into the public domain.

*/

#include "Kieserkennung.h"

Kieserkennung::Kieserkennung() {
  abweichung = 20;
}

bool Kieserkennung::weichtBodenAb(uint8_t currentR, uint8_t currentG, uint8_t currentB, uint8_t sollR, uint8_t sollG, uint8_t sollB) {
  int deltaR = currentR - sollR;
  int deltaG = currentG - sollG;
  int deltaB = currentB - sollB;

  int quadratDistanz = deltaR * deltaR + deltaG * deltaG + deltaB * deltaB;

  return quadratDistanz > abweichung * abweichung;
}