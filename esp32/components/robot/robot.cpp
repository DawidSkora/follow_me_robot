#include "robot.hpp"
#include "messages.hpp"
#include "queues.hpp"

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

void Robot::start_task() {
    xTaskCreatePinnedToCore(task, "motor", 4096, this, 2, nullptr, 1);
}

void Robot::task(void* args) {
    auto* self = static_cast<Robot*>(args);
    self->task_loop();
}

// Motors are driven only by fresh commands from the RPi (UART).
// Default state is STOP: stale command, or no valid/fresh obstacle reading
// (forward only) => stop.
void Robot::task_loop() {
    this->motor_driver.stop();
    vTaskDelay(pdMS_TO_TICKS(1000));

    UartCommandType command = UartCommandType::UartCommandType_Stop;
    TickType_t last_command_tick = xTaskGetTickCount();
    float distance_cm = -1.0f;
    TickType_t last_sensor_tick = 0;
    bool have_sensor = false;

    while(true) {
        TickType_t now = xTaskGetTickCount();

        UartCommand uart_command;
        if(xQueueReceive(Queues::uart_command_queue, &uart_command, 0) == pdTRUE) {
            command = uart_command.type;
            last_command_tick = now;
        }

        SensorData sensor;
        if(xQueueReceive(Queues::usonic_sensor_queue, &sensor, 0) == pdTRUE) {
            distance_cm = sensor.distance_cm;
            last_sensor_tick = now;
            have_sensor = true;
        }

        bool command_fresh = (now - last_command_tick) < pdMS_TO_TICKS(COMMAND_TIMEOUT_MS);
        bool front_clear = have_sensor
            && (now - last_sensor_tick) < pdMS_TO_TICKS(SENSOR_TIMEOUT_MS)
            && distance_cm > MIN_FRONT_DISTANCE_CM;  // -1 (sensor error) => not clear

        if(!command_fresh) {
            command = UartCommandType::UartCommandType_Stop;
        }

        switch(command) {
            case UartCommandType::UartCommandType_MoveForward:
                if(front_clear) motor_driver.move_forward(); else motor_driver.stop();
                break;
            case UartCommandType::UartCommandType_MoveBackward:
                motor_driver.move_backward();
                break;
            case UartCommandType::UartCommandType_TurnLeft:
                motor_driver.turn_left();
                break;
            case UartCommandType::UartCommandType_TurnRight:
                motor_driver.turn_right();
                break;
            case UartCommandType::UartCommandType_Stop:
            default:
                motor_driver.stop();
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(ROBOT_TICK_MS));
    }
}
