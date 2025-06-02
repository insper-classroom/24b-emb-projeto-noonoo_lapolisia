add_library ( lm35
    lm35.h
    lm35.c
)

    target_link_libraries(lm35 pico_stdlib hardware_adc)

target_include_directories(lm35 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")