#include "oauth.h"

String OAuthManager::getAuthUrl(const String &clientId,
                                const String &redirectUri,
                                const String &scope,
                                const String &state) {
    String url = "https://discord.com/api/oauth2/authorize";
    url += "?response_type=code";
    url += "&client_id=" + clientId;
    url += "&scope=" + scope;
    url += "&redirect_uri=" + redirectUri;
    url += "&state=" + state;
    return url;
}

bool OAuthManager::exchangeCodeForToken(const String &clientId,
                                        const String &clientSecret,
                                        const String &redirectUri,
                                        const String &code,
                                        OAuthToken &token) {
    HTTPClient http;
    http.begin("https://discord.com/api/oauth2/token");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String body = "client_id=" + clientId +
                  "&client_secret=" + clientSecret +
                  "&grant_type=authorization_code" +
                  "&code=" + code +
                  "&redirect_uri=" + redirectUri;

    int httpCode = http.POST(body);
    if (httpCode != 200) {
        http.end();
        return false;
    }

    String resp = http.getString();
    http.end();

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, resp);

    token.accessToken = doc["access_token"].as<String>();
    token.refreshToken = doc["refresh_token"].as<String>();
    token.expiresAt = millis() / 1000 + doc["expires_in"].as<uint32_t>();

    return true;
}

bool OAuthManager::refreshAccessToken(const String &clientId,
                                      const String &clientSecret,
                                      OAuthToken &token) {
    if (token.refreshToken == "") return false;

    HTTPClient http;
    http.begin("https://discord.com/api/oauth2/token");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String body = "client_id=" + clientId +
                  "&client_secret=" + clientSecret +
                  "&grant_type=refresh_token" +
                  "&refresh_token=" + token.refreshToken;

    int httpCode = http.POST(body);
    if (httpCode != 200) {
        http.end();
        return false;
    }

    String resp = http.getString();
    http.end();

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, resp);

    token.accessToken = doc["access_token"].as<String>();
    token.refreshToken = doc["refresh_token"].as<String>();
    token.expiresAt = millis() / 1000 + doc["expires_in"].as<uint32_t>();

    return true;
}

bool OAuthManager::isTokenValid(const OAuthToken &token) {
    return (millis() / 1000) < token.expiresAt;
}
