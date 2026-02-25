#pragma once
#include "../../core/transport.h"
#include "../../core/packet.h"
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

class LoRaModule : public Transport {
private:
    void (*rxCallback)(const OpenRF_Packet&) = nullptr;

    // Auto-detect SPI pins for board
    void detectPins(uint8_t &ssPin, uint8_t &resetPin, uint8_t &dio0Pin);

public:
    const char* name() override;
    void begin() override;
    bool send(const OpenRF_Packet &pkt) override;
    void loop() override;
    void setReceiveCallback(void (*cb)(const OpenRF_Packet&)) override;
};
