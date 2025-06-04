add_library ( lcd
    lcd.h
    lcd.c
)

target_link_libraries(lcd hardware_adc harware_gpio pico_stdlib hardware_spi hardware_dma) # includes

target_include_directories(lcd PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/tft_lcd_ili9341/ili9341           # headers ili9341
    ${CMAKE_CURRENT_LIST_DIR}/tft_lcd_ili9341/gfx               # headers gfx
    )