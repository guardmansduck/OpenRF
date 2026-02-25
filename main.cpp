#include <Arduino.h>
#include "core/router.h"
#include "core/packet.h"

#include "modules/lora/lora.h"
#include "modules/meshtastic/meshtastic.h"
#include "modules/reticulum/reticulum.h"
#include "modules/discord/discord.h"
#include "modules/sms/sms.h"
#include "modules/slack/slack.h"
#include "modules/gmail/gmail.h"
#include "modules/steam/steam.h"

#include "oauth/oauth.h"
#include "config/links.h"

Router router;

LoRaModule lora;
MeshtasticModule meshtastic;
ReticulumModule reticulum;

OAuthManager oauth;
OAuthToken discordToken, smsToken, slackToken, gmailToken, steamToken;

DiscordModule discord(&oauth, &discordToken, "DISCORD_CHANNEL_ID");
SMSModule sms(&oauth, &smsToken, "+15551234567");
SlackModule slack(&oauth, &slackToken, "SLACK_CHANNEL_ID");
GmailModule gmail(&oauth, &gmailToken, "YOUR_EMAIL@gmail.com");
SteamModule steam(&oauth, &steamToken, "STEAM_ID");

void onPacketReceived(const OpenRF_Packet &pkt) {
    if (pkt.src != 0x00000001) lora.send(pkt);
    if (pkt.src != 0x11111111) meshtastic.send(pkt);
    if (pkt.src != 0x22222222) reticulum.send(pkt);
    if (pkt.src != 0xDDDDDDDD) discord.send(pkt);
    if (pkt.src != 0xABCDEF01) sms.send(pkt);
    if (pkt.src != 0xBBBBBBBB) slack.send(pkt);
    if (pkt.src != 0xCCCCCCCC) gmail.send(pkt);
    if (pkt.src != 0xEEEEEEEE) steam.send(pkt);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\nWiFi connected!");

    router.addTransport(&lora);
    router.addTransport(&meshtastic);
    router.addTransport(&reticulum);
    router.addTransport(&discord);
    router.addTransport(&sms);
    router.addTransport(&slack);
    router.addTransport(&gmail);
    router.addTransport(&steam);

    lora.setReceiveCallback(onPacketReceived);
    meshtastic.setReceiveCallback(onPacketReceived);
    reticulum.setReceiveCallback(onPacketReceived);
    discord.setReceiveCallback(onPacketReceived);
    sms.setReceiveCallback(onPacketReceived);
    slack.setReceiveCallback(onPacketReceived);
    gmail.setReceiveCallback(onPacketReceived);
    steam.setReceiveCallback(onPacketReceived);

    router.begin();
    Serial.println("OpenRF universal router running!");
}

void loop() {
    router.loop();

    if (!oauth.isTokenValid(discordToken)) oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", discordToken);
    if (!oauth.isTokenValid(smsToken)) oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", smsToken);
    if (!oauth.isTokenValid(slackToken)) oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", slackToken);
    if (!oauth.isTokenValid(gmailToken)) oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", gmailToken);
    if (!oauth.isTokenValid(steamToken)) oauth.refreshAccessToken("CLIENT_ID", "CLIENT_SECRET", steamToken);

    delay(10);
}
