#include "ultrasonic_sensor.hpp"
#include "messages.hpp"
#include "queues.hpp"

#include "freertos/FreeRTOS.h"
#include "esp_timer.h"

UltrasonicSensor::UltrasonicSensor(gpio_num_t trig, gpio_num_t echo) {
    this->trig = trig;
    this->echo = echo;
}

float UltrasonicSensor::measure_distance_cm() {
    int64_t start_time = 0;
    int64_t end_time = 0;
    int timeout = 1000000; // 1 second timeout (safety net)

    // Send 10µs HIGH pulse on TRIG pin to start measurement
    gpio_set_level(trig, 0);
    esp_rom_delay_us(2);
    gpio_set_level(trig, 1);
    esp_rom_delay_us(10);
    gpio_set_level(trig, 0);

    // Wait for ECHO pin to go HIGH (start of the echo pulse)
    while (gpio_get_level(echo) == 0 && timeout > 0) {
        timeout--;
    }
    if (timeout == 0) return -1; // Timeout error

    start_time = esp_timer_get_time(); // Time when echo pulse starts

    // Wait for ECHO pin to go LOW (end of echo pulse)
    timeout = 1000000;
    while (gpio_get_level(echo) == 1 && timeout > 0) {
        timeout--;
    }
    if (timeout == 0) return -1; // Timeout error

    end_time = esp_timer_get_time(); // Time when echo pulse ends

    // Pulse duration in microseconds
    int64_t pulse_duration = end_time - start_time;

    // HC-SR04 formula: distance (cm) = duration (us) / 58.0
    float distance_cm = (float)pulse_duration / 58.0;

    return distance_cm;
}

void UltrasonicSensor::start_task() {
    xTaskCreatePinnedToCore(task, "sensors", 4096, nullptr, 3, nullptr, 0);
}


void UltrasonicSensor::task(void* arg) {
    while(true) {
        SensorData sensor_data;
        sensor_data.distance_cm = measure_distance_cm();

        xQueueSend(Queues::usonic_sensor_queue, &sensor_data, 0);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}