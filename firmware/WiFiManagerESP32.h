#ifndef WIFI_MANAGER_ESP32_H
#define WIFI_MANAGER_ESP32_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

#define EEPROM_SIZE 192
#define SSID_ADDR   0
#define PASS_ADDR   64
#define EEPROM_SIZE 192
#define DEVICE_ADDR 128

class WiFiManagerESP32 {
  public:
    using CmdCallback = std::function<void(WebServer&)>;

    WiFiManagerESP32();
    void setSsid(String ssid);
    void begin();
    bool isConnected();
    void process();
    void setCmdCallback(CmdCallback cb);
    String getDeviceName() const { return deviceName; }

  private:
    WebServer* server;
    DNSServer dns;
    String ssid;
    String deviceName;
    CmdCallback cmdCallback;

    void startAPMode();
    void startClientMode();
    void handleRoot();
    void handleSave();
    void handleReset();
    void handlePing();
    void handleCmd();
    void handleNotFound();
    void handleRestApi();
    void saveCredentials(const char* ssid, const char* password, const char* deviceName);
    bool loadCredentials(char* ssid, char* password, char* deviceName);
    void connectToWiFi(const char* ssid, const char* password);
    void handleCaptivePortal(String uri);
};

#endif
