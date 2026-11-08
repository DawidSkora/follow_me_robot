#pragma once

struct SensorData {
    float distance_cm;
};

enum class UartCommandType {
    UartCommandType_MoveForward,
    UartCommandType_MoveBackward,
    UartCommandType_TurnLeft,
    UartCommandType_TurnRight,
    UartCommandType_Stop
};

struct UartCommand {
    UartCommandType type;
};
