#include "queues.hpp"
#include "messages.hpp"


QueueHandle_t Queues::usonic_sensor_queue = nullptr;

void Queues::init() {
    Queues::usonic_sensor_queue = xQueueCreate(1, sizeof(SensorData));
    configASSERT(Queues::usonic_sensor_queue != nullptr);
}
