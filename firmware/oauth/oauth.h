#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct OAuthToken {
    String accessToken;
    String refreshToken;
    uint32_t expiresAt; // Unix timestamp when token expires
};

class OAuthManager {
public:
    OAuthManager() {}

    // Get URL for user authorization
    String getAuthUrl(const String &clientId,
                      const String &redirectUri,
                      const String &scope,
                      const String &state);

    // Exchange authorization code for access token
    bool exchangeCodeForToken(const String &clientId,
                              const String &clientSecret,
                              const String &redirectUri,
                              const String &code,
                              OAuthToken &token);

    // Refresh access token using refresh token
    bool refreshAccessToken(const String &clientId,
                            const String &clientSecret,
                            OAuthToken &token);

    // Check if token is valid (not expired)
    bool isTokenValid(const OAuthToken &token);
};
