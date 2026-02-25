#pragma once
#include <Arduino.h>

namespace DiscordLinks {
    const String AUTH_URL_BASE      = "https://discord.com/api/oauth2/authorize";
    const String TOKEN_URL          = "https://discord.com/api/oauth2/token";
    const String GATEWAY_URL        = "wss://gateway.discord.gg/?v=10&encoding=json";
    const String API_BASE           = "https://discord.com/api/v10";
}

namespace DiscordScopes {
    const String MESSAGES_READ      = "messages.read";
    const String MESSAGES_WRITE     = "messages.write";
    const String IDENTIFY           = "identify";
}

namespace SMSLinks {
    const String API_BASE           = "https://sms.example.com/api";
}
