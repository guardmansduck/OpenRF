#include "discord.h"

DiscordModule::DiscordModule(OAuthManager *oauthManager,
                             OAuthToken *oauthToken,
                             const String &channel)
    : oauth(oauthManager), token(oauthToken), channelId(channel) {}

const char* DiscordModule::name() {
    return "discord";
}

void DiscordModule::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected! Discord module cannot start.");
        return;
    }

    connectGateway();
}

void DiscordModule::connectGateway() {
    ws.begin(DiscordLinks::GATEWAY_URL);
    ws.onEvent([this](WStype_t type, uint8_t *payload, size_t length) {
        if (type == WStype_TEXT) {
            handleIncomingMessage(String((char*)payload));
        }
    });
    ws.setReconnectInterval(5000);
    ws.enableHeartbeat(true);
}

void DiscordModule::handleIncomingMessage(const String &msg) {
    OpenRF_Packet pkt;
    pkt.src = 0xDDDDDDDD;
    pkt.dst = 0xFFFFFFFF;
    pkt.len = msg.length() > 220 ? 220 : msg.length();
    memcpy(pkt.payload, msg.c_str(), pkt.len);
    pkt.crc = crc16(pkt.payload, pkt.len);

    if (rxCallback) rxCallback(pkt);
}

bool DiscordModule::send(const OpenRF_Packet &pkt) {
    if (!oauth->isTokenValid(*token)) {
        oauth->refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", *token);
    }
    String msg((char*)pkt.payload, pkt.len);
    Serial.printf("[Discord] Sending: %s\n", msg.c_str());
    // Real implementation: POST to Discord API using token->accessToken
    return true;
}

void DiscordModule::loop() {
    ws.loop();
}

void DiscordModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
