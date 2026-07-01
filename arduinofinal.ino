#include <SPI.h>
#include <mcp2515.h>

// Structure to hold the incoming CAN data
struct can_frame canMsg;

// MCP2515 Object (Using Pin 10 for CS as we verified earlier)
MCP2515 mcp2515(10);

void setup() {
  Serial.begin(115200);
  
  // 1. Initialize the MCP2515
  mcp2515.reset();
  
  // CRITICAL: Must match the 8MHz crystal on your board and the ESP32 speed
  if (mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ) == MCP2515::ERROR_OK) {
    Serial.println("CAN Initialized Successfully!");
  } else {
    Serial.println("Error Initializing CAN - Check Wiring/Crystal!");
  }
  
  mcp2515.setNormalMode();
  
  Serial.println("------- DiRT 4 Arduino Dashboard Ready -------");
}

void loop() {
  // Check if a new CAN message has arrived
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    
    // Check for the specific ID we set in the ESP32 (0x7E8)
    if (canMsg.can_id == 0x7E8) {
      
      // --- 1. EXTRACT DATA ---
      
      // Extract RPM from bytes 3 and 4 (OBD-II standard)
      // We reverse the (RPM * 4) math done on the ESP32
      unsigned int rawRPM = (canMsg.can_id == 0x7E8) ? (canMsg.data[3] << 8 | canMsg.data[4]) : 0;
      int finalRPM = rawRPM / 4;

      // Extract Speed from byte 5 (Custom position)
      int finalSpeed = canMsg.data[5];

      // --- 2. DISPLAY DATA ---
      
      Serial.print("RPM: ");
      Serial.print(finalRPM);
      Serial.print(" | Speed: ");
      Serial.print(finalSpeed);
      Serial.println(" km/h");

      // --- 3. LED CONTROL LOGIC ---
      
      // Example: If RPM is over 6500, turn on a shift light on Pin 9
      if (finalRPM > 6500) {
        digitalWrite(9, HIGH); 
      } else {
        digitalWrite(9, LOW);
      }
      
      // If you are using NeoPixels (WS2812B), you would use the 
      // 'finalRPM' variable here to map how many LEDs light up.
    }
  }
}