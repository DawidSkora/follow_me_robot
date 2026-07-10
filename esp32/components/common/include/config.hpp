#pragma once

#include <cstdlib>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

// motors control
#define FORWARD_LEFT GPIO_NUM_16
#define BACKWARD_LEFT GPIO_NUM_17
#define FORWARD_RIGHT GPIO_NUM_18
#define BACKWARD_RIGHT GPIO_NUM_5

// embedded led
#define LED_PIN GPIO_NUM_2


// ultrasonic sensor
#define TRIG_PIN GPIO_NUM_33
#define ECHO_PIN GPIO_NUM_32

// UART1 - odbiór z Raspberry Pi 5
#define UART_RPI       UART_NUM_1
#define RPI_RX_PIN     GPIO_NUM_16   // do TX Raspberry Pi
#define RPI_TX_PIN     GPIO_NUM_17   // do RX Raspberry Pi (opcjonalne)

static const constexpr uint32_t UART_BAUD = 115200;
static const constexpr uint32_t BUF_SIZE = 1024;

// min distance after which cars stops
static const constexpr float MIN_FRONT_DISTANCE_CM = 20.0;