#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

class MotorDriver {
private:
    enum class DrivingState { FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT, STOP };
    enum class WheelDirection { FORWARD, BACKWARD, STOP };
    DrivingState driving_state;
    gpio_num_t front_left;
    gpio_num_t front_right;
    gpio_num_t back_left;
    gpio_num_t back_right;
    int speed;

    void set_driving_state(DrivingState state);
    bool is_driving_state(DrivingState state);
    void soft_state_change(DrivingState state);
    void left_side_direction(WheelDirection direction);
    void right_side_direction(WheelDirection direction);
public:
    MotorDriver(gpio_num_t fl, gpio_num_t fr, gpio_num_t bl, gpio_num_t br);
    ~MotorDriver();
    void move_forward();
    void move_backward();
    void turn_left();
    void turn_right();
    void stop();
    void setSpeed(int speed);
};