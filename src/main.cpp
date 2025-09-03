#include <Arduino.h>
#include "Blinker.h"
#include "ESPGlobalStates.h"
#include "ESPWiFi.h"
#include "ESPWebServer.h"
#include "HandlePorton.h"
#include "ESPLogger.h"

Blinker led(pinLed);

ESPGlobalStates commands;
ESPWiFi wifi(led);
ESPLogger logger;
Porton porton(logger);
ESPWebServer server(commands, porton);
void setup()
{
  led.setup();
  Serial.begin(115200);
  wifi.connect();
  porton.setup();
  server.begin();
}

void loop()
{
  commands.checkCommands();

  porton.handleCommand();
  porton.handleStop();
  porton.autoClose();

  wifi.reconnect();

  // logger.log(String(counter++));
}
