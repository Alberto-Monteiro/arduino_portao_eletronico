#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "Credenciais.h"

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);

  Blynk.begin(authPortaoPequeno, ssidPortaoPequeno, passPortaoPequeno, domain, port);
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V0)
{
  digitalWrite(LED_BUILTIN, param.asInt());
}
