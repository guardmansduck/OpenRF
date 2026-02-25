#include <Arduino.h>
#include "core/router.h"
#include "core/packet.h"

// Modules
#include "modules/lora/lora.h"
#include "modules/discord/discord.h"
#include "modules/sms/sms.h"

// OAuth
#include "oauth/oauth.h"
#include "config/links.h"

// ======== Global Objects ========
Router router;

LoRaModule lora;

// Central OAuth manager and tokens
OAuthManager oauth;
OAuthToken discordToken;
OAuthToken smsToken;

DiscordModule discord(&oauth, &discordToken, "YOUR_DISCORD_CHANNEL_ID");
SMSModule sms(&oauth, &smsToken, "+15551234567");

// ======== OpenRF Packet Routing ========
// This example forwards packets between all modules
void onPacketReceived(const OpenRF_Packet &pkt) {
    // Forward packet to all modules except the source
    if (pkt.src != 0x00000001) lora.send(pkt);
    if (pkt.src != 0xDDDDDDDD) discord.send(pkt);
    if (pkt.src != 0xFFFFFFFF) sms.send(pkt);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // WiFi required for Discord and SMS
    WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    // Setup router
    router.addTransport(&lora);
    router.addTransport(&discord);
    router.addTransport(&sms);

    // Set callback for all modules
    lora.setReceiveCallback(onPacketReceived);
    discord.setReceiveCallback(onPacketReceived);
    sms.setReceiveCallback(onPacketReceived);

    // Begin modules
    router.begin();

    Serial.println("OpenRF router is running!");
}

void loop() {
    router.loop();

    // Optional: refresh tokens periodically
    if (!oauth.isTokenValid(discordToken)) {
        oauth.refreshAccessToken("YOUR_CLIENT_ID", "YOUR_CLIENT_SECRET", discordToken);
    }
    if (!oauth.isTokenValid(smsToken)) {
        oauth.refreshAccessToken("YOUR_CLIENT_ID", "YOUR_CLIENT_SECRET", smsToken);
    }

    delay(10); // allow other tasks to run
}
