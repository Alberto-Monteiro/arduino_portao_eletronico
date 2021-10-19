#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "Credenciais.h"

BlynkTimer timer;

void pinHigh();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinHigh();

  Serial.begin(9600);

  Blynk.begin(authPortaoPequeno, ssidPortaoPequeno, passPortaoPequeno, domain, port);
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V1)
{
  digitalWrite(LED_BUILTIN, LOW);
  timer.setTimer(500L, pinHigh, 1);
}

void pinHigh()
{
  digitalWrite(LED_BUILTIN, HIGH);
}