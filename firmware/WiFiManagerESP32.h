#ifndef WIFI_MANAGER_ESP32_H
#define WIFI_MANAGER_ESP32_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

#define EEPROM_SIZE 128
#define SSID_ADDR   0
#define PASS_ADDR   64

class WiFiManagerESP32 {
  public:
    WiFiManagerESP32();
    void setSsid(String ssid);
    void begin();
    bool isConnected();
    void process();

  private:
    WebServer* server;
    DNSServer dns;
    String ssid;

    void startAPMode();
    void handleRoot();
    void handleSave();
    void saveCredentials(const char* ssid, const char* password);
    bool loadCredentials(char* ssid, char* password);
    void connectToWiFi(const char* ssid, const char* password);
    void handleCaptivePortal(String uri);
};

#endif
