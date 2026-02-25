#include "sms.h"
#include <ArduinoJson.h>

SMSModule::SMSModule(OAuthManager *oauthManager,
                     OAuthToken *oauthToken,
                     const String &phone)
    : oauth(oauthManager), token(oauthToken), phoneNumber(phone) {}

const char* SMSModule::name() { return "sms"; }

void SMSModule::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected! SMS module cannot start.");
        return;
    }
    Serial.printf("SMS module ready for number: %s\n", phoneNumber.c_str());
}

bool SMSModule::send(const OpenRF_Packet &pkt) {
    if (!oauth->isTokenValid(*token))
        oauth->refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", *token);

    String msg((char*)pkt.payload, pkt.len);

    HTTPClient http;
    String url = SMSLinks::API_BASE + "/send";
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + token->accessToken);
    http.addHeader("Content-Type", "application/json");

    String body = "{\"to\":\"" + phoneNumber + "\",\"message\":\"" + msg + "\"}";
    int code = http.POST(body);
    http.end();

    if (code >= 200 && code < 300) {
        Serial.printf("[SMS] Sent: %s\n", msg.c_str());
        return true;
    }
    Serial.printf("[SMS] Failed to send (%d)\n", code);
    return false;
}

void SMSModule::pollMessages() {
    if (!oauth->isTokenValid(*token))
        oauth->refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", *token);

    HTTPClient http;
    String url = SMSLinks::API_BASE + "/inbox?to=" + phoneNumber;
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + token->accessToken);

    int code = http.GET();
    if (code == 200) {
        String resp = http.getString();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, resp);

        for (JsonObject msg : doc.as<JsonArray>()) {
            String body = msg["message"];
            OpenRF_Packet pkt;
            pkt.src = 0xABCDEF01;
            pkt.dst = 0xFFFFFFFF;
            pkt.len = body.length() > 220 ? 220 : body.length();
            memcpy(pkt.payload, body.c_str(), pkt.len);
            pkt.crc = crc16(pkt.payload, pkt.len);
            if (rxCallback) rxCallback(pkt);
        }
    }
    http.end();
}

void SMSModule::loop() { pollMessages(); }

void SMSModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
