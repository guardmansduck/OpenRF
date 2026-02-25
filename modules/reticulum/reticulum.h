#pragma once
#include "../../core/transport.h"
#include "../../core/packet.h"
#include <Arduino.h>

class ReticulumModule : public Transport {
private:
    void (*rxCallback)(const OpenRF_Packet&) = nullptr;
public:
    ReticulumModule() {}
    const char* name() override { return "reticulum"; }
    void begin() override;
    bool send(const OpenRF_Packet &pkt) override;
    void loop() override;
    void setReceiveCallback(void (*cb)(const OpenRF_Packet&)) override;
};
