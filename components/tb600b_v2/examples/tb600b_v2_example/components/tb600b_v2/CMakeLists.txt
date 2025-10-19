idf_component_register(
    SRCS "tb600b_v2.cpp"
    INCLUDE_DIRS "include"
    REQUIRES log cxx esp_driver_uart main
)
