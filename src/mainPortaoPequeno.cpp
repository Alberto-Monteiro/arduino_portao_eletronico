#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>
#include <BlynkSimpleEsp8266.h>

#include "credenciais.h"

ESP8266WebServer server(80);
BlynkTimer blynkTimer;
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

  server.on("/", []() {
    server.send(200, "text/plain; charset=UTF-8", "ESP8266 Portão pequeno V1.0.1");
  });

  ElegantOTA.begin(&server, otaUser, otaPassword);
  server.begin();
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();
  Blynk.run();
  blynkTimer.run();
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
