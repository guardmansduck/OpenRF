#include "meshtastic.h"
#include <SPI.h>
#include <LoRa.h> // Assuming you have Meshtastic-compatible LoRa library

void MeshtasticModule::begin() {
    BoardPins pins = detectBoardPins();
    LoRa.setPins(pins.ss, pins.reset, pins.dio0);
    if (!LoRa.begin(915E6)) { 
        Serial.println("Meshtastic LoRa init failed!"); 
    } else { 
        Serial.println("Meshtastic LoRa ready."); 
    }
}

bool MeshtasticModule::send(const OpenRF_Packet &pkt) {
    LoRa.beginPacket();
    LoRa.write(pkt.payload, pkt.len);
    LoRa.endPacket();
    return true;
}

void MeshtasticModule::loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        OpenRF_Packet pkt;
        pkt.len = LoRa.readBytes(pkt.payload, packetSize);
        pkt.src = 0xMESHTASTIC; // Unique source ID
        if (rxCallback) rxCallback(pkt);
    }
}

void MeshtasticModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
