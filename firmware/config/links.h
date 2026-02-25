#pragma once
#include <Arduino.h>

// ===== Discord OAuth & API =====
namespace DiscordLinks {
    const String AUTH_URL_BASE      = "https://discord.com/api/oauth2/authorize";
    const String TOKEN_URL          = "https://discord.com/api/oauth2/token";
    const String GATEWAY_URL        = "wss://gateway.discord.gg/?v=10&encoding=json";
    const String API_BASE           = "https://discord.com/api/v10";
}

// ===== Example scopes =====
namespace DiscordScopes {
    const String MESSAGES_READ      = "messages.read";
    const String MESSAGES_WRITE     = "messages.write";
    const String IDENTIFY           = "identify";
}

// ===== Add other service links here =====
namespace SMSLinks {
    const String API_BASE           = "https://sms.example.com/api";
}
