Sim2CAN — Sim Racing to CAN Bus Telemetry Bridge

Turn a racing sim's live telemetry into real CAN bus frames — no real car, no real ECU needed.

Why

Real CAN bus hardware is expensive and a real car isn't exactly lab equipment. Instead of a real vehicle, this project taps into Live For Speed (LFS)'s built-in OutGauge UDP telemetry, ships it over WiFi, and re-encodes it as real CAN traffic on physical transceivers — so the rest of the pipeline (CAN parsing, dashboards, decoders) can be built and tested exactly like it would be on a real car.

Pipeline

LFS (OutGauge UDP)  --WiFi-->  ESP32  --CAN-->  MCP2515 #1  ---CAN bus wires---> MCP2515 #2  --SPI-->  Arduino UNO --Serial/LED--> Dashboard output
<img width="1447" height="258" alt="Capture d&#39;écran 2026-04-18 223726" src="https://github.com/user-attachments/assets/32c2f9c7-f941-4772-b5e1-952f1c1902cc" />


Live For Speed is configured (via cfg.txt) to broadcast OutGauge telemetry — speed, RPM, gear, turbo, engine temp, fuel — as UDP packets.
An ESP32 joins the network (or hosts its own AP), listens on the UDP port, parses the OutGauge packet, and repacks the relevant fields into standard CAN frames.
The ESP32 pushes those frames out through an MCP2515 CAN transceiver module.
A second MCP2515, wired to an Arduino UNO, receives the frames over the physical CAN bus.
The Arduino decodes the CAN payload (RPM, speed, etc.) and outputs it — printed to serial for now, with shift-light LED logic based on RPM threshold.


Tools used


Oracle VirtualBox (Linux guest) — isolated environment for capture/testing
Wireshark — captured and dissected the raw OutGauge UDP packets to reverse-engineer the field layout before writing the parser
SocketCAN + ICSim (vcan0) — virtual CAN bus used to test frame structure and candump-style traffic before touching real hardware
Arduino IDE 2.3.8 — firmware for both the ESP32 (encoder) and Arduino UNO (decoder)
ESP32 Dev Module — WiFi ↔ CAN bridge
Arduino UNO — CAN receiver / decoder
2x MCP2515 CAN Bus Transceiver Modules — physical CAN interface on each end


Screenshots

1. Capturing raw OutGauge UDP traffic (Linux VM + LFS)

<img width="1920" height="1080" alt="Capture d&#39;écran 2026-03-31 222924" src="https://github.com/user-attachments/assets/8e7327dd-b872-4cc2-af54-203a094ffd0b" /> 
Live For Speed running alongside a Linux VM, dumping the raw OutGauge UDP payload in hex to identify field boundaries (fuel, brake balance, car name, etc.) before writing a parser.

2. Virtual CAN bus testing with ICSim

<img width="960" height="826" alt="Capture d&#39;écran 2026-03-30 211627" src="https://github.com/user-attachments/assets/b08afabe-b01c-4a06-9b36-c2298660f344" />
Testing CAN frame structure on a virtual vcan0 interface using ICSim before wiring up real MCP2515 hardware — a cheap way to validate frame IDs and payload sizes first.

3. Wireshark dissection of the OutGauge packet

<img width="1920" height="1080" alt="Capture d&#39;écran 2026-03-31 232616" src="https://github.com/user-attachments/assets/33b04389-6295-43f0-8523-e8ff80d0568c" />
Wireshark filtering on udp.port == 2992, showing the custom dissector output for LFS OutGauge Telemetry Data — Speed, RPM, Turbo, Engine Temp, and Fuel Capacity extracted live while driving.

4. Field-level packet breakdown

<img width="541" height="329" alt="Capture d&#39;écran 2026-03-31 225606" src="https://github.com/user-attachments/assets/97a44346-c90f-489b-a6db-eacd7d1753fb" />
Close-up of the dissected UDP payload, confirming the byte offsets used later in the ESP32 parsing code (Time, Car Name, Flags, Gear, Speed, RPM, Turbo, Engine Temp, Fuel Capacity).

5. ESP32 encoder / Arduino decoder firmware

<img width="1920" height="1080" alt="Capture d&#39;écran 2026-04-19 014858" src="https://github.com/user-attachments/assets/44e80692-2ddc-47e3-914c-4bf521c7164d" />
Left: ESP32 sketch packing RPM (OBD-II style, RPM×4 split across 2 bytes) and speed into a canMsg struct before sending via MCP2515. Right: Arduino UNO sketch reversing the math to recover RPM and speed from the CAN frame. The serial output ("CAN Send Error – Check J1 Jumpers and Wiring!") here was mid-debug of the SPI/CAN wiring between the two MCP2515 boards.

6. ESP32 WiFi AP + UDP listener, MCP2515 init success

<img width="1920" height="1080" alt="Capture d&#39;écran 2026-04-18 225134" src="https://github.com/user-attachments/assets/9f23bfac-abb0-4a8e-b71d-6845cbe76878" />
ESP32 hosting its own WiFi access point and listening on a UDP port for incoming telemetry, alongside a successful MCP2515 Initialized Successfully! log confirming the CAN controller came up correctly over SPI.

7. MCP2515 CAN transceiver module

Show Image
<img width="534" height="409" alt="Capture d&#39;écran 2026-04-18 172210" src="https://github.com/user-attachments/assets/14dabb63-1a30-4d8b-9342-eee29a43fcc2" />
