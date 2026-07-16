#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class Queues {
public:
    static void init();
    static QueueHandle_t usonic_sensor_queue;
    static QueueHandle_t uart_command_queue;
};