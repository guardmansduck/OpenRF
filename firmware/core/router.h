#pragma once
#include "packet.h"
#include <Arduino.h>
#include <vector>

class Transport {
public:
    virtual const char* name() = 0;
    virtual void begin() = 0;
    virtual bool send(const OpenRF_Packet &pkt) = 0;
    virtual void loop() = 0;
    virtual void setReceiveCallback(void (*cb)(const OpenRF_Packet&)) = 0;
};

class Router {
private:
    std::vector<Transport*> transports;

public:
    void addTransport(Transport* t) { transports.push_back(t); }
    void begin() { for (auto t : transports) t->begin(); }
    void loop() { for (auto t : transports) t->loop(); }
};
