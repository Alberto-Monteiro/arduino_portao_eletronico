#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "Credenciais.h"
#include "OTA.h"

BlynkTimer timer;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600);

  Blynk.begin(authPortaoPequeno, ssidPortaoPequeno, passPortaoPequeno, domain, port);

  configOTA();
  ArduinoOTA.setHostname("esp8266_portao_pequeno");
}

void loop()
{
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();
}

BLYNK_WRITE(V1)
{
  digitalWrite(LED_BUILTIN, !param.asInt());
  //if (param.asInt() == 0)
  //{
  //  digitalWrite(LED_BUILTIN, LOW);
  //  timer.setTimer(
  //      500L, []()
  //      { digitalWrite(LED_BUILTIN, HIGH); },
  //      1);
  //}
}

BLYNK_CONNECTED()
{
  Serial.println("IP local:");
  Serial.println(WiFi.localIP());
}