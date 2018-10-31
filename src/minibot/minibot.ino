/*
   Minibot

   Felipe Kühne
   29/10/2018

*/

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WEMOS_Motor.h>
#include "wifi.h"

int pwmSpeed = 100;

//Motor shield I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 1000);
Motor M2(0x30, _MOTOR_B, 1000); //Motor B

WFclass wifi;
ESP8266WebServer server(80);

String ssid("minibot");
String password("minibot");
String mDnsName("minibot");

void setup()
{
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  Serial.println("Motor A&B STOP");
  delay(200);

  Serial.begin(9600);
  Serial.println("Minibot starting...");
  //wifi.connectFromList();
  wifi.softAP("minibot", "minibot");

  /* Registrando as funções de callback para as rotas desejadas: */
  server.on("/", handleRoot); /* Rota raiz. */
  server.on("/fw", handleFw);
  server.on("/bw", handleBw);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);

  /* Finalmente inicializa o servidor webserver, com as configurações feitas. */
  server.begin();
  Serial.println("Servidor HTTP rodando...");

  if(!MDNS.begin("minibot")) Serial.println("Erro ao configurar serviço mDNS.");
  else Serial.println("mDNS rodando sob o nome " + mDnsName + ".local");
}

void loop()
{
  server.handleClient();
}

void handleRoot()
{
  Serial.print("Recebendo requisição de ");
  Serial.println(server.client().remoteIP());

  String webString =
    "<!DOCTYPE HTML><html><head>"
    "<meta name='apple-mobile-web-app-capable' content='yes'>"
    "<style>body{background-color:#4285F4;font-size:60px;font-family:verdana;}</style>"
    "<script>"
    "function fw(){window.location.assign('/fw')}"
    "function bw(){window.location.assign('/bw')}"
    "function left(){window.location.assign('/left')}"
    "function right(){window.location.assign('/right')}"
    "</script>"
    "</head>"
    "<body><center>"
    "<div><bold>Minibot</div></br>"
    "</br><div>"
    "<input type='button' style='font-size:80px' value='FW' onclick='fw()'></br></br>"
    "<input type='button' style='font-size:80px' value='BW' onclick='bw()'></div>"
    "</center></body></html>";
  server.send(200, "text/html", webString);

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleFw()
{
  M1.setmotor(_CW, pwmSpeed);
  M2.setmotor(_CW, pwmSpeed);
  handleRoot();
}

void handleBw()
{
  M1.setmotor(_CCW, pwmSpeed);
  M2.setmotor(_CCW, pwmSpeed);
  handleRoot();
}

void handleLeft()
{
  M1.setmotor(_STOP);
  M2.setmotor(_CW, pwmSpeed);
  handleRoot();
}

void handleRight()
{
  M1.setmotor(_CW, pwmSpeed);
  M2.setmotor(_STOP);
  handleRoot();
}

void handleStop()
{
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  handleRoot();
}

