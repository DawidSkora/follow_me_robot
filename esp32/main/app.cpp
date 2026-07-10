#include "app.hpp"
#include "ultrasonic_sensor.hpp"
#include "motor_driver.hpp"
#include "uart.hpp"

void App::_init_pins() {
        // bit mask with output pins
    uint64_t bit_mask = 0;
    bit_mask = (1ULL << FORWARD_LEFT) | (1ULL << FORWARD_RIGHT) | (1ULL << BACKWARD_LEFT) | (1ULL << BACKWARD_RIGHT) | (1ULL << TRIG_PIN) | (1ULL << LED_PIN);

    // register output pins
    gpio_config_t output_config = {};
    output_config.intr_type = GPIO_INTR_DISABLE;
    output_config.mode = GPIO_MODE_OUTPUT;
    output_config.pin_bit_mask = bit_mask;
    output_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    output_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&output_config);

    // register input pins
    gpio_config_t input_config = {};
    input_config.pin_bit_mask = (1ULL << ECHO_PIN);
    input_config.mode = GPIO_MODE_INPUT;
    input_config.pull_up_en = GPIO_PULLUP_DISABLE;
    input_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    input_config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&input_config);
}

void App::run() {
    _init_pins();

    Uart uart(UART_BAUD, BUF_SIZE);
    uart.init(UART_RPI, RPI_TX_PIN, RPI_RX_PIN);

}