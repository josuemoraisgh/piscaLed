#include <Arduino.h>
#include "inindThread.h"

#define PINLED 13
#define BOTAO 2

void piscaLed()
{
  bool x = digitalRead(PINLED);
  digitalWrite(PINLED, !x);
}

void setup()
{
  pinMode(PINLED, OUTPUT);
  pinMode(BOTAO, INPUT);
  // threadSetup(piscaLed, 500, NULL);
}

void loop()
{
  if (digitalRead(BOTAO) == HIGH)
  {
    piscaLed();
  }
  else
  {
    digitalWrite(PINLED, LOW);
  }
  delay(500);
}
