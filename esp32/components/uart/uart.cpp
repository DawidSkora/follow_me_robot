#include "uart.hpp"

Uart::Uart(uint32_t baudrate, uint32_t buffer) {
    _baudrate = baudrate;
    _buffer = buffer;
}

void Uart::listen() {
    uint8_t data[128];
    int length = 0;

    while (true) {
        // Read data from UART
        length = uart_read_bytes(_port, data, sizeof(data), pdMS_TO_TICKS(100));

        // Process each received byte as a command
        for (int i = 0; i < length; i++) {
            char cmd = (char)data[i];
            UartCommandType command;

            // Parse command from character
            switch (cmd) {
                case 'f':
                    command = UartCommandType::UartCommandType_MoveForward;
                    break;
                case 'b':
                    command = UartCommandType::UartCommandType_MoveBackward;
                    break;
                case 'l':
                    command = UartCommandType::UartCommandType_TurnLeft;
                    break;
                case 'r':
                    command = UartCommandType::UartCommandType_TurnRight;
                    break;
                case 's':
                    command = UartCommandType::UartCommandType_Stop;
                    break;
                default:
                    continue;  // Skip unknown commands
            }

            // TODO: Handle the parsed command (e.g., dispatch to handler or store in queue)
        }
    }
}

void Uart::init(uart_port_t port, int tx_pin, int rx_pin) {
    _port = port;  // Store port for use in listen()
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

void Uart::start_task() {
    xTaskCreatePinnedToCore(task, "uart", 4096, this, 1, nullptr, 1);
}

void Uart::task(void* arg) {
    auto* self = static_cast<Uart*>(arg);
    self->listen();
}