/*
   Minibot

   Felipe Kühne
   29/10/2018
*/

#include <DNSServer.h>
#include <WEMOS_Motor.h>
#include "wifi.h"
#include "html/index.h"

const char compile_date[] = __DATE__ " " __TIME__;
String version("0.0-1");
int pwmSpeed = 100;

//Motor shield I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 1000);
Motor M2(0x30, _MOTOR_B, 1000); //Motor B

WFclass wifi;
ESP8266WebServer server(80);
DNSServer dnsServer;

IPAddress myIP(192, 168, 1, 1);
String ssid("minibot");

void printBanner()
{
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

  wifi.softAP(myIP, ssid);
  dnsServer.start(53, "*", myIP);

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
  Serial.println("Servidor HTTP rodando...");

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  dnsServer.processNextRequest();
  server.handleClient();
}

void handleRoot()
{
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("Recebendo requisição de ");
  Serial.println(server.client().remoteIP());
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s);

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleRun()
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

void handleOrientation()
{
  Serial.println(server.arg("x"));  
  Serial.println(server.arg("y"));
  // How to read content from client?
  //handleRoot();
}

