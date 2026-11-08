#pragma once

#include <cstdlib>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/uart.h"

// motors control
static constexpr gpio_num_t FORWARD_LEFT = GPIO_NUM_16;
static constexpr gpio_num_t BACKWARD_LEFT = GPIO_NUM_17;
static constexpr gpio_num_t FORWARD_RIGHT = GPIO_NUM_18;
static constexpr gpio_num_t BACKWARD_RIGHT = GPIO_NUM_5;

// embedded led
static constexpr gpio_num_t LED_PIN = GPIO_NUM_2;

// ultrasonic sensor
static constexpr gpio_num_t TRIG_PIN = GPIO_NUM_33;
static constexpr gpio_num_t ECHO_PIN = GPIO_NUM_32;

// UART1 - odbiór z Raspberry Pi 5
static constexpr uart_port_t UART_RPI = UART_NUM_1;
// static constexpr gpio_num_t RPI_RX_PIN = GPIO_NUM_16;   // do TX Raspberry Pi
// static constexpr gpio_num_t RPI_TX_PIN = GPIO_NUM_17;   // do RX Raspberry Pi (opcjonalne)
static constexpr gpio_num_t RPI_RX_PIN = GPIO_NUM_21;   // do TX Raspberry Pi
static constexpr gpio_num_t RPI_TX_PIN = GPIO_NUM_22;   // do RX Raspberry Pi (opcjonalne)

static constexpr uint32_t UART_BAUD = 115200;
static constexpr uint32_t BUF_SIZE = 1024;

// min distance after which cars stops
static constexpr float MIN_FRONT_DISTANCE_CM = 20.0f;

// safety timeouts (robot stops when they expire)
static constexpr uint32_t COMMAND_TIMEOUT_MS = 500;  // no fresh command from RPi
static constexpr uint32_t SENSOR_TIMEOUT_MS = 300;   // no fresh ultrasonic reading
static constexpr uint32_t ROBOT_TICK_MS = 20;        // control loop period
