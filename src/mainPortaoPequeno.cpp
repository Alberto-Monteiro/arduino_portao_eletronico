#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "Credenciais.h"

BlynkTimer timer;

AsyncWebServer server(80);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);

  Serial.begin(9600);

  Blynk.begin(authPortaoPequeno, ssidPortaoPequeno, passPortaoPequeno, domain, port);

  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "ESP8266 Portao pequeno"); });

  AsyncElegantOTA.begin(&server, otaUser, otaPassword);
}

void loop()
{
  Blynk.run();
  timer.run();
  AsyncElegantOTA.loop();
}

BLYNK_WRITE(V1)
{
  digitalWrite(LED_BUILTIN, !param.asInt());
  digitalWrite(D2, param.asInt());
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
  Blynk.virtualWrite(V10, WiFi.localIP().toString().c_str());
}