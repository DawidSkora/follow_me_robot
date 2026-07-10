#include "motor_driver.hpp"
#include <stdexcept>

static const int CHANGE_MODE_DELAY_MS = 100;

MotorDriver::MotorDriver(gpio_num_t fl, gpio_num_t fr, gpio_num_t bl, gpio_num_t br) {
    this->front_left = fl;
    this->front_right = fr;
    this->back_left = bl;
    this->back_right = br;
}

MotorDriver::~MotorDriver() {

}

void MotorDriver::soft_state_change(DrivingState state) {
    if(not is_driving_state(state)) {
        this->stop();
        vTaskDelay(CHANGE_MODE_DELAY_MS / portTICK_PERIOD_MS);
        set_driving_state(state);
    }
}

void MotorDriver::left_side_direction(WheelDirection direction) {
    if(direction == WheelDirection::FORWARD) {
        gpio_set_level(back_left, 0);
        gpio_set_level(front_left, 1);
    } else if(direction == WheelDirection::BACKWARD) {
        gpio_set_level(front_left, 0);
        gpio_set_level(back_left, 1);
    } else if(direction == WheelDirection::STOP) {
        gpio_set_level(front_left, 0);
        gpio_set_level(back_left, 0);
    } else {
        // throw std::invalid_argument("Invalid direction");
    }
}

void MotorDriver::right_side_direction(WheelDirection direction) {
    if(direction == WheelDirection::FORWARD) {
        gpio_set_level(back_right, 0);
        gpio_set_level(front_right, 1);
    } else if(direction == WheelDirection::BACKWARD) {
        gpio_set_level(front_right, 0);
        gpio_set_level(back_right, 1);
    } else if(direction == WheelDirection::STOP) {
        gpio_set_level(front_right, 0);
        gpio_set_level(back_right, 0);
    } else {
        //throw std::invalid_argument("Invalid direction");
    }
}

void MotorDriver::move_forward() {
    soft_state_change(DrivingState::FORWARD);
    right_side_direction(WheelDirection::FORWARD);
    left_side_direction(WheelDirection::FORWARD);
}

void MotorDriver::move_backward() {
    soft_state_change(DrivingState::BACKWARD);
    right_side_direction(WheelDirection::BACKWARD);
    left_side_direction(WheelDirection::BACKWARD);
}

void MotorDriver::turn_left() {
    soft_state_change(DrivingState::TUNR_LEFT);
    right_side_direction(WheelDirection::FORWARD);
    left_side_direction(WheelDirection::BACKWARD);
}

void MotorDriver::turn_right() {
    soft_state_change(DrivingState::TURN_RIGHT);
    right_side_direction(WheelDirection::BACKWARD);
    left_side_direction(WheelDirection::FORWARD);
}

void MotorDriver::stop() {
    set_driving_state(DrivingState::STOP);
    right_side_direction(WheelDirection::STOP);
    left_side_direction(WheelDirection::STOP);
}

void MotorDriver::setSpeed(int new_speed) {
    if(new_speed < 0) new_speed = 0;
    if(new_speed > 255) new_speed = 255;
    this->speed = new_speed;
}

void MotorDriver::set_driving_state(DrivingState state) {
    if(not is_driving_state(state)) {
        driving_state = state;
    }
}

bool MotorDriver::is_driving_state(DrivingState state) {
    return driving_state == state;
}
