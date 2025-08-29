#include "WiFiManagerESP32.h"
#include "webpages.h"
#include "esp_netif.h"

WiFiManagerESP32::WiFiManagerESP32() {
    server = new WebServer(80);
}

void WiFiManagerESP32::setSsid(String ssid) {
    this->ssid = ssid;
}

void WiFiManagerESP32::begin() {
    EEPROM.begin(EEPROM_SIZE);

    char storedSsid[32];
    char storedPass[64];

    if (loadCredentials(storedSsid, storedPass)) {
        Serial.println("Found WiFi-Credentials, Connecting...");
        connectToWiFi(storedSsid, storedPass);

        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
            delay(500);
            Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWifi successfully connected!");
            return;
        }
        Serial.println("\nWifi connection failed!");
    }

    startAPMode();
}

bool WiFiManagerESP32::isConnected() {
    return WiFi.status() == WL_CONNECTED;
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
    if (!server->hasArg("ssid") || !server->hasArg("pass")) {
        server->send(400, "text/html", "Fehler: SSID oder Passwort fehlt!");
        return;
    }

    saveCredentials(server->arg("ssid").c_str(), server->arg("pass").c_str());
    server->send(200, "text/html", page);
    delay(2000);
    ESP.restart();
}

void WiFiManagerESP32::saveCredentials(const char* ssid, const char* password) {
    for (int i = 0; i < 32; i++) EEPROM.write(SSID_ADDR + i, ssid[i]);
    for (int i = 0; i < 64; i++) EEPROM.write(PASS_ADDR + i, password[i]);
    EEPROM.commit();
    Serial.println("WiFi-Credentials saved.");
}

bool WiFiManagerESP32::loadCredentials(char* ssid, char* password) {
    if (EEPROM.read(SSID_ADDR) == 0xFF) return false;
    for (int i = 0; i < 32; i++) ssid[i] = EEPROM.read(SSID_ADDR + i);
    for (int i = 0; i < 64; i++) password[i] = EEPROM.read(PASS_ADDR + i);
    return true;
}

void WiFiManagerESP32::connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    Serial.println("Connecting to: " + String(ssid));
}

void WiFiManagerESP32::process() {
    dns.processNextRequest();
    server->handleClient();
}
