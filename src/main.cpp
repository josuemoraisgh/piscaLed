#include <Arduino.h>
#include "inindThread.h"

#define PINLED 13
#define BOTAO 2

void piscaLed()
{
  bool x = digitalRead(PINLED);
  if (digitalRead(BOTAO) == HIGH)
  {
    digitalWrite(PINLED, !x);
  }
  else
  {
    digitalWrite(PINLED, LOW);
  }
}

void setup()
{
  pinMode(PINLED, OUTPUT);
  pinMode(BOTAO, INPUT);
  threadSetup(piscaLed, 500, NULL);
}

void loop()
{
}
