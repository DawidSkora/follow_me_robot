#include "robot.hpp"
#include "messages.hpp"
#include "queues.hpp"

#include "freertos/FreeRTOS.h"

void Robot::start_task() {
    xTaskCreatePinnedToCore(_task, "motor", 4096, nullptr, 2, nullptr, 1);
}

void Robot::_search_for_the_way() {
    SensorData sensor;
    xQueueReceive(Queues.usonic_sensor_queue, &sensor, portMAX_DELAY);
    while(sensor.distance_cm < MIN_FRONT_DISTANCE_CM) {
        motor_driver->move_backward();
        vTaskDelay(pdMS_TO_TICKS(300));
        motor_driver->turn_right();
        vTaskDelay(pdMS_TO_TICKS(500));
        motor_driver->stop();
        vTaskDelay(pdMS_TO_TICKS(300));
        xQueueReceive(Queues.usonic_sensor_queue, &sensor, portMAX_DELAY);
    }
}

void Robot::_task(void* args) {
    motor_driver->stop();
    vTaskDelay(pdMS_TO_TICKS(1000));  
    while(true) {
        SensorData sensor;
        if(xQueueReceive(Queues.usonic_sensor_queue, &sensor, portMAX_DELAY)) {
            if(sensor.distance_cm > MIN_FRONT_DISTANCE_CM) {
                motor_driver->move_forward();
            } else {
                motor_driver->stop();
                vTaskDelay(pdMS_TO_TICKS(300));
                search_for_the_way();
            }
        }
    }
}