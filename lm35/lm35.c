#include "lm35.h"

float read_lm35_temperature(const char unit) {
    adc_select_input(LM35_ADC_PIN_NUM); // Seleciona o canal ADC do LM35
    const float conversionFactor = 3.3f / (1 << 12);
    float adc_voltage = (float)adc_read() * conversionFactor;
    // LM35 tem uma saída de 10mV por grau Celsius
    float tempC = adc_voltage * 100.0f;

    if (unit == 'C') {
        return tempC;
    } else if (unit == 'F') {
        return tempC * 9.0f / 5.0f + 32.0f;
    } else {
        return tempC; // Padrão Celsius
    }
}