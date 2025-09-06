#include "ESPWebServer.h"
#include "PrivateConfig.h"

ESPWebServer::ESPWebServer(ESPGlobalStates& commands, Porton& porton) : server(80), _commands(commands), _porton(porton) {}

void ESPWebServer::begin()
{
  routeNotFount();

  routeStatus();
  routeHome();
  routeOpen();
  ElegantOTA.begin(&server, ElegantOTAUsername, ElegantOTAPassword); // Puede cambiarse entre WebServer y AsyncWebServer en: #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
  server.begin();
}

// ALLOWED IP
bool ESPWebServer::isIPAllowed(AsyncWebServerRequest* request)
{
  String clientIP = request->client()->remoteIP().toString();
  clientIP.trim();
  Serial.println(clientIP);

  // Funciona mediante una lista blanca de IP's locales, de lo contrario, no podrás acceder a la Web del microcontrolador
  // Esto es solo una forma de proteger el microcontrolador frente a terceros en redes grandes.
  const String allowedIPs[] = {
      "192.168.0.100",
      "192.168.0.150", // MOTO
      "192.168.0.151", // NOVA
      "192.168.1.100",
  };

  const int numAllowed = sizeof(allowedIPs) / sizeof(allowedIPs[0]);

  for (int i = 0; i < numAllowed; i++)
  {
    if (clientIP.equals(allowedIPs[i]))
    {
      Serial.println("Conexión permitida...");
      return true;
    }
  }

  Serial.println("Conexión denegada...");
  return false;
}

// NOTFOUND
void ESPWebServer::routeNotFount()
{
  server.onNotFound([this](AsyncWebServerRequest* request)
    {
      if (!isIPAllowed(request)) {
        request->send(403);
        return;
      }
      request->redirect("/");
    });
  Serial.println("routeNotFound...");
}

// STATUS
void ESPWebServer::routeStatus()
{
  server.on("/status", HTTP_GET, [this](AsyncWebServerRequest* request)
    {
      if (!isIPAllowed(request))
      {
        request->send(403);
        return;
      }
      request->send(200, "text/html; charset=utf-8", controllerStatus()); });
  Serial.println("routeStatus...");
}

String ESPWebServer::controllerStatus()
{
  String status = "online";
  return status;
}

// HOME
void ESPWebServer::routeHome()
{
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request)
    {
      if (!isIPAllowed(request)) {
        Serial.println("Denegado" + request->client()->remoteIP().toString());
        request->send(403);
        return;
      }
      request->send(200, "text/html; charset=utf-8", controllerHome()); });
  Serial.println("routeHome...");
}

String ESPWebServer::controllerHome()
{
  String html = "<!DOCTYPE html>";
  html += "<html lang=\"es\">";

  // HEAD
  html += "<head>";
  html += "<meta charset=\"utf-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<title>Portón</title>";

  // STYLE
  html += "<style>";
  html += "   body {";
  html += "     font-family: Roboto, system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;";
  html += "     background-color: rgb(235, 235, 235);";
  html += "     display: grid;";
  html += "     justify-content: center;";
  html += "     margin: 0;";
  html += "     padding: 0;";
  html += "     align-content: center;";
  html += "   }";

  html += "   nav {";
  html += "     background-color: rgb(55, 59, 62);";
  html += "     color: rgb(255, 255, 255);";
  html += "     width: 100vw;";
  html += "     box-sizing: border-box;";
  html += "     padding: 0 10px;";
  html += "     margin: 0;";
  html += "     display: flex;";
  html += "     justify-content: space-between;";
  html += "     align-items: center;";
  html += "   }";

  html += "   h2 {";
  html += "     font-size: 20px;";
  html += "     text-align: center;";
  html += "     margin-block: 10px;";
  html += "     font-weight: 400;";
  html += "   }";

  html += "   ul {";
  html += "     list-style: none;";
  html += "     display: flex;";
  html += "     gap: 10px;";
  html += "     font-size: 18px;";
  html += "     align-items: center;";
  html += "   }";

  html += "   h1 {";
  html += "     margin-block: 15px;";
  html += "     text-align: center;";
  html += "   }";

  html += "   a {";
  html += "     text-decoration: none;";
  html += "     color: rgb(255, 255, 255);";
  html += "   }";

  html += "   nav ul li {";
  html += "     cursor: pointer;";
  html += "   }";

  html += "   div {";
  html += "     text-align: center";
  html += "   }";

  html += "   button {";
  html += "     max-width: 100px";
  html += "   }";

  html += "</style>";

  // MESSAGE
  html += "<script>";

  html += "   let message = \"...\";";
  html += "   document.addEventListener(\"DOMContentLoaded\", () => {";
  html += "     document.getElementById(\"message\").innerText = message;";
  html += "   });";

  // FUNCTION RESTART
  html += "   async function ESPrestart() {";
  html += "     try {";
  html += "       const response = await fetch('/restart');";
  html += "       if (!response.ok) {";
  html += "         throw new Error('Error en la red');";
  html += "       }";
  html += "       const text = await response.text();";
  html += "       message = text;";
  html += "       console.log(message);";
  html += "       document.getElementById(\"message\").innerText = message;";
  html += "     } catch (error) {";
  html += "       console.error(error);";
  html += "       message = 'Error al reiniciar';";
  html += "       document.getElementById(\"message\").innerText = message;";
  html += "     }";
  html += "   }";

  // FUNCTION RESTART COMPLETE
  html += "   async function ESPRestartComplete() {";
  html += "     let attempt = 0;";
  html += "     message = \"Reconectando...\";";
  html += "     document.getElementById(\"message\").innerText = message;";
  html += "     while (true) {";
  html += "       try {";
  html += "         const response = await fetch('/status');";
  html += "         if (!response.ok) throw new Error('No OK');";
  html += "         const text = await response.text();";
  html += "         console.log('Servidor disponible:', text);";
  html += "         document.getElementById(\"message\").innerText = text;";
  html += "         return;";
  html += "       } catch (error) {";
  html += "         attempt++;";
  html += "         console.warn(`Intento ${attempt} fallido. Reintentando...`);";
  html += "         message = `Esperando respuesta del servidor (${attempt})...`;";
  html += "         document.getElementById(\"message\").innerText = message;";
  html += "         await new Promise(resolve => setTimeout(resolve, 2000));";
  html += "       }";
  html += "     }";
  html += "   }";

  // REINICIAR Y VERIFICAR
  html += "   async function reiniciarYVerificar() {";
  html += "     await ESPrestart();";
  html += "     setTimeout(() => {";
  html += "       ESPRestartComplete();";
  html += "     }, 3000);";
  html += "   }";

  // PORTON
  html += "   async function porton(action) {";
  html += "     try {";
  html += "       message = \"esperando respuesta...\";";
  html += "       document.getElementById(\"message\").innerText = message;";
  html += "       const response = await fetch(`gate?call=${action}`);";
  html += "       if (!response.ok) throw new Error('No response... Inténtalo nuevamente');";
  html += "       const text = await response.text();";
  html += "       console.log(text);";
  html += "       document.getElementById(\"message\").innerText = text;";
  html += "       return;";
  html += "     } catch (error) {";
  html += "       message = \"Error al intentar conectar al controlador... Inténtalo nuevamente\";";
  html += "       document.getElementById(\"message\").innerText = message;";
  html += "       console.error(error);";
  html += "     }";
  html += "   }";

  html += "</script>";

  // END HEAD
  html += "</head>";

  // BODY
  html += "<body>";

  // MENU
  html += "<nav>";
  html += "   <h2>ESP32</h2>";
  html += "   <ul>";
  html += "     <li><a href=\"/\">Inicio</a></li>";
  html += "     <li><a href=\"/status\">Estado</a></li>";
  html += "     <li><a href=\"/update\">Update</a></li>";
  html += "     <li onclick=\"porton('restart')\">Reiniciar</li>";
  html += "   </ul>";
  html += "</nav>";
  html += "<h1>ESP32</h1>";

  html += "<div style=\"display: grid; gap: 25px;\">";
  html += "   <span id=\"message\"></span>";
  html += "   <div style=\"display: flex; gap: 15px; justify-content: center;\">";
  html += "     <button type=\"button\" onclick=\"porton(`open`)\">Abrir</button>";
  html += "     <button type=\"button\" onclick=\"porton(`close`)\">Cerrar</button>";
  html += "     <button type=\"button\" onclick=\"porton(`stop`)\">Detener</button>";
  html += "   </div>";
  html += "</div>";

  html += "</body>";
  html += "</html>";
  -
    Serial.println("routeHome...");
  return html;
}

// OPEN
void ESPWebServer::routeOpen()
{
  server.on("/gate", HTTP_GET, [this](AsyncWebServerRequest* request)
    {
      Serial.println("routeOpen...");
      if (!isIPAllowed(request)) {
        request->send(403);
        return;
      }
      String arg = request->arg("call");
      request->send(200, "text/html; charset=utf-8", controllerPorton(arg)); });
  Serial.println("routeOpen...");
}

String ESPWebServer::controllerPorton(const String& arg)
{
  String response = "";

  if (arg == "open")
  {
    Serial.println("arg Open...");
    response = _porton.open();
  }
  else if (arg == "close")
  {
    Serial.println("arg Close...");
    response = _porton.close();
  }
  else if (arg == "stop")
  {
    Serial.println("arg Stop...");
    response = _porton.stop();
  }
  else if (arg == "restart") {
    Serial.println("arg Restart...");
    response = ESPReset();
  }
  else {
    response = "Solicitud inválida...";
  }

  Serial.println(response);
  return response;
}

// RESET
String ESPWebServer::ESPReset()
{
  String text = "";
  if (_commands.CommandReset) {
    return text = "Reinicio ya programado...";
  }

  text = "Reiniciando...";
  _commands.reset();
  return text;
}
