#include "lora.h"

const char* LoRaModule::name() { return "lora"; }

void LoRaModule::begin() {
    BoardPins pins = detectBoardPins();
    Serial.printf("LoRa pins: NSS=%d, RESET=%d, DIO0=%d\n", pins.ss, pins.reset, pins.dio0);

    SPI.begin();
    LoRa.setPins(pins.ss, pins.reset, pins.dio0);

    if (!LoRa.begin(915E6)) {
        Serial.println("LoRa init failed!");
        while(1);
    }

    LoRa.onReceive([](int packetSize){
        if (packetSize == sizeof(OpenRF_Packet)) {
            OpenRF_Packet pkt;
            for (int i = 0; i < packetSize; i++)
                ((uint8_t*)&pkt)[i] = LoRa.read();
            if (LoRaModule::rxCallback)
                LoRaModule::rxCallback(pkt);
        }
    });

    LoRa.receive();
}

bool LoRaModule::send(const OpenRF_Packet &pkt) {
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&pkt, sizeof(OpenRF_Packet));
    return LoRa.endPacket() == 1;
}

void LoRaModule::loop() {}

void LoRaModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
