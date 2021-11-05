#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "Credenciais.h"

static const uint8_t BOTOEIRA = D3;
static const uint8_t AJUSTE = D1;
static const uint8_t APRENDER = D2;

static const uint8_t LED_FF = D0;
int pinLedFF = LOW;
WidgetLED led_ff(V4);

static const uint8_t LED_FA = D4;
int pinLedFA = LOW;
WidgetLED led_fa(V5);

static const uint8_t LED_ST = D5;
int pinLedST = LOW;
WidgetLED led_st(V6);

WidgetLED led_af(V7);

BlynkTimer timer;

AsyncWebServer server(80);

void leituraDosLeds();

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

  timer.setInterval(100L, leituraDosLeds);

  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "ESP8266 Portao grande"); });

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
  digitalWrite(BOTOEIRA, param.asInt() == 0 ? 1 : 0);
}

BLYNK_WRITE(V2)
{
  digitalWrite(AJUSTE, param.asInt() == 0 ? 1 : 0);
}

BLYNK_WRITE(V3)
{
  digitalWrite(APRENDER, param.asInt() == 0 ? 1 : 0);
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

BLYNK_CONNECTED()
{
  Serial.println("IP local:");
  Serial.println(WiFi.localIP());
  Blynk.virtualWrite(V10, WiFi.localIP().toString().c_str());
}