#ifndef ESPWEBSERVER_H
#define ESPWEBSERVER_h

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include "ESPGlobalStates.h"
#include "HandlePorton.h"

class ESPWebServer
{
public:
  ESPWebServer(ESPGlobalStates& commands, Porton& porton);
  void begin();

private:
  ESPGlobalStates& _commands;
  Porton& _porton;

  AsyncWebServer server;
  void routeStatus();
  void routeHome();
  void routeOpen();
  void routeNotFount();

  String controllerStatus();
  String controllerHome();
  String controllerPorton(const String& arg);

  bool isIPAllowed(AsyncWebServerRequest* request);

  String ESPReset();

  /*
    Descomentar

    const char* ElegantOTAUsername = "username";
    const char* ElegantOTAPassword = "password";
  */
};

#endif
