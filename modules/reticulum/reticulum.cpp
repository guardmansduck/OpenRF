#include "reticulum.h"
#include <Reticulum.h>
#include "../../board_profiles.h"

void ReticulumModule::begin() {
    Serial.println("Initializing Reticulum...");
    BoardPins pins = detectBoardPins();
    Reticulum.begin(pins.ss, pins.reset, pins.dio0, 915E6);
    Serial.println("Reticulum mesh ready.");
}

bool ReticulumModule::send(const OpenRF_Packet &pkt) {
    ReticulumMessage msg(pkt.payload, pkt.len);
    return Reticulum.send(msg);
}

void ReticulumModule::loop() {
    while (Reticulum.available()) {
        ReticulumMessage msg = Reticulum.receive();
        OpenRF_Packet pkt;
        pkt.len = msg.length();
        memcpy(pkt.payload, msg.data(), pkt.len);
        pkt.src = 0xRETICULIM;
        if (rxCallback) rxCallback(pkt);
    }
}

void ReticulumModule::setReceiveCallback(void (*cb)(const OpenRF_Packet&)) {
    rxCallback = cb;
}
