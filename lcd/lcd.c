#include "lcd.h"

// Variáveis globais para armazenar o valor a ser exibido e um sinalizador
static uint16_t g_valor_raw_para_exibir;
static bool g_novos_dados_para_exibir = false;

void apresentar_novo_valor_na_tela(uint16_t valor_raw) {
    g_valor_raw_para_exibir = valor_raw;
    g_novos_dados_para_exibir = true;
}

void atualizar_tela_com_dados() {
    if (g_novos_dados_para_exibir) {
        uint16_t entrada = g_valor_raw_para_exibir;
        g_novos_dados_para_exibir = false; // Reseta o sinalizador

        // Cálculos baseados no valor de entrada (raw)
        uint32_t mv = (uint32_t)entrada * 3300 / 4095; // Tensão em mV

        // Assumindo que 'entrada' é a leitura de um divisor de tensão onde:
        // 3.3V --- R_VAR --- PINO_ADC --- R_FIXO --- GND
        // 'mv' é a tensão sobre R_FIXO.
        // R_VAR = R_FIXO * (3300 - mv) / mv = (R_FIXO * 3300 / mv) - R_FIXO
        uint32_t res = mv > 0 ? (R_FIXO * 3300 / mv - R_FIXO) : 0; // Resistência de R_VAR em Ohms
        
        // Corrente através do circuito: I = V_RFIXO / R_FIXO = mv / R_FIXO (resultado em mA)
        // Para obter microAmps: I_uA = (mv * 1000) / R_FIXO
        uint32_t microA = (mv > 0 && R_FIXO > 0) ? ((uint32_t)mv * 1000 / R_FIXO) : 0;
        // Se preferir a fórmula original de corrente do seu código anterior:
        // uint32_t microA = (3300 - mv) * 1000 / R_FIXO;

        // Atualização da tela
        GFX_clearScreen();
        GFX_setCursor(8, 12);
        GFX_setTextColor(ILI9341_WHITE); // Cor do texto
        GFX_setTextBack(ILI9341_BLACK); // Cor de fundo do texto (pode ser definida uma vez fora se não mudar)
        
        GFX_printf("U: %4u mV\n", mv);
        GFX_setCursor(8, 38);
        GFX_printf("R: %5u Ohm\n", res);
        GFX_setCursor(8, 64);
        GFX_printf("I: %4u uA\n", microA);
        GFX_setCursor(8, 90);
        GFX_printf("RAW: %u\n", entrada);
        
        GFX_flush(); // Envia os dados para o display
    }
}

int main(void) {
    // Inicializações do sistema
    stdio_init_all(); // Para serial USB (debug)
    
    // Inicialização do GPIO do Backlight
    gpio_init(P_BL);
    gpio_set_dir(P_BL, GPIO_OUT);
    gpio_put(P_BL, 1); // Liga o backlight

    // Inicialização do Display LCD
    LCD_initDisplay();
    LCD_setRotation(1);    // Define a rotação (0-3)
    GFX_createFramebuf();  // Cria o framebuffer para a biblioteca GFX

    // Exemplo de como usar:
    // Em uma aplicação real, apresentar_novo_valor_na_tela() seria chamada
    // quando o seu subsistema de leitura ADC tivesse um novo valor.
    uint16_t valor_teste_raw = 2048; // Começa com um valor médio (3.3V/2)
    uint32_t tempo_ultima_atualizacao_simulada_ms = 0;

    while (true) {
        uint32_t agora_ms = to_ms_since_boot(get_absolute_time());

        // Simulação de chegada de novos dados a cada 1 segundo para demonstração
        if (agora_ms - tempo_ultima_atualizacao_simulada_ms >= 1000) {
            tempo_ultima_atualizacao_simulada_ms = agora_ms;
            
            apresentar_novo_valor_na_tela(valor_teste_raw);
            
            // Altera o valor de teste para a próxima iteração
            valor_teste_raw += 100;
            if (valor_teste_raw > 4095) {
                valor_teste_raw = 100; // Reinicia com um valor baixo
            }
        }

        // Chama a função de atualização da tela. Ela só fará o trabalho pesado
        // de redesenhar se houver novos dados.
        atualizar_tela_com_dados();

        // Pequeno delay para não sobrecarregar o processador no loop principal,
        // especialmente se não houver outras tarefas.
        // Em um sistema maior, isso pode não ser necessário ou pode ser gerenciado de outra forma.
        sleep_ms(10); 
    }

    return 0; // Nunca alcançado
}