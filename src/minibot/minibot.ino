/*
   Minibot

   Felipe Kühne
   29/10/2018
*/

#define AP 1 /* Define se é Access point ou cliente wifi. */

#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <WEMOS_Motor.h>
#include "wifi.h"
#include "html/index.h"

const char compile_date[] = __DATE__ " " __TIME__;
String version("0.0-1");

int pwmSpeed = 100;
double x = 0;
double y = 0;

//Motor shield I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 1000);
Motor M2(0x30, _MOTOR_B, 1000); //Motor B

WFclass wifi;
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);    // create a websocket server on port 81

String myName("minibot");

void printBanner()
{
  /*
         _      _ __        __ 
  __ _  (_)__  (_) /  ___  / /_
 /  ' \/ / _ \/ / _ \/ _ \/ __/
/_/_/_/_/_//_/_/_.__/\___/\__/ 
                               
*/
  Serial.println("           _       _ _           _");
  Serial.println("          (_)     (_) |         | |");
  Serial.println(" _ __ ___  _ _ __  _| |__   ___ | |_");
  Serial.println("| '_ ` _ \\| | '_ \\| | '_ \\ / _ \\| __|");
  Serial.println("| | | | | | | | | | | |_) | (_) | |_");
  Serial.println("|_| |_| |_|_|_| |_|_|_.__/ \\___/ \\__|");
  Serial.println("Version " + version + ", compiled at " + String(compile_date));
  Serial.println();
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  Serial.println("Minibot starting...");
  printBanner();
  
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  Serial.println("Motor A&B STOP");
  delay(200);

#if(AP == 1)
  wifi.softAP(myName);
#else
  wifi.connectFromList();
#endif

  MDNS.begin(myName.c_str());
  Serial.println("Serviço mDNS iniciado com o nome " + myName + ".local");
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Serviço websocket iniciado na porta 81.");

  /* Registrando as funções de callback para as rotas desejadas: */
  server.on("/", handleRoot); /* Rota raiz. */
  server.onNotFound(handleRoot);  
  server.on("/run", handleRun);
  server.on("/stop", handleStop);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/orientation", handleOrientation);

  /* Finalmente inicializa o servidor webserver, com as configurações feitas. */
  server.begin();
  Serial.println("Serviço HTTP rodando na porta 80.");

  digitalWrite(LED_BUILTIN, HIGH);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] get Text: %s\n", num, payload);
      
      /* Parse received data from client. */
      
      break;
  }
}

void loop()
{
  webSocket.loop();                           // constantly check for websocket events
  server.handleClient();
}

void handleRoot()
{
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("Recebendo requisição de ");
  Serial.println(server.client().remoteIP());
  server.send(200, "text/html", String(MAIN_page));

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleRun()
{
  M1.setmotor(_CW, pwmSpeed);
  M2.setmotor(_CW, pwmSpeed);
  //handleRoot();
}

void handleBw()
{
  M1.setmotor(_CCW, pwmSpeed);
  M2.setmotor(_CCW, pwmSpeed);
  //handleRoot();
}

void handleLeft()
{
  M1.setmotor(_STOP);
  M2.setmotor(_CW, pwmSpeed);
  //handleRoot();
}

void handleRight()
{
  M1.setmotor(_CW, pwmSpeed);
  M2.setmotor(_STOP);
  //handleRoot();
}

void handleStop()
{
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  //handleRoot();
}

void handleOrientation()
{
  Serial.println(server.arg("x"));  
  Serial.println(server.arg("y"));
}

