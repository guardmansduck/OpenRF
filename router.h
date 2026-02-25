#pragma once
#include "packet.h"
#include "transport.h"
#include <vector>

class Router {
private:
    std::vector<Transport*> transports;

public:
    // Add a transport to the router
    void addTransport(Transport* t) {
        transports.push_back(t);

        // Set callback so router receives packets from this transport
        t->setReceiveCallback([this, t](const OpenRF_Packet &pkt){
            this->onReceive(pkt, t);
        });
    }

    // Called by transports on packet reception
    void onReceive(const OpenRF_Packet &pkt, Transport* from) {
        // Validate packet
        if (!validate_crc(pkt)) return;

        // Deliver locally
        deliverLocal(pkt);

        // Forward to other transports
        for (auto &t : transports) {
            if (t != from) {
                t->send(pkt);
            }
        }
    }

    // Override in main firmware for local delivery (e.g., display, BLE, phone)
    virtual void deliverLocal(const OpenRF_Packet &pkt) = 0;

    // Call in main loop
    void loop() {
        for (auto &t : transports) {
            t->loop();
        }
    }

    // Initialize all transports
    void begin() {
        for (auto &t : transports) {
            t->begin();
        }
    }
};
