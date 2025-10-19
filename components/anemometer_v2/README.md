# How to use this library
### 1. Include this library
Edit your `main/cmakelists.txt`.

```cmake
idf_component_register(
    SRCS "main.cpp"
    PRIV_REQUIRES anemometer_v2
    INCLUDE_DIRS "." "include"
)
```

### 2. Code Example

```cpp
// System Include
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// User Include
#include "anemometer.h"
#include "board_pins.h"

extern "C" void app_main(void)
{
    anemometer_handle_t *wind_sensor = anemometer_init(ANEMOMETER_ADC_PIN);
    if (wind_sensor == nullptr)
    {
        ESP_LOGE("MAIN", "Anemometer Failed");
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        // 1. EXECUTE ALL MEASUREMENTS FIRST
        anemometer_measure_and_update(wind_sensor); // Updates every 10s non-blocking

        // Total delay is now 500ms + 4500ms = 5 seconds between SO2 read and H2S read
        vTaskDelay(pdMS_TO_TICKS(4500));
    }
}
```
