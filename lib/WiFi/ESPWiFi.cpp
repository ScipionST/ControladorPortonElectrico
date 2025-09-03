#include "ESPWiFi.h"
#include "PrivateConfig.h"

/*
  Descomenta esto y usa tu propia red/redes

struct Red
{
  const char* ssid;
  const char* password;
};

Red redes[] = {
    {"red1", "123456"},
    {"red2", "123456"},
};

*/

ESPWiFi::ESPWiFi(Blinker& led) : _led(led)
{
  totalRedes = size_t(redes) / sizeof(Red); // 1 Red = 8 bytes (4 ssid - 4 password) - La cantidad de redes es: 16 bytes(4 bytes *ssid + 4 bytes *passowrd) * 2(redes) / 8 bytes (4 bytes * 2 Red))
}

void ESPWiFi::connect()
{
  WiFiStatusTimer = millis();

  for (int i = 0; i < totalRedes; i++)
  {
    WiFi.begin(redes[i].ssid, redes[i].password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
      delay(250);
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      _led.blink(6, 100);
      return;
    }
  }

  ESP.restart();
}

void ESPWiFi::reconnect()
{
  static unsigned long timer = millis();
  if (millis() - timer >= 5000)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.disconnect();
      connect();
    }
    timer = millis();
  }
}
