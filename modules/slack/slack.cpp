#include "slack.h"
#include <ArduinoJson.h>

SlackModule::SlackModule(OAuthManager *oauthManager,
                         OAuthToken *oauthToken,
                         const String &channel)
    : oauth(oauthManager), token(oauthToken), channelId(channel) {}

const char* SlackModule::name() { return "slack"; }

void SlackModule::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected! Slack module cannot start.");
        return;
    }
    Serial.printf("Slack module ready for channel: %s\n", channelId.c_str());
}

bool SlackModule::send(const OpenRF_Packet &pkt) {
    if (!oauth->isTokenValid(*token))
        oauth->refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", *token);

    String msg((char*)pkt.payload, pkt.len);

    HTTPClient http;
    String url = "https://slack.com/api/chat.postMessage";
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + token->accessToken);
    http.addHeader("Content-Type", "application/json");

    String body = "{\"channel\":\"" + channelId + "\",\"text\":\"" + msg + "\"}";
    int code = http.POST(body);
    http.end();

    if (code >= 200 && code < 300) {
        Serial.printf("[Slack] Sent: %s\n", msg.c_str());
        return true;
    }
    Serial.printf("[Slack] Failed to send (%d)\n", code);
    return false;
}

void SlackModule::loop() {
    // Poll messages via Slack Events API if needed
    // Could implement here to receive messages and call rxCallback
}

void SlackModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
