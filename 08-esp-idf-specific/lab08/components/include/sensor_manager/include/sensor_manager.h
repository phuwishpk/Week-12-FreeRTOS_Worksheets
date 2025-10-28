#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef struct {
    float temperature;
    float humidity;
    uint32_t timestamp;
} sensor_data_t;

esp_err_t sensor_manager_init(void);
esp_err_t sensor_manager_start(void);
QueueHandle_t sensor_manager_get_data_queue(void);
void sensor_manager_stop(void);
```

```c
// components/sensor_manager/src/sensor_manager.c
#include "sensor_manager.h"
#include "esp_log.h"

static const char *TAG = "SENSOR_MGR";
static QueueHandle_t sensor_data_queue;
static TaskHandle_t sensor_task_handle;

void sensor_task(void *parameter)
{
    sensor_data_t data;
    
    while (1) {
        // Read sensors
        data.temperature = 25.0 + (rand() % 200) / 10.0;
        data.humidity = 40.0 + (rand() % 500) / 10.0;
        data.timestamp = xTaskGetTickCount();
        
        // Send to queue
        if (xQueueSend(sensor_data_queue, &data, pdMS_TO_TICKS(100)) != pdTRUE) {
            ESP_LOGW(TAG, "Sensor data queue full");
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t sensor_manager_init(void)
{
    sensor_data_queue = xQueueCreate(10, sizeof(sensor_data_t));
    if (sensor_data_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create sensor data queue");
        return ESP_ERR_NO_MEM;
    }
    
    ESP_LOGI(TAG, "Sensor manager initialized");
    return ESP_OK;
}

esp_err_t sensor_manager_start(void)
{
    BaseType_t result = xTaskCreatePinnedToCore(
        sensor_task,
        "SensorTask",
        2048,
        NULL,
        5,
        &sensor_task_handle,
        APP_CPU_NUM  // Run on Core 1
    );
    
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create sensor task");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Sensor manager started");
    return ESP_OK;
}

QueueHandle_t sensor_manager_get_data_queue(void)
{
    return sensor_data_queue;
}