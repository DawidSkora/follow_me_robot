#pragma once

struct SensorData {
    float distance_cm;
};

struct UartCommand {
    char command[32];
};