#pragma once
#include "../../core/transport.h"
#include "../../core/packet.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../../oauth/oauth.h"
#include "../../config/links.h"

class MeshtasticModule : public Transport {
private:
    void (*rxCallback)(const OpenRF_Packet&) = nullptr;

public:
    MeshtasticModule() {}
    const char* name() override { return "meshtastic"; }
    void begin() override;
    bool send(const OpenRF_Packet &pkt) override;
    void loop() override;
    void setReceiveCallback(void (*cb)(const OpenRF_Packet&)) override;
};
