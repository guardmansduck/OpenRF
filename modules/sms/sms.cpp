#include "sms.h"

SMSModule::SMSModule(OAuthManager *oauthManager,
                     OAuthToken *oauthToken,
                     const String &phone)
    : oauth(oauthManager), token(oauthToken), phoneNumber(phone) {}

const char* SMSModule::name() {
    return "sms";
}

void SMSModule::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected! SMS module cannot start.");
        return;
    }
    Serial.printf("SMS module ready for number: %s\n", phoneNumber.c_str());
}

bool SMSModule::send(const OpenRF_Packet &pkt) {
    if (!oauth->isTokenValid(*token)) {
        oauth->refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", *token);
    }

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
    } else {
        Serial.printf("[SMS] Failed to send (%d)\n", code);
        return false;
    }
}

void SMSModule::pollMessages() {
    // Optional: implement polling if the API supports retrieving incoming messages
    // Convert incoming SMS into OpenRF_Packet and call rxCallback
}

void SMSModule::loop() {
    pollMessages(); // poll new messages every loop
}

void SMSModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
