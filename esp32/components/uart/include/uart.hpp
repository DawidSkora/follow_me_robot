#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"

enum class UartCommandType {
    UartCommandType_MoveForward,
    UartCommandType_MoveBackward,
    UartCommandType_TurnLeft,
    UartCommandType_TurnRight,
    UartCommandType_Stop
};

class Uart {
private:
    uint32_t _baudrate;
    uint32_t _buffer;
    uart_port_t _port;

    static void task(void* arg);

public:
    Uart(uint32_t baudrate, uint32_t buffer);
    void init(uart_port_t port, int tx_pin, int rx_pin);
    void listen();
    void start_task();
};