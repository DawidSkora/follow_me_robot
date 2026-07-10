idf_component_register(
    SRCS "main.cpp" "app.cpp"
    INCLUDE_DIRS "."
    REQUIRES
        ultrasonic_sensor
        motor_driver
        uart
)