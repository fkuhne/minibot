/*
 * Minibot
 * 
 * Felipe Kühne
 * 29/10/2018
 * 
 */

#ifndef wifi_h
#define wifi_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

class WFclass
{
  public:
    WFclass();
    void connect();
    void connectFromList();
    IPAddress softAP(const String ssid, const String password = "");
    IPAddress softAP(const IPAddress ip, const String ssid, const String password = "");

  private:
    const char* c_ssid = "";
    const char* c_pass = "";
    IPAddress myIP;
};

#endif


