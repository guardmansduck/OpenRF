#pragma once
#include "packet.h"

// Abstract Transport interface
class Transport {
public:
    virtual const char* name() = 0;          // Name of the transport
    virtual void begin() = 0;                // Init hardware
    virtual bool send(const OpenRF_Packet &pkt) = 0; // Send packet
    virtual void loop() = 0;                 // Called every cycle to process RX/TX
    virtual void setReceiveCallback(void (*cb)(const OpenRF_Packet&)) = 0; // Callback on packet received
};
