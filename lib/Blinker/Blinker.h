#ifndef BLINKER_H
#define BLINKER_H

#include "Arduino.h"

class Blinker
{
public:
  Blinker(const uint8_t pin);
  void setup();
  void blink(const int count, const int interval);

private:
  const uint8_t _pin;
  int blinkCount;
  int blinkInterval;
};

#endif
