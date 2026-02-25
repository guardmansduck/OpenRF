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

namespace GmailLinks {
    const String API_BASE           = "https://gmail.googleapis.com/gmail/v1/users/me";
    const String SEND_MESSAGE       = API_BASE + "/messages/send";
    const String LIST_MESSAGES      = API_BASE + "/messages";
    const String GET_MESSAGE        = API_BASE + "/messages/"; 
}

namespace SlackLinks {
    const String API_BASE           = "https://slack.com/api";
    const String CHAT_POST_MESSAGE  = API_BASE + "/chat.postMessage";
}

namespace SteamLinks {
    const String API_BASE        = "https://api.steampowered.com";
    const String CHAT_SEND       = API_BASE + "/ISteamWebUserPresenceOAuth/Message/v1";
    const String CHAT_RECEIVE    = API_BASE + "/ISteamWebUserPresenceOAuth/RecentMessages/v1";
    const String OAUTH_AUTHORIZE = "https://steamcommunity.com/oauth/login";
    const String OAUTH_TOKEN     = "https://api.steampowered.com/ISteamOAuth/GetToken/v1";
}
