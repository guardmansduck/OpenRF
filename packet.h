#pragma once
#include <Arduino.h>
#include <stdint.h>

// OpenRF Packet definition
struct OpenRF_Packet {
    uint32_t src;         // Node ID of sender
    uint32_t dst;         // Node ID of destination (0xFFFFFFFF = broadcast)
    uint8_t flags;        // Flags: mesh, p2p, encrypted, etc.
    uint8_t len;          // Length of payload
    uint8_t payload[220]; // Payload (UTF-8 text or binary)
    uint16_t crc;         // CRC16 for integrity
};

// Calculate CRC16 (basic CCITT)
inline uint16_t crc16(const uint8_t* data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

// Fill CRC for packet
inline void fill_crc(OpenRF_Packet &pkt) {
    pkt.crc = crc16(pkt.payload, pkt.len);
}

// Validate CRC
inline bool validate_crc(const OpenRF_Packet &pkt) {
    return pkt.crc == crc16(pkt.payload, pkt.len);
}
