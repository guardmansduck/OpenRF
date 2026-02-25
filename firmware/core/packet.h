#pragma once
#include <Arduino.h>

struct OpenRF_Packet {
    uint32_t src;
    uint32_t dst;
    uint16_t len;
    uint8_t payload[220];
    uint16_t crc;
};

inline uint16_t crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
            crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
    }
    return crc;
}
