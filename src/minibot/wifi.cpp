/*
   Minibot

   Felipe Kühne
   29/10/2018

*/

#include "wifi.h"

/* Contrutor da classe. Não faz nada, só instancia o objeto */
WFclass::WFclass() {
  /* Read this:
      https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-class.html?highlight=wifi#persistent */
  WiFi.persistent(false);
}

/* Inicia a conexão do cliente Wi-Fi com a rede disponível no local. */
void WFclass::connect()
{
  WiFi.mode(WIFI_STA);

  Serial.print("Conectando na rede: ");
  Serial.print(c_ssid);

  /* Inicia o cliente Wi-Fi, conectando à rede especificada, de acordo com
       as credenciais de acessi (SSID e senha). */
  int status = WiFi.begin(c_ssid, c_pass);

  /* O ESP tende a demorar para conectar em redes instáveis. Quando isto
       ocorre, a melhor alternativa é causar um reset na placa e tentar
       novamente. Isto é o que o código abaixo faz. */
  for (int count = 0; count <= 30; count++)
  {
    if (WiFi.status() == WL_CONNECTED)
      break;

    delay(1000);
    Serial.print(".");

    if (count == 30)
    {
      Serial.println(" desisto :(");
      ESP.restart();
    }
  }

  myIP = WiFi.localIP();

  Serial.println(" conectado!");
  Serial.print(" MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print(" IP: ");
  Serial.println(WiFi.localIP());
  Serial.print(" Subrede: ");
  Serial.println(WiFi.subnetMask());
  Serial.print(" Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print(" DNS: ");
  Serial.println(WiFi.dnsIP());
  Serial.print(" Canal: ");
  Serial.println(WiFi.channel());
  Serial.print(" Status: ");
  Serial.println(WiFi.status());
  Serial.println();
}

struct wifinetworks {
  String ssid;
  String pass;
} wifiNetworks[] = {
  {"EVENTOS", "campus2018"},
  {"GVT-A339", "0073162810"},
  {"DBDevices", "!mP@db@dM"},
  {"DBLABDevices", "@rdu!n0$"},
  {"DB_4G_Alessandra", "w1f1_alessandra"},
  {"Paulo Silva", "w1f1dbpo@"},
  {"dblab-movel", "@rdu!n0$"},
};

/* Tenta conectar com alguma rede no local, a partir de uma lista pré-definida. */
void WFclass::connectFromList()
{
  WiFi.mode(WIFI_STA);

  int wifiListSize = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);

  for (int i = 0; i < wifiListSize; i++)
  {
    Serial.print("Tentando conectar na rede " + wifiNetworks[i].ssid);

    int status = WiFi.begin(wifiNetworks[i].ssid.c_str(), wifiNetworks[i].pass.c_str());
    for (int count = 0; count <= 10; count++)
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        myIP = WiFi.localIP();

        Serial.println(" conectado!");
        Serial.print(" MAC: ");
        Serial.println(WiFi.macAddress());
        Serial.print(" IP: ");
        Serial.println(WiFi.localIP());
        return;
      }

      delay(500);
      Serial.print(".");
    }
    Serial.println();
  }

  /* Se chegou até aqui, não conseguiu se conectar em nenhuma rede. */
  Serial.println(" desisto :(");
  ESP.restart();
}

/* Cria um access point. */
IPAddress WFclass::softAP(const String ssid, const String password)
{
  WiFi.mode(WIFI_AP);
  
  if (password.length()) WiFi.softAP(ssid.c_str(), password.c_str());
  else WiFi.softAP(ssid.c_str());

  Serial.print("Criando rede ");
  Serial.print(ssid);
  Serial.print(" (pwd: ");
  if (password.length()) Serial.print(password);
  Serial.println(")");
  myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  return myIP;
}

/* Cria um access point com IP definido. */
IPAddress WFclass::softAP(const IPAddress ip, const String ssid, const String password)
{
  WiFi.mode(WIFI_AP);
  
  WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));

  if (password.length()) WiFi.softAP(ssid.c_str(), password.c_str());
  else WiFi.softAP(ssid.c_str());

  Serial.print("Criando rede ");
  Serial.print(ssid);
  Serial.print(" (pwd: ");
  if (password.length()) Serial.print(password);
  Serial.println(")");
  myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  return myIP;
}

