#include "lora.h"

const char* LoRaModule::name() {
    return "lora";
}

// Board auto-detection
void LoRaModule::detectPins(uint8_t &ssPin, uint8_t &resetPin, uint8_t &dio0Pin) {
    String chip = ESP.getChipModel();
    int cores = ESP.getChipCores();

    // Example profiles (expand as needed)
    if (chip == "ESP32D0WDQ6") {          // T-Beam / LilyGo
        ssPin = 5;
        resetPin = 14;
        dio0Pin = 2;
    } else if (chip == "ESP32-WROOM-32") { // Heltec LoRa
        ssPin = 18;
        resetPin = 14;
        dio0Pin = 26;
    } else {                               // default fallback
        ssPin = 5;
        resetPin = 14;
        dio0Pin = 2;
    }

    Serial.printf("LoRa pins auto-detected: NSS=%d, RESET=%d, DIO0=%d\n", ssPin, resetPin, dio0Pin);
}

void LoRaModule::begin() {
    uint8_t ss, rst, dio0;
    detectPins(ss, rst, dio0);

    SPI.begin();                  // Start SPI bus
    LoRa.setPins(ss, rst, dio0);

    if (!LoRa.begin(915E6)) {
        Serial.println("LoRa init failed!");
        while (1);
    }

    LoRa.onReceive([](int packetSize){
        if (packetSize == sizeof(OpenRF_Packet)) {
            OpenRF_Packet pkt;
            for (int i = 0; i < packetSize; i++) {
                ((uint8_t*)&pkt)[i] = LoRa.read();
            }
            if (LoRaModule::rxCallback) {
                LoRaModule::rxCallback(pkt);
            }
        }
    });

    LoRa.receive();
}

bool LoRaModule::send(const OpenRF_Packet &pkt) {
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&pkt, sizeof(OpenRF_Packet));
    return LoRa.endPacket() == 1;
}

void LoRaModule::loop() {
    // LoRa library handles receive asynchronously
}

void LoRaModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
