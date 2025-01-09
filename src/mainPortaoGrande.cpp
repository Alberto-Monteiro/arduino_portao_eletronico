#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>
#include <BlynkSimpleEsp8266.h>

#include "credenciais.h"

ESP8266WebServer server(80);
BlynkTimer blynkTimer;

static const uint8_t APRENDER = D2;
static const uint8_t BOTOEIRA = D3;
static const uint8_t BOTOEIRA_A = D5;
static const uint8_t BOTOEIRA_F = D6;

static const uint8_t LED_FF = D7;
int pinLedFF = LOW;
WidgetLED led_ff(V4);

static const uint8_t LED_FA = D4;
int pinLedFA = LOW;
WidgetLED led_fa(V5);

WidgetLED led_af(V7);

void leituraDosLeds();

void interacaoComPortao(int pinValue, uint8_t pin);

void setup()
{
  pinMode(APRENDER, INPUT);
  pinMode(BOTOEIRA, INPUT);
  pinMode(BOTOEIRA_A, INPUT);
  pinMode(BOTOEIRA_F, INPUT);
  pinMode(LED_FF, INPUT);
  pinMode(LED_FA, INPUT);

  Serial.begin(9600);

  Blynk.begin(authPortaoGrande, ssidPortaoGrande, passPortaoGrande, domain, port);

  blynkTimer.setInterval(100L, leituraDosLeds);

  server.on("/", []()
            { server.send(200, "text/plain; charset=UTF-8", "ESP8266 Portão grande V1.1.0"); });

  ElegantOTA.begin(&server, otaUser, otaPassword);

  server.begin();
}

void loop()
{
  Blynk.run();
  blynkTimer.run();
  ElegantOTA.loop();
  server.handleClient();
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
}

void interacaoComPortao(int pinValue, uint8_t pin)
{
  if (pinValue == 1)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    auto resetPin = pin == BOTOEIRA
                        ? []()
    { pinMode(BOTOEIRA, INPUT); }
                    : pin == BOTOEIRA_A ? []()
    { pinMode(BOTOEIRA_A, INPUT); }
                                        : []()
    { pinMode(BOTOEIRA_F, INPUT); };

    blynkTimer.setTimer(500L, resetPin, 1);
  }
}

BLYNK_WRITE(V1)
{
  interacaoComPortao(param.asInt(), BOTOEIRA);
}

BLYNK_WRITE(V8)
{
  interacaoComPortao(param.asInt(), BOTOEIRA_A);
}

BLYNK_WRITE(V9)
{
  interacaoComPortao(param.asInt(), BOTOEIRA_F);
}

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    pinMode(APRENDER, OUTPUT);
    digitalWrite(APRENDER, LOW);
  }
  else
  {
    pinMode(APRENDER, INPUT);
  }
}

BLYNK_CONNECTED()
{
  Serial.println("IP local:");
  Serial.println(WiFi.localIP());
  Blynk.virtualWrite(V10, WiFi.localIP().toString().c_str());
}
