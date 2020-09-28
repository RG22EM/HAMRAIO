/* 
 *  Библиотека для интерфейса ICOM CI-V
 *  (c) UA6EM
 *  26.09.2020
 *  V1.00-BETA
 */
/* */
#ifndef ICOM_h
#define ICOM_h
#include <Arduino.h>
#include <SoftwareSerial.h>


class ICOM
{
  public:
  ICOM();
  void setSerial(SoftwareSerial portInfo);
  void begin(int baud);
  
  unsigned long getFreqMode();
  
  private:
  
 };
  
#endif
/* */
