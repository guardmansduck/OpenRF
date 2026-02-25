#pragma once
#include "../../core/transport.h"
#include "../../core/packet.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../../oauth/oauth.h"
#include "../../config/links.h"

class SMSModule : public Transport {
private:
    void (*rxCallback)(const OpenRF_Packet&) = nullptr;
    OAuthManager *oauth;
    OAuthToken *token;
    String phoneNumber;  // the number this module sends from / listens to

    void pollMessages(); // optional for pulling SMS if API supports it

public:
    SMSModule(OAuthManager *oauthManager,
              OAuthToken *oauthToken,
              const String &phone);

    const char* name() override;
    void begin() override;
    bool send(const OpenRF_Packet &pkt) override;
    void loop() override;
    void setReceiveCallback(void (*cb)(const OpenRF_Packet&)) override;
};
