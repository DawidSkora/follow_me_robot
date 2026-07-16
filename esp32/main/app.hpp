#pragma once

#include "config.hpp"
#include "ultrasonic_sensor.hpp"
#include "robot.hpp"
#include "uart.hpp"

class App {
private:
    void _init_pins();
    UltrasonicSensor ultrasonic{TRIG_PIN, ECHO_PIN};
    Robot robot;
    Uart uart{UART_BAUD, BUF_SIZE};
public:
    void run();
};
