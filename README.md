# Sim2CAN — Sim Racing to CAN Bus Telemetry Bridge

Turn a racing sim's live telemetry into real CAN bus frames — no real car, no real ECU needed.

## Why

Real CAN bus hardware is expensive and a real car isn't exactly lab equipment. Instead of a real vehicle, this project taps into **Live For Speed (LFS)**'s built-in `OutGauge` UDP telemetry, ships it over WiFi, and re-encodes it as real CAN traffic on physical transceivers — so the rest of the pipeline (CAN parsing, dashboards, decoders) can be built and tested exactly like it would be on a real car.

## Pipeline

```
LFS (OutGauge UDP)  --WiFi-->  ESP32  --CAN-->  MCP2515 #1  ---CAN bus wires---> MCP2515 #2  --SPI-->  Arduino UNO --Serial/LED--> Dashboard output
```

1. **Live For Speed** is configured (via `cfg.txt`) to broadcast `OutGauge` telemetry — speed, RPM, gear, turbo, engine temp, fuel — as UDP packets.
2. An **ESP32** joins the network (or hosts its own AP), listens on the UDP port, parses the OutGauge packet, and repacks the relevant fields into standard CAN frames.
3. The ESP32 pushes those frames out through an **MCP2515 CAN transceiver module**.
4. A second MCP2515, wired to an **Arduino UNO**, receives the frames over the physical CAN bus.
5. The Arduino decodes the CAN payload (RPM, speed, etc.) and outputs it — printed to serial for now, with shift-light LED logic based on RPM threshold.

## Tools used

- **Oracle VirtualBox** (Linux guest) — isolated environment for capture/testing
- **Wireshark** — captured and dissected the raw OutGauge UDP packets to reverse-engineer the field layout before writing the parser
- **SocketCAN + ICSim** (`vcan0`) — virtual CAN bus used to test frame structure and candump-style traffic before touching real hardware
- **Arduino IDE 2.3.8** — firmware for both the ESP32 (encoder) and Arduino UNO (decoder)
- **ESP32 Dev Module** — WiFi ↔ CAN bridge
- **Arduino UNO** — CAN receiver / decoder
- **2x MCP2515 CAN Bus Transceiver Modules** — physical CAN interface on each end

## Screenshots

### 1. MCP2515 CAN transceiver module
![MCP2515 module](images/07_mcp2515_can_module.png)
The MCP2515 CAN bus transceiver module used on both ends of the physical CAN link (ESP32 side and Arduino side) — the core piece of hardware that makes this a real CAN bus project instead of just a UDP relay.

### 2. Capturing raw OutGauge UDP traffic (Linux VM + LFS)
![LFS UDP hex dump](images/01_lfs_linux_udp_hexdump.png)
First pass at understanding the protocol: Live For Speed running alongside a Linux VM, dumping the raw OutGauge UDP payload in hex to identify field boundaries (fuel, brake balance, car name, etc.) by eye before reaching for a proper dissector.

### 3. Wireshark dissection of the OutGauge packet
![Wireshark OutGauge capture](images/03_wireshark_outgauge_capture.png)
Wireshark filtering on `udp.port == 2992`, showing the custom dissector output for `LFS OutGauge Telemetry Data` — Speed, RPM, Turbo, Engine Temp, and Fuel Capacity extracted live while driving.

### 4. Field-level packet breakdown
![Wireshark OutGauge fields](images/04_wireshark_outgauge_dissected.png)
Close-up of the dissected UDP payload, confirming the exact byte offsets used later in the ESP32 parsing code (Time, Car Name, Flags, Gear, Speed, RPM, Turbo, Engine Temp, Fuel Capacity).

### 5. Virtual CAN bus testing with ICSim
![ICSim vcan0 candump](images/02_icsim_vcan0_candump.png)
With the protocol understood, CAN frame structure gets validated on a virtual `vcan0` interface using ICSim before wiring up real MCP2515 hardware — a cheap way to check frame IDs and payload sizes first.

### 6. ESP32 encoder / Arduino decoder firmware
![ESP32 encode, Arduino decode](images/05_esp32_arduino_can_encode_decode.png)
Left: ESP32 sketch packing RPM (OBD-II style, RPM×4 split across 2 bytes) and speed into a `canMsg` struct before sending via MCP2515. Right: Arduino UNO sketch reversing the math to recover RPM and speed from the CAN frame. The serial output ("CAN Send Error – Check J1 Jumpers and Wiring!") here was mid-debug of the SPI/CAN wiring between the two MCP2515 boards.

### 7. ESP32 WiFi AP + UDP listener, MCP2515 init success
![ESP32 AP + MCP2515 init](images/06_esp32_ap_mcp2515_init_success.png)
The payoff: ESP32 hosting its own WiFi access point and listening on a UDP port for incoming telemetry, alongside a successful `MCP2515 Initialized Successfully!` log confirming the CAN controller came up correctly over SPI — full pipeline working end to end.

## Status / known issues

- CAN send errors traced back to J1 jumper / wiring misconfiguration on the MCP2515 boards — resolved by re-checking jumper position and SPI wiring.
- Currently outputs decoded values over Serial; a physical gauge/LED dashboard is the next step.
- Shift-light logic implemented (LED on Pin 9 when RPM > 6500) as a first real-world "actuation" from CAN data.

## Roadmap

- [ ] Move from Serial print to a physical instrument cluster / LED bar for RPM and speed
- [ ] Support more OutGauge fields (fuel, gear, turbo) as separate CAN IDs
- [ ] Clean up wiring and document a proper schematic
- [ ] Package as a reusable "sim-to-CAN" bridge for other telemetry-capable games/sims

## Setup

1. Enable OutGauge in LFS `cfg.txt` and point it at the ESP32's IP/port.
2. Flash `esptelemetry.ino` to the ESP32 (WiFi + UDP listener + CAN encoder).
3. Wire ESP32 → MCP2515 #1 → CAN bus wires (CANH/CANL) → MCP2515 #2 → Arduino UNO.
4. Flash the Arduino decoder sketch (`testarduino.ino`) to the UNO.
5. Open both serial monitors to watch encode/decode in real time.

## Disclaimer

This is a learning/portfolio project built to practice CAN bus workflows without needing a real vehicle. Not intended for use on an actual car's CAN network.
