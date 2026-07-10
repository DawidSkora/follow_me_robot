idf_component_register(
    SRCS
        "motor_driver.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp_driver_gpio
)