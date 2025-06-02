#include "dht11.h"

dht11_reading_t read_dht11_data() {
    dht11_reading_t result = {0.0f, 0.0f, false};
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint pulse_counts[82]; // Suficiente para start + 40 bits + resposta
    int count = 0;

    // Enviar sinal de start
    gpio_set_dir(DHT11_GPIO_PIN, GPIO_OUT);
    gpio_put(DHT11_GPIO_PIN, 0);
    sleep_ms(18); // Manter baixo por pelo menos 18ms
    gpio_put(DHT11_GPIO_PIN, 1);
    sleep_us(30); // Pull up por 20-40us
    gpio_set_dir(DHT11_GPIO_PIN, GPIO_IN);

    // Esperar pela resposta do DHT11
    uint32_t timeout_start = time_us_32();
    while (gpio_get(DHT11_GPIO_PIN) && (time_us_32() - timeout_start < 100)) { // Espera o pino ir para baixo
        tight_loop_contents();
    }
    if (!gpio_get(DHT11_GPIO_PIN)) { // Pino foi para baixo
        timeout_start = time_us_32();
        while (!gpio_get(DHT11_GPIO_PIN) && (time_us_32() - timeout_start < 100)) { // Espera o pino ir para alto (ack low)
            tight_loop_contents();
        }
        if (gpio_get(DHT11_GPIO_PIN)) { // Pino foi para alto
            timeout_start = time_us_32();
            while(gpio_get(DHT11_GPIO_PIN) && (time_us_32() - timeout_start < 100)) { // Espera o pino ir para baixo (ack high)
                tight_loop_contents();
            }
        } else {
            return result; // Falha no handshake
        }
    } else {
        return result; // Falha no handshake
    }


    // Ler os 40 bits de dados
    for (int i = 0; i < 40; ++i) {
        uint32_t low_pulse_start = time_us_32();
        while (!gpio_get(DHT11_GPIO_PIN) && (time_us_32() - low_pulse_start < 100)) { // Tempo em baixo (separador de bit)
            tight_loop_contents();
        }
        if (gpio_get(DHT11_GPIO_PIN)) { // Subiu
             uint32_t high_pulse_start = time_us_32();
            while (gpio_get(DHT11_GPIO_PIN) && (time_us_32() - high_pulse_start < 100)) { // Tempo em alto (dado do bit)
                tight_loop_contents();
            }
            uint32_t high_duration = time_us_32() - high_pulse_start;
            data[i / 8] <<= 1;
            if (high_duration > 40) { // Se o pulso alto for > ~30-40us, é um '1' (DHT11: 26-28us para 0, 70us para 1)
                data[i / 8] |= 1;
            }
        } else {
            return result; // Timeout
        }
    }

    // Verificar checksum
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        result.humidity = (float)data[0] + ((float)data[1] / 10.0f); // DHT11 parte decimal da umidade é geralmente 0
        result.temperature_c = (float)data[2] + ((float)data[3] / 10.0f); // DHT11 parte decimal da temp é geralmente 0
        result.valid = true;
    }
    return result;
}