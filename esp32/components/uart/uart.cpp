#include "uart.hpp"

Uart::Uart(uint32_t baudrate, uint32_t buffer) {
    _baudrate = baudrate;
    _buffer = buffer;
}

void Uart::listen() {
    
}

void Uart::init(uart_port_t port, int tx_pin, int rx_pin) {
    uart_config_t cfg = {};
    cfg.baud_rate = _baudrate;
    cfg.data_bits = UART_DATA_8_BITS;
    cfg.parity    = UART_PARITY_DISABLE;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    cfg.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(port, _buffer * 2, 0, 0, nullptr, 0));
    ESP_ERROR_CHECK(uart_param_config(port, &cfg));
    ESP_ERROR_CHECK(uart_set_pin(port, tx_pin, rx_pin,
                                  UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}