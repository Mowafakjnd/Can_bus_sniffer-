# Can_bus_sniffer-
CAN Bus Sniffer - DiRT 4 Arduino Dashboard
A real-time automotive data logger and game dashboard system that captures CAN bus communication from racing simulators (DiRT 4) and displays live telemetry on an Arduino-powered LED dashboard.

Overview
This project bridges the gap between racing simulation software and physical hardware by intercepting OBD-II CAN bus messages and translating them into visual feedback through LEDs and serial output. It's designed for sim racing enthusiasts who want tangible, real-time displays of in-game vehicle telemetry (RPM, speed, etc.).

Key Features
CAN Bus Communication: Uses an MCP2515 CAN controller to read OBD-II standardized messages (ID: 0x7E8)
Real-Time Data Extraction: Parses RPM and speed from incoming CAN frames
Visual Feedback: Triggers shift lights and NeoPixel LED effects based on engine RPM thresholds
Serial Logging: Outputs telemetry data to serial monitor for debugging and real-time monitoring
Hardware Integration: Arduino-based solution compatible with standard 8MHz crystal oscillators

Technical Stack
Microcontroller: Arduino (with MCP2515 CAN transceiver) + ESP32
Protocol: CAN 2.0B at 500 kbps
Components: MCP2515 module, shift light LEDs, NeoPixels (WS2812B)
Simulation Software: DiRT 4 or other racing games with CAN output support

Use Cases
DIY sim racing cockpit enhancement
Game-to-hardware integration projects
Real-time vehicle telemetry visualization
Educational automotive electronics projects

<img width="1920" height="1080" alt="Capture d&#39;écran 2026-03-31 222924" src="https://github.com/user-attachments/assets/8e7327dd-b872-4cc2-af54-203a094ffd0b" /> 
<img width="960" height="826" alt="Capture d&#39;écran 2026-03-30 211627" src="https://github.com/user-attachments/assets/b08afabe-b01c-4a06-9b36-c2298660f344" />

<img width="1920" height="1080" alt="Capture d&#39;écran 2026-03-31 232616" src="https://github.com/user-attachments/assets/33b04389-6295-43f0-8523-e8ff80d0568c" />
<img width="541" height="329" alt="Capture d&#39;écran 2026-03-31 225606" src="https://github.com/user-attachments/assets/97a44346-c90f-489b-a6db-eacd7d1753fb" />



<img width="1920" height="1080" alt="Capture d&#39;écran 2026-04-19 014858" src="https://github.com/user-attachments/assets/44e80692-2ddc-47e3-914c-4bf521c7164d" />
<img width="1920" height="1080" alt="Capture d&#39;écran 2026-04-18 225134" src="https://github.com/user-attachments/assets/9f23bfac-abb0-4a8e-b71d-6845cbe76878" />

<img width="534" height="409" alt="Capture d&#39;écran 2026-04-18 172210" src="https://github.com/user-attachments/assets/14dabb63-1a30-4d8b-9342-eee29a43fcc2" />

