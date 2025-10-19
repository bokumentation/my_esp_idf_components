// Receiver Code
// ESP32-DevKitC

#include <Arduino.h>
#include <stdio.h>
#include "esp_log.h" 
#include "SPI.h"
#include "mcp2515.h"


// --- ESP32-DevKit-C (VSPI Bus) SPI PIN DEFINITIONS ---
// SCK  -> GPIO18
// MISO -> GPIO19
// MOSI -> GPIO23
// CS   -> GPIO5 (or another available GPIO)
const int SPI_CS_PIN = 5; 

// Define the structure for the received CAN frame
struct can_frame canMsg;

// Initialize MCP2515 object, passing the CS pin
MCP2515 mcp2515(SPI_CS_PIN);

void setup() {
  // Serial.begin() is still needed to initialize the UART for printf redirection.
  Serial.begin(115200); 
  
  // Set the Chip Select pin as an OUTPUT
  pinMode(SPI_CS_PIN, OUTPUT); 

  // Initialize the SPI bus for the ESP32-DevKit-C's VSPI pins: (SCK, MISO, MOSI, SS)
  SPI.begin(18, 19, 23, 5); 

  mcp2515.reset();

  // Initialize CAN at 500kbps (MUST match the Transmitter's bitrate)
  if (mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ) == MCP2515::ERROR_OK) {
    // Replaced Serial.println with printf
    printf("MCP2515 Initialized Successfully!\n"); 
  } else {
    // Replaced Serial.println with printf
    printf("Error Initializing MCP2515...\n"); 
    while (1); 
  }
  
  mcp2515.setNormalMode();
  printf("CAN Receiver Ready (ESP32-DevKit-C Polling Mode).\n");
}

void loop() {
  // Check if a message is available and read it directly
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    
    // Check if the received message ID is the one we expect (0x100)
    if (canMsg.can_id == 0x100) {
      
      // Fixed-size array for data (max 8 bytes + null terminator)
      char data_str[9] = {0};
      size_t copy_len = min((size_t)canMsg.can_dlc, sizeof(data_str) - 1);
      
      // Copy data and ensure null termination
      memcpy(data_str, canMsg.data, copy_len);
      data_str[copy_len] = '\0';
      
      // Replaced Serial.print/println with a single printf call
      // %lX is used for the unsigned long CAN ID in hexadecimal format
      // %s is used for the null-terminated string data
      printf("✅ Received Hello! - ID: 0x%lX, Data: %s\n", canMsg.can_id, data_str);
    }
  }

  // Polling delay
  delay(10);
}