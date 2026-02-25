#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct OAuthToken {
    String accessToken;
    String refreshToken;
    uint32_t expiresAt;
};

class OAuthManager {
public:
    OAuthManager() {}
    String getAuthUrl(const String &clientId,
                      const String &redirectUri,
                      const String &scope,
                      const String &state);
    bool exchangeCodeForToken(const String &clientId,
                              const String &clientSecret,
                              const String &redirectUri,
                              const String &code,
                              OAuthToken &token);
    bool refreshAccessToken(const String &clientId,
                            const String &clientSecret,
                            OAuthToken &token);
    bool isTokenValid(const OAuthToken &token);
};
