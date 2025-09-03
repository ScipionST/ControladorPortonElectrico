#include "WiFi.h"
#include "Blinker.h"

class Blinker;

class ESPWiFi
{
public:
  ESPWiFi(Blinker& led);
  void connect();
  void reconnect();

private:
  Blinker& _led;
  int totalRedes = 0;
  unsigned long WiFiStatusTimer = 0;
};
