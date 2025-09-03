#include "ESPGlobalStates.h"

ESPGlobalStates::ESPGlobalStates() {}

void ESPGlobalStates::checkCommands()
{
  if (CommandReset && millis() - TimerReset >= TimeoutReset)
  {
    CommandReset = false;
    ESP.restart();
  }
}

void ESPGlobalStates::reset()
{
  CommandReset = true;
  TimerReset = millis();
}
