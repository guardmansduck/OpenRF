#include <Arduino.h>
#include "core/router.h"
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
    lora.send(pkt);
    meshtastic.send(pkt);
    reticulum.send(pkt);
    discord.send(pkt);
    sms.send(pkt);
    slack.send(pkt);
    gmail.send(pkt);
    steam.send(pkt);
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
    delay(10);
}
