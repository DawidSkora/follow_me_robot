#include "uart.hpp"
#include "queues.hpp"
#include "esp_log.h"

static const char* TAG = "uart";

Uart::Uart(uint32_t baudrate, uint32_t buffer) {
    _baudrate = baudrate;
    _buffer = buffer;
}

void Uart::listen() {
    uint8_t byte = 0;

    while (true) {
        // Read one byte at a time so a command is handled immediately
        // (a larger buffer would wait for the whole timeout).
        int length = uart_read_bytes(_port, &byte, 1, pdMS_TO_TICKS(100));
        if (length != 1) {
            continue;
        }
        ESP_LOGD(TAG, "rx byte: 0x%02x '%c'", byte, (byte >= 32 && byte < 127) ? (char)byte : '?');

        UartCommand command;
        switch ((char)byte) {
            case 'f':
                command.type = UartCommandType::UartCommandType_MoveForward;
                break;
            case 'b':
                command.type = UartCommandType::UartCommandType_MoveBackward;
                break;
            case 'l':
                command.type = UartCommandType::UartCommandType_TurnLeft;
                break;
            case 'r':
                command.type = UartCommandType::UartCommandType_TurnRight;
                break;
            case 's':
                command.type = UartCommandType::UartCommandType_Stop;
                break;
            default:
                continue;  // Skip unknown bytes
        }

        // Queue holds only the latest command; Robot applies it and
        // treats it as stale after COMMAND_TIMEOUT_MS.
        xQueueOverwrite(Queues::uart_command_queue, &command);
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