#pragma once
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

class UltrasonicSensor {
    public:
        UltrasonicSensor(gpio_num_t trig, gpio_num_t echo);
        ~UltrasonicSensor() = default;
        float measure_distance_cm();
        void start_task();
    private:
        gpio_num_t trig;
        gpio_num_t echo;

        static void task(void* arg);
};