#include "WiFiManagerESP32.h"
#include "webpages.h"
#include "esp_netif.h"
#include "esp_wifi.h"

WiFiManagerESP32::WiFiManagerESP32() {
    server = new WebServer(80);
}

void WiFiManagerESP32::setSsid(String ssid) {
    this->ssid = ssid;
}

void WiFiManagerESP32::begin() {
    EEPROM.begin(EEPROM_SIZE);

    char storedSsid[32] = {0};
    char storedPass[64] = {0};
    char storedDevice[64] = {0};

    if (loadCredentials(storedSsid, storedPass, storedDevice)) {
        deviceName = storedDevice;
        Serial.println("Found WiFi-Credentials, Connecting...");
        WiFi.disconnect(true, true); 
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);      
        connectToWiFi(storedSsid, storedPass);
        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
            delay(500);
            Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED) {            
            Serial.println("\nWifi successfully connected!");       
            startClientMode();
            return;
        }
        Serial.println("\nWifi connection failed!");
    }
    startAPMode();
}

bool WiFiManagerESP32::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManagerESP32::startClientMode() {
    IPAddress myIP = WiFi.localIP();
    Serial.println("Start REST API Service");

    // OPTIONS-Handler
    server->on("/", HTTP_OPTIONS, std::bind(&WiFiManagerESP32::handleCorsPreflight, this));
    server->on("/ping", HTTP_OPTIONS, std::bind(&WiFiManagerESP32::handleCorsPreflight, this));
    server->on("/reset", HTTP_OPTIONS, std::bind(&WiFiManagerESP32::handleCorsPreflight, this));
    server->on("/cmd", HTTP_OPTIONS, std::bind(&WiFiManagerESP32::handleCorsPreflight, this));

    // GET/POST-Handler
    server->on("/", std::bind(&WiFiManagerESP32::handleApp, this));
    server->on("/ping", std::bind(&WiFiManagerESP32::handlePing, this));
    server->on("/reset", std::bind(&WiFiManagerESP32::handleReset, this));
    server->on("/cmd", std::bind(&WiFiManagerESP32::handleCmd, this));

    server->onNotFound([this]() {
        sendCORSHeaders();
        server->send(404, "application/json", "{\"error\":\"Route not found\"}");
    });

    server->begin();
    Serial.print("REST API Service on http://");
    Serial.print(myIP);
    Serial.println(":80");
}

void WiFiManagerESP32::startAPMode() {
    Serial.println("Starting AccessPoint Modus...");
    WiFi.mode(WIFI_AP);
    if (ssid.length() == 0) ssid = "ESP32_Setup";
    WiFi.softAP(ssid.c_str());

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(myIP);

    // DNS redirect
    bool dnsOk = dns.start(53, "*", myIP);
    Serial.print("DNS Server status: ");
    Serial.println(dnsOk ? "OK" : "FAILED");

    // Captive Portal Endpoints
    server->on("/", std::bind(&WiFiManagerESP32::handleRoot, this));
    server->on("/save", std::bind(&WiFiManagerESP32::handleSave, this));

    // Android/iOS/Linux/Captive Portal URLs
    const char* captiveUrls[] = {
        "/generate_204", "/gen_204", "/hotspot-detect.html",
        "/kindle-wifi/wifistub.html", "/success.txt",
        "/check_network_status.txt", "/library/test/success.html",
        "/connecttest.txt", "/ncsi.txt", "/204",
        "/ipv6check", "/fwlink"
    };

    for (auto url : captiveUrls) {
        server->on(url, HTTP_ANY, [this, url]() { handleCaptivePortal(url); });
    }

    server->onNotFound([this]() { handleRoot(); });

    server->begin();
    Serial.println("Captive Portal started. Connect to '" + ssid + "'.");
}

void WiFiManagerESP32::sendCORSHeaders() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    server->sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,OPTIONS");
    server->sendHeader("Access-Control-Allow-Headers", "*");
}

void WiFiManagerESP32::handleCorsPreflight() {
    sendCORSHeaders();
    server->send(204);  // No Content
}

void WiFiManagerESP32::handleCaptivePortal(String uri) {
    Serial.println("Captive Portal Request: " + uri);
    if (uri == "/connecttest.txt" || uri == "/ncsi.txt" ||
        uri == "/generate_204" || uri == "/gen_204" ||
        uri == "/204" || uri == "/ipv6check" ||
        uri == "/hotspot-detect.html" ||
        uri == "/success.txt" || uri == "/check_network_status.txt") {
        Serial.printf("Captive Portal Trigger: %s\n", uri.c_str());
        server->sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
        server->send(302, "text/plain", "");
        return;
    }
    else if (uri == "/fwlink") {server->sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true); server->send(302, "text/plain", "");}
    else {handleRoot();}
}

void WiFiManagerESP32::handleRoot() {
    String page(index_html);
    String options;

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) options += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";

    page.replace("%SSID_OPTIONS%", options);
    page.replace("%SVG_LOGO%", FPSTR(svg_logo));
    server->send(200, "text/html", page);
}

void WiFiManagerESP32::handleSave() {
    String page(saved_html);
    page.replace("%SVG_LOGO%", FPSTR(svg_logo));
    if (!server->hasArg("ssid") || !server->hasArg("pass") || !server->hasArg("device")) {
        server->send(400, "text/html", "Error Reading SSID, password, or devicename");
        return;
    }

    String devName = server->arg("device");
    saveCredentials(server->arg("ssid").c_str(), server->arg("pass").c_str(), devName.c_str());
    server->send(200, "text/html", page);
    delay(2000);
    ESP.restart();
}

void WiFiManagerESP32::handleReset() {
    sendCORSHeaders();
    if (server->method() == HTTP_OPTIONS) return;
    Serial.println("Resetting EEPROM and WiFi credentials...");
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
    server->send(200, "application/json", "{\"status\":\"EEPROM cleared, device reset\"}");
    delay(500);
    ESP.restart();    
}

void WiFiManagerESP32::handlePing() {
    sendCORSHeaders();
    IPAddress ip = WiFi.localIP();
    String device = deviceName.length() ? deviceName : "esp32-d2";
    String json = "{";
    json += "\"ip\":\"" + ip.toString() + "\",";
    json += "\"device\":\"" + device + "\"}";
    server->send(200, "application/json", json);
}

void WiFiManagerESP32::handleCmd() {
    sendCORSHeaders();
    if (cmdCallback) {
        cmdCallback(*server);
    } else {
        server->send(404, "application/json", "{\"error\":\"No CMD handler set\"}");
    }
}

void WiFiManagerESP32::handleNotFound() {
    sendCORSHeaders();
    server->send(404, "application/json", "{\"error\":\"Route not found\"}");
}

void WiFiManagerESP32::handleApp() {
    sendCORSHeaders();
    String page(app_html);
    page.replace("%SVG_LOGO%", FPSTR(svg_logo));
    page.replace("%DEVICE_NAME%", deviceName);
    server->send(200, "text/html", page);
}

void WiFiManagerESP32::setCmdCallback(CmdCallback cb) { 
    cmdCallback = cb; 
}

void WiFiManagerESP32::saveCredentials(const char* ssid, const char* password, const char* deviceName) {
    for (int i = 0; i < 32; i++) EEPROM.write(SSID_ADDR + i, ssid[i]);
    for (int i = 0; i < 64; i++) EEPROM.write(PASS_ADDR + i, password[i]);
    for (int i = 0; i < 64; i++) EEPROM.write(DEVICE_ADDR + i, deviceName[i]);
    
    EEPROM.commit();
    
    Serial.println("WiFi-Credentials + Device Name saved.");
}

bool WiFiManagerESP32::loadCredentials(char* ssid, char* password, char* deviceName) {
    memset(ssid, 0, 32);
    memset(password, 0, 64);
    memset(deviceName, 0, 64);

    if (EEPROM.read(SSID_ADDR) == 0xFF) return false;

    for (int i = 0; i < 32; i++) ssid[i] = EEPROM.read(SSID_ADDR + i);
    for (int i = 0; i < 64; i++) password[i] = EEPROM.read(PASS_ADDR + i);
    for (int i = 0; i < 64; i++) deviceName[i] = EEPROM.read(DEVICE_ADDR + i);
    return true;
}

void WiFiManagerESP32::connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    Serial.println("Connecting to: " + String(ssid));
}

void WiFiManagerESP32::process() {
    // Connection Handler
    if (!isConnected()) dns.processNextRequest();
    server->handleClient();

    // Serial Handler
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command == "GET_IP") {
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println(WiFi.localIP().toString());
            } else {
                Serial.println("NOT_CONNECTED");
            }
        }
    }
}