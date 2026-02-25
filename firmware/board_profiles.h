#pragma once
#include <Arduino.h>

// Struct to hold generic pin configurations
struct BoardPins {
    uint8_t ss;
    uint8_t reset;
    uint8_t dio0;
    // Add more fields here if other modules need pins (e.g., I2C, UART)
};

// ===== Board-specific functions =====
inline BoardPins board_TBeam() {
    return {5, 14, 2};
}

inline BoardPins board_Heltec() {
    return {18, 14, 26};
}

inline BoardPins board_TDeck() {
    return {15, 27, 33};
}

inline BoardPins board_Default() {
    return {5, 14, 2};
}

// ===== Central auto-detect function =====
inline BoardPins detectBoardPins() {
    String chip = ESP.getChipModel();

    if (chip == "ESP32D0WDQ6") {          // T-Beam / LilyGo
        return board_TBeam();
    } else if (chip == "ESP32-WROOM-32") { // Heltec LoRa
        return board_Heltec();
    } else if (chip == "ESP32-PICO-D4") {  // Example T-Deck
        return board_TDeck();
    } else {                               // Default fallback
        return board_Default();
    }
}
