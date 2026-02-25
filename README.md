# OpenRF

**Universal cross-protocol radio messaging firmware**

OpenRF is open-source firmware that turns radio hardware into universal cross-protocol messaging nodes.  
A single device can simultaneously bridge:

- LoRa radios  
- Mesh radios (Meshtastic, Reticulum/RNode)  
- Packet radio (AX.25, KISS TNC)  
- BLE / USB phone links  
- Internet messaging via a mobile companion app (Discord, SMS, Matrix, Telegram)

**No gateway computer or external hardware required.**  
Each OpenRF node routes messages locally between all connected transports.

---

## Features

- Multi-radio simultaneous bridging  
- Universal RF packet layer  
- Embedded mesh relay  
- Cross-protocol translation  
- Phone ↔ RF bridge  
- MCU-friendly firmware (ESP32, STM32, etc.)  
- No central server or gateway hardware  
- Transport plugin system for easy protocol additions  

---

## Example Use Case

A handheld OpenRF node can relay messages like this:

Meshtastic ↔ LoRa ↔ RNode ↔ Discord ↔ SMS


All without any extra gateway hardware — the radio itself is the bridge.

---

## Architecture

OpenRF firmware is organized in layers:

- **Core router** – transport-agnostic message forwarding  
- **Universal packet layer** – compact, cross-transport packet format  
- **Transport adapters** – implement different radios (LoRa, Meshtastic, RNode, AX.25, BLE, Wi-Fi)  
- **Board support** – ESP32, T-Beam, Heltec, RNode, T-Deck  

All transports feed into the router, allowing multi-radio forwarding and bridging.

---

## Internet Bridge

A mobile companion app connects via BLE or USB and enables:

- Discord bridge  
- SMS bridge  
- Matrix bridge  
- Telegram bridge  

The phone provides internet connectivity while OpenRF handles RF routing.

---

## Supported Hardware (Planned)

- ESP32 LoRa boards  
- Meshtastic devices  
- RNode devices  
- Heltec LoRa  
- LilyGo T-Beam  
- T-Deck  

---

## Related Projects

OpenRF interoperates and bridges across existing radio stacks:

- :contentReference[oaicite:0]{index=0}  
- :contentReference[oaicite:1]{index=1}  
- :contentReference[oaicite:2]{index=2}  

---

## Roadmap

**Phase 1:**  
- Universal packet layer  
- Raw LoRa transport  
- BLE mobile bridge  

**Phase 2:**  
- Meshtastic transport  
- Multi-transport routing  

**Phase 3:**  
- RNode transport  
- Internet app MVP  

**Phase 4:**  
- Encryption and identity  
- Mesh optimization  

---

## License

OpenRF is licensed under **GPL-3.0** to ensure openness and collaboration in the RF firmware ecosystem.

---

## Getting Started

1. Clone the repository:

```bash
git clone https://github.com/<yourusername>/openrf.git
