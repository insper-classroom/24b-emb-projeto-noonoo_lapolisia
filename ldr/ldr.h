#ifndef _LDR_H
#define _LDR_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "math.h"

#define LDR_ADC_PIN_NUM 0  // ADC0 (corresponde a GP26)
#define LDR_GPIO_PIN 26

int read_luminosity();

#endif