/* 
 *  Библиотека для интерфейса ICOM CI-V
 *  (c) UA6EM
 *  26.09.2020
 *  V1.00-BETA
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "ICOM.h"

// define software serial IO pins here:
extern SoftwareSerial rigCat(2,3); // rx,tx

#define dlyTime 5  // delay (in ms) after serial writes

ICOM::ICOM(){ } // nothing to do when first instanced

// Setup software serial with user defined input
// from the Arduino sketch (function, though very slow)
void ICOM::setSerial(SoftwareSerial portInfo) {
  rigCat = portInfo;
}

// similar to Serial.begin(baud); command
void ICOM::begin(int baud) {
  rigCat.begin(baud);
}



/* */
