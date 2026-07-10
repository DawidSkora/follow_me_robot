#pragma once

#include "config.hpp"
#include "motor_driver.hpp"

class Robot {
private:
    MotorDriver motor_driver(FORWARD_LEFT, FORWARD_RIGHT, BACKWARD_LEFT, BACKWARD_RIGHT);
    
    void task();
    void _search_for_the_way();
public:   
    void start_task();
};