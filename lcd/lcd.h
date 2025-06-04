#ifndef _LCD_H
#define _LCD_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h" // Para sleep_ms no exemplo de main
#include "hardware/gpio.h" // Apenas para o backlight

#include "tft_lcd_ili9341/ili9341/ili9341.h"
#include "tft_lcd_ili9341/gfx/gfx.h"

#define P_BL   14
#define R_FIXO 10000u


#endif