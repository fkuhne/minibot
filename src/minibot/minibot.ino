/*
   Minibot

   Felipe Kühne
   29/10/2018

*/

#include <ESP8266mDNS.h>
#include <WEMOS_Motor.h>
#include "wifi.h"

const char compile_date[] = __DATE__ " " __TIME__;
String version("0.0-1");
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

String css(".garden {position:relative; width:200px; height:200px; border:5px solid #CCC; border-radius:10px;} .ball {position:absolute; top:90px; left:90px;width:20px; height:20px; background:green; border-radius:100%;}");

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
  Serial.begin(9600);
  Serial.println("Minibot starting...");
  printBanner();
  
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  Serial.println("Motor A&B STOP");
  delay(200);

  //wifi.connectFromList();
  wifi.softAP("minibot", "minibot");

  /* Registrando as funções de callback para as rotas desejadas: */
  server.on("/", handleRoot); /* Rota raiz. */
  server.on("/fw", handleFw);
  server.on("/bw", handleBw);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);
  server.on("/orientation", handleOrientation);

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
    "<style type=\"text/css\">" + css + "</style>"
    "<script>"
    "function fw(){window.location.assign('/fw')}"
    "function bw(){window.location.assign('/bw')}"
    "function left(){window.location.assign('/left')}"
    "function right(){window.location.assign('/right')}"
    "function handleOrientation(event){var x = event.beta; var y=event.gamma;"
    "output.innerHTML  = \"beta : \" + x + \"\n\";"
    "output.innerHTML += \"gamma: \" + y + \"\n\";"
    "if (x >  90) { x =  90}; if (x < -90) { x = -90};"
    "x += 90; y += 90;}"
    "window.addEventListener('deviceorientation', handleOrientation);"
    "</script>"
    "</head>"
    "<body><center>"
    "<div class=\"garden\"><div class=\"ball\"></div></div><pre class=\"output\"></pre>"
    "<div><bold>Minibot</div></br>"
    "</br><div>"
    "<input type='button' style='font-size:80px' value='FW' onmousedown='fw()'></br></br>"
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

void handleOrientation()
{
  // How to read content from client?
  handleRoot();
}

