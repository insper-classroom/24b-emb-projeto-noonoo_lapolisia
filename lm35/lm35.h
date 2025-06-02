#ifndef _LM35_H
#define _LM35_H

#include "pico/stdlib.h"
#include "hardware/adc.h"

#define LM35_ADC_PIN_NUM 1  // ADC1 (corresponde a GP27)
#define LM35_GPIO_PIN 27    // GP27 para inicialização do ADC

float read_lm35_temperature(const char unit);

#endif