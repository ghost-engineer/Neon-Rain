#include <WiFi.h>
#include <vector>
#include <string>

// Wi-Fi settings (can be left empty for scanning)
const char* ssid = "";
const char* password = "";

// "Rain" parameters
const int termWidth = 40; // terminal width
const int termHeight = 20; // terminal height
const char rainChars[] = "|/\\-";

// Get Wi-Fi signal strength (RSSI)
int getWiFiStrength() {
    int n = WiFi.scanNetworks();
    int bestRSSI = -100;
    for (int i = 0; i < n; ++i) {
        int rssi = WiFi.RSSI(i);
        if (rssi > bestRSSI) bestRSSI = rssi;
    }
    return bestRSSI;
}

// Convert RSSI to "rain intensity"
int rssiToRainDensity(int rssi) {
    // -100 (bad) ... -30 (excellent)
    int norm = constrain(rssi, -100, -30) + 100;
    return map(norm, 0, 70, 1, termWidth / 2);
}

// Clear terminal
void clearScreen() {
    Serial.write("\033[2J\033[H");
}

// Generate and print "rain"
void printRain(int density) {
    std::vector<std::string> screen(termHeight, std::string(termWidth, ' '));
    for (int d = 0; d < density; ++d) {
        int col = random(0, termWidth);
        int len = random(3, termHeight);
        char c = rainChars[random(0, sizeof(rainChars) - 1)];
        for (int row = 0; row < len; ++row) {
            if (row < termHeight) screen[row][col] = c;
        }
    }
    for (const auto& line : screen) {
        Serial.println(line.c_str());
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("\nESP32 Wi-Fi Rain: RSSI -> Matrix");
}

void loop() {
    int rssi = getWiFiStrength();
    int density = rssiToRainDensity(rssi);
    clearScreen();
    Serial.printf("Wi-Fi RSSI: %d dBm\n", rssi);
    printRain(density);
    delay(300);
}
