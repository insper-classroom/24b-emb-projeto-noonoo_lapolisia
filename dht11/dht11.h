#ifndef _DHT11_H
#define _DHT11_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define DHT11_GPIO_PIN 15

typedef struct {
    float humidity;
    float temperature_c;
    bool valid;
} dht11_reading_t;

dht11_reading_t read_dht11_data();

#endif