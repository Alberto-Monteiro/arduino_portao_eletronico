#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "Credenciais.h"

BlynkTimer blynkTimer;
AsyncWebServer serverOTA(80);
bool isWiFiConected = false;
int valuePinBotoeira = LOW;

void openTheGate(int pinValue);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  blynkTimer.setInterval(300L, []()
                         {
    if (!isWiFiConected) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    } });

  pinMode(D1, INPUT);
  blynkTimer.setInterval(50L, []()
                         {
      if (valuePinBotoeira != digitalRead(D1)) {
        valuePinBotoeira = digitalRead(D1);
        if (valuePinBotoeira == HIGH)
          openTheGate(1);
      } });

  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);

  Serial.begin(9600);

  Blynk.begin(authPortaoPequeno, ssidPortaoPequeno, passPortaoPequeno, domain, port);

  serverOTA.begin();
  serverOTA.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain; charset=UTF-8", "ESP8266 Portão pequeno V1.0.1"); });

  AsyncElegantOTA.begin(&serverOTA, otaUser, otaPassword);
}

void loop()
{
  Blynk.run();
  blynkTimer.run();
  AsyncElegantOTA.loop();
}

BLYNK_WRITE(V1)
{
  openTheGate(param.asInt());
  Blynk.virtualWrite(V1, "0");
}

void openTheGate(int pinValue)
{
  if (pinValue == 1)
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(D2, LOW);
    blynkTimer.setTimer(
        300L, []()
        { 
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(D2, HIGH); },
        1);
  }
}

BLYNK_CONNECTED()
{
  isWiFiConected = true;
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("IP local:");
  Serial.println(WiFi.localIP());
  Blynk.virtualWrite(V10, WiFi.localIP().toString().c_str());
}

BLYNK_DISCONNECTED()
{
  isWiFiConected = false;
  digitalWrite(LED_BUILTIN, LOW);
}