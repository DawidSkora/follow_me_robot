#include "queues.hpp"
#include "messages.hpp"


QueueHandle_t Queues::usonic_sensor_queue = nullptr;

void Queues::init() {
    usonic_sensor_queue = xQueueCreate(5, sizeof(SensorData));
}