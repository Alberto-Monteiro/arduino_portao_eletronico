#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>
#include <BlynkSimpleEsp8266.h>

#include "Credenciais.h"

ESP8266WebServer server(80);
BlynkTimer blynkTimer;

static const uint8_t BOTOEIRA = D3;
static const uint8_t AJUSTE = D1;
static const uint8_t APRENDER = D2;

static const uint8_t LED_FF = D7;
int pinLedFF = LOW;
WidgetLED led_ff(V4);

static const uint8_t LED_FA = D4;
int pinLedFA = LOW;
WidgetLED led_fa(V5);

static const uint8_t LED_ST = D5;
int pinLedST = LOW;
WidgetLED led_st(V6);

WidgetLED led_af(V7);

void leituraDosLeds();

void openTheGate(int pinValue);

void setup()
{
  pinMode(BOTOEIRA, OUTPUT);
  digitalWrite(BOTOEIRA, HIGH);

  pinMode(AJUSTE, OUTPUT);
  digitalWrite(AJUSTE, HIGH);

  pinMode(APRENDER, OUTPUT);
  digitalWrite(APRENDER, HIGH);

  pinMode(LED_FF, INPUT);
  pinMode(LED_FA, INPUT);
  pinMode(LED_ST, INPUT);

  Serial.begin(9600);

  Blynk.begin(authPortaoGrande, ssidPortaoGrande, passPortaoGrande, domain, port);

  blynkTimer.setInterval(100L, leituraDosLeds);

  server.on("/", []() {
    server.send(200, "text/plain; charset=UTF-8", "ESP8266 Portão grande V1.0.1");
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

void leituraDosLeds()
{
  if (pinLedFF != digitalRead(LED_FF))
  {
    pinLedFF = digitalRead(LED_FF);
    if (digitalRead(LED_FF) == HIGH)
    {
      led_ff.off();
      Blynk.setProperty(V1, "offLabel", "Abrindo...");
      Blynk.setProperty(V1, "onLabel", "Parar");
      led_af.on();
    }
    else
    {
      led_ff.on();
      Blynk.setProperty(V1, "offLabel", "Abrir");
      Blynk.setProperty(V1, "onLabel", "Abrir");
      led_af.off();
    }
  }

  if (pinLedFA != digitalRead(LED_FA))
  {
    pinLedFA = digitalRead(LED_FA);
    if (digitalRead(LED_FA) == HIGH)
    {
      led_fa.off();
      Blynk.setProperty(V1, "offLabel", "Fechando...");
      Blynk.setProperty(V1, "onLabel", "Parar");
    }
    else
    {
      led_fa.on();
      Blynk.setProperty(V1, "offLabel", "Fechar");
      Blynk.setProperty(V1, "onLabel", "Fechar");
    }
  }

  if (pinLedST != digitalRead(LED_ST))
  {
    pinLedST = digitalRead(LED_ST);
    if (digitalRead(LED_ST) == HIGH)
    {
      led_st.off();
    }
    else
    {
      led_st.on();
    }
  }
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
    digitalWrite(BOTOEIRA, LOW);
    blynkTimer.setTimer(
        300L, []()
        { digitalWrite(BOTOEIRA, HIGH); },
        1);
  }
}

BLYNK_WRITE(V2)
{
  digitalWrite(AJUSTE, param.asInt() == 0 ? 1 : 0);
}

BLYNK_WRITE(V3)
{
  digitalWrite(APRENDER, param.asInt() == 0 ? 1 : 0);
}

BLYNK_CONNECTED()
{
  Serial.println("IP local:");
  Serial.println(WiFi.localIP());
  Blynk.virtualWrite(V10, WiFi.localIP().toString().c_str());
}