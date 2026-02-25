#include "gmail.h"
#include <ArduinoJson.h>
#include <base64.h>

GmailModule::GmailModule(OAuthManager *oauthManager,
                         OAuthToken *oauthToken,
                         const String &email)
    : oauth(oauthManager), token(oauthToken), userEmail(email) {}

const char* GmailModule::name() { return "gmail"; }

void GmailModule::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected! Gmail module cannot start.");
        return;
    }
    Serial.printf("Gmail module ready for user: %s\n", userEmail.c_str());
}

bool GmailModule::send(const OpenRF_Packet &pkt) {
    if (!oauth->isTokenValid(*token))
        oauth->refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", *token);

    String msg((char*)pkt.payload, pkt.len);
    String emailContent = "To: " + userEmail + "\r\n";
    emailContent += "Subject: OpenRF Message\r\n\r\n";
    emailContent += msg;

    String base64Email = base64::encode(emailContent);

    HTTPClient http;
    http.begin(GmailLinks::SEND_MESSAGE);
    http.addHeader("Authorization", "Bearer " + token->accessToken);
    http.addHeader("Content-Type", "application/json");

    String body = "{\"raw\":\"" + base64Email + "\"}";
    int code = http.POST(body);
    http.end();

    if (code >= 200 && code < 300) {
        Serial.printf("[Gmail] Sent: %s\n", msg.c_str());
        return true;
    }
    Serial.printf("[Gmail] Failed to send (%d)\n", code);
    return false;
}

void GmailModule::loop() {
    // Optional: poll Gmail inbox using GmailLinks::LIST_MESSAGES
    // Convert new emails into OpenRF_Packet and call rxCallback
}

void GmailModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
