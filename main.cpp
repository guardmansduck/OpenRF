#include <Arduino.h>
#include "core/router.h"
#include "core/packet.h"

#include "modules/lora/lora.h"
#include "modules/discord/discord.h"
#include "modules/sms/sms.h"

#include "oauth/oauth.h"
#include "config/links.h"

Router router;

LoRaModule lora;
OAuthManager oauth;
OAuthToken discordToken;
OAuthToken smsToken;

DiscordModule discord(&oauth, &discordToken, "YOUR_DISCORD_CHANNEL_ID");
SMSModule sms(&oauth, &smsToken, "+15551234567");

void onPacketReceived(const OpenRF_Packet &pkt) {
    if (pkt.src != 0x00000001) lora.send(pkt);
    if (pkt.src != 0xDDDDDDDD) discord.send(pkt);
    if (pkt.src != 0xABCDEF01) sms.send(pkt);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    router.addTransport(&lora);
    router.addTransport(&discord);
    router.addTransport(&sms);

    lora.setReceiveCallback(onPacketReceived);
    discord.setReceiveCallback(onPacketReceived);
    sms.setReceiveCallback(onPacketReceived);

    router.begin();

    Serial.println("OpenRF router running!");
}

void loop() {
    router.loop();

    if (!oauth.isTokenValid(discordToken))
        oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", discordToken);
    if (!oauth.isTokenValid(smsToken))
        oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", smsToken);

    delay(10);
}
