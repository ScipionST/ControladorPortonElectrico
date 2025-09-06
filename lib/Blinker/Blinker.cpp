#include "Blinker.h"

Blinker::Blinker(const uint8_t pin) : _pin(pin) {}

void Blinker::setup()
{
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void Blinker::blink(const int count, const int interval)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(_pin, !digitalRead(_pin));

    unsigned long start = millis();
    int counter = 0;
    while (millis() - start < interval) {
      counter++;
    }
  }
}
