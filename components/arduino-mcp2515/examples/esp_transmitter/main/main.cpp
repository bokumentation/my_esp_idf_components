// Transmitter Code
// ESP32-C3-Supermini

#include <Arduino.h>
#include <stdio.h>
#include "esp_log.h" 
#include "SPI.h"
#include "mcp2515.h"

// Define a Tag for the logger output
static const char *TAG = "CAN_TX";

// --- ESP32-C3 SPI PIN DEFINITIONS ---
// SCK  -> GPIO4
// MISO -> GPIO5
// MOSI -> GPIO6
// CS   -> GPIO7
const int SPI_CS_PIN = 7;

// Define the structure for the CAN frame
struct can_frame canMsg;

// Initialize MCP2515 object, passing the CS pin
MCP2515 mcp2515(SPI_CS_PIN);

void setup()
{
    // Serial.begin() is NOT needed when using the esp_log API.
    // The ESP-IDF environment handles logging initialization.

    // Set the Chip Select pin as an OUTPUT
    pinMode(SPI_CS_PIN, OUTPUT);

    // Initialize the SPI bus for the ESP32-C3's custom pins: (SCK, MISO, MOSI, SS)
    SPI.begin(4, 5, 6, 7);

    mcp2515.reset();

    // Initialize CAN at 500kbps (adjust bitrate and crystal frequency as needed)
    if (mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ) == MCP2515::ERROR_OK)
    {
        // Replaced Serial.println with ESP_LOGI
        ESP_LOGI(TAG, "MCP2515 Initialized Successfully!");
    }
    else
    {
        // Replaced Serial.println with ESP_LOGE
        ESP_LOGE(TAG, "Error Initializing MCP2515...");
        while (1)
            ;
    }

    mcp2515.setNormalMode();

    // Prepare the message data for "Hello!"
    canMsg.can_id = 0x100;
    canMsg.can_dlc = 6;
    char dataString[] = "Hello!";
    for (int i = 0; i < canMsg.can_dlc; i++)
    {
        canMsg.data[i] = dataString[i];
    }
}

void loop()
{
    // Send the message
    if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK)
    {

        // Create a temporary string to format the data (optional, but cleaner)
        // char data_str[canMsg.can_dlc + 1] = {0};
        // memcpy(data_str, canMsg.data, canMsg.can_dlc);
        char data_str[9] = {0};
        // -----------------------------------------------------------------

        // We can only safely copy up to 8 bytes, so use the smaller of the two
        size_t copy_len = min((size_t)canMsg.can_dlc, sizeof(data_str) - 1);

        // Copy only the actual message length
        memcpy(data_str, canMsg.data, copy_len);
        data_str[copy_len] = '\0'; // Ensure the string is null-terminated

        // Replaced Serial.print/println with a single ESP_LOGI
        // ESP_LOGI(TAG, "Message Sent! ID: 0x%lX, Data: %s", canMsg.can_id, data_str);
        printf("Message Sent! ID: 0x%lX, Data: %s \n", canMsg.can_id, data_str);
    }
    else
    {
        // Replaced Serial.println with ESP_LOGE
        ESP_LOGE(TAG, "Error Sending Message...");
    }

    delay(1000);
}