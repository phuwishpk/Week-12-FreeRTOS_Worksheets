# components/sensor_manager/CMakeLists.txt
idf_component_register(
    SRCS "src/sensor_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES freertos
)