#include "ldr.h"

int read_luminosity() {
    adc_select_input(LDR_ADC_PIN_NUM);
    uint16_t raw = adc_read(); // valor de 0 a 4095
    float voltage = raw * 3.3f / 4095.0f;

    if (voltage < 0.6f) {
        return 1;
    } else if (voltage < 1.5f) {
        return 2;
    } else if (voltage < 2.9f) {
        return 3;
    } else {
        return 4;
    }
}
