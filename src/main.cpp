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

void seralComm()
{
  if (Serial.available() > 0)
  {
    String x = Serial.readStringUntil('\n');
    Serial.print("O valor desejado foi:");
    Serial.println(x);
    Serial.println("Digite o tempo desejado:");
    thread.interval[0] = x.toInt();
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(PINLED, OUTPUT);
  pinMode(BOTAO, INPUT);
  threadSetup(piscaLed, 500, seralComm, 100, NULL);
  Serial.println("Digite o tempo desejado:");
}

void loop()
{
}
