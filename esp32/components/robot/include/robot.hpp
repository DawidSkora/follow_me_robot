#pragma once

#include "config.hpp"
#include "motor_driver.hpp"

class Robot {
private:
    MotorDriver motor_driver{FORWARD_LEFT, FORWARD_RIGHT, BACKWARD_LEFT, BACKWARD_RIGHT};
    
    static void task(void* arg);
    void task_loop();
public:   
    void start_task();
};