// System Include
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// User Include
#include "board_pins.h"
#include "tb600b_v2.h"

extern "C" void app_main(void)
{
    tb600b_handle_t *h2s_sensor =
        tb600b_init(H2S_UART_PORT, H2S_TX_PIN, H2S_RX_PIN, BAUD_RATE, H2S_LOG_TAG, TB600B_SENSOR_TYPE_H2S);

    tb600b_handle_t *so2_sensor =
        tb600b_init(SO2_UART_PORT, SO2_TX_PIN, SO2_RX_PIN, BAUD_RATE, SO2_LOG_TAG, TB600B_SENSOR_TYPE_SO2);

    if (h2s_sensor == nullptr || so2_sensor == nullptr)
    {
        ESP_LOGE("MAIN", "One or both sensors failed to initialize.");
        return;
    }

    tb600b_set_passive_mode(h2s_sensor);
    tb600b_set_passive_mode(so2_sensor);

    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        // 1. EXECUTE ALL MEASUREMENTS FIRST
        tb600b_measure_and_update(h2s_sensor);
        vTaskDelay(pdMS_TO_TICKS(100)); // Small delay between UART reads
        tb600b_measure_and_update(so2_sensor);

        // 2. AGGREGATE AND PRINT ALL DATA ONCE
        ESP_LOGI("MAIN", "--- Aggregating Cycle Data ---");

        // 3. WAIT FOR THE NEXT MEASUREMENT CYCLE
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
