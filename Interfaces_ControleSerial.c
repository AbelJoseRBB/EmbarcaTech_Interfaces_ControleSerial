#include <stdio.h>
#include <ctype.h> // Biblioteca para o uso da função isdigit()
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "pio_matrix.pio.h"
#include "Bibliotecas/led_matriz.h"
#include "Bibliotecas/ssd1306.h"
#include "Bibliotecas/font.h"

#define pin_blue 12 
#define pin_green 11

#define botaoA 5
#define botaoB 6

#define I2C_PORT i2c1
#define ADDR 0x3C
#define I2C_SDA 14
#define I2C_SCL 15

// Variáveis globais para controle dos botões e display
volatile bool botaoA_press = false; 
volatile bool botaoB_press = false;
ssd1306_t ssd;

// Declaração de funções que serão utilizadas
void button_isr(uint gpio, uint32_t events);
void setup();

int main()
{
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, matriz5x5);

    setup();    // Chama a função de configuração
    stdio_init_all();   

    while (true) {
        // Verifica se há conexão com o Serial Monitor
        if(stdio_usb_connected()){
            char character = getchar();                     // Lê um caractere recebido via Seria Monitor
            
            //  Verifica se o caractere enviado é um dígito
            if(isdigit(character)){  
                imprimir_num(character, pio, sm);           // Exibe o número digitado na matriz5x5
                printf("Número: %c\n", character);          // Imprime o caracter digitado no Seria Monitor
                ssd1306_fill(&ssd, false);                  // Limpa o display
                ssd1306_draw_char(&ssd, character, 64, 32); // Mostra o caractere digitado no display
                ssd1306_send_data(&ssd);                    //  Atualiza o display 
            }else{
                apagar_leds(pio, sm);                       // Limpa a matriz5x5
                printf("Caracter: %c\n", character);        // Imprime o caracter digitado no Seria Monitor
                
                ssd1306_fill(&ssd, false);                  // Limpa o display
                ssd1306_draw_char(&ssd, character, 64, 32); // Mostra o caractere digitado no display
                ssd1306_send_data(&ssd);                    //  Atualiza o display 
            }
        }
    sleep_ms(50);                                           // Pequena pausa para reduzir o uso da CPU   
    }
}

// Função de interrupção para detectar o pressionamento do botão com debounce
void button_isr(uint gpio, uint32_t events){
    static absolute_time_t last_press = 0;                  // Armazena o tempo do último pressionamento
    absolute_time_t now = get_absolute_time();              // Obtém o tempo atual 

    if(absolute_time_diff_us(last_press, now) > 200000){    // Debounce de 200 ms
        // Verifica se o botão A foi pressionado
        if(gpio == botaoA){                                 
            botaoA_press = !botaoA_press; 
            gpio_put(pin_green, botaoA_press);              // Alterna o estado do LED verde 
            if(gpio_get(pin_green)){                        // Se o LED verde for ligado 
                printf("LED verde: ligado\n");  
                
                // Atualiza o display com a informação "LED verde ligado"
                ssd1306_fill(&ssd, false);  
                ssd1306_draw_string(&ssd, "LED", 5, 30);    
                ssd1306_draw_string(&ssd, "verde:", 35, 30);
                ssd1306_draw_string(&ssd, "ligado", 79, 30);
                ssd1306_send_data(&ssd);    
            }else{                                          // Se o LED verde for desligado 
                printf("LED verde: desligado\n");

                // Atualiza o display com a informação "LED verde desligado"
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "LED", 30, 25);
                ssd1306_draw_string(&ssd, "verde:", 60, 25);
                ssd1306_draw_string(&ssd, "desligado", 30, 40);
                ssd1306_send_data(&ssd);
            }
        }else{
            // Verifica se o botão B foi pressionado
            if(gpio == botaoB){                             
                botaoB_press = !botaoB_press;
                gpio_put(pin_blue, botaoB_press);            // Alterna o estado do LED azul 
                if(gpio_get(pin_blue)){                      // Se o LED azul for ligado
                    printf("LED azul: ligado\n");

                    // Atualiza o display com a informação "LED azul ligado"
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_string(&ssd, "LED", 8, 30);
                    ssd1306_draw_string(&ssd, "azul:", 37, 30);
                    ssd1306_draw_string(&ssd, "ligado", 73, 30);
                    ssd1306_send_data(&ssd);
                }else{                                       // Se o LED azul for desligado 
                    printf("LED azul: desligado\n");
                    
                    // Atualiza o display com a informação "LED azul desligado"
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_string(&ssd, "LED", 35, 25);
                    ssd1306_draw_string(&ssd, "azul:", 65, 25);
                    ssd1306_draw_string(&ssd, "desligado", 30, 40);
                    ssd1306_send_data(&ssd);
                }
            }
        }
        last_press = now;                                    // Atualiza o último tempo de pressionamento 
    }
}

void setup(){
    // Inicializa o LED azul
    gpio_init(pin_blue);
    gpio_set_dir(pin_blue, GPIO_OUT);

    // Inicializa o LED verde
    gpio_init(pin_green);
    gpio_set_dir(pin_green, GPIO_OUT);

    // Inicializa o botão A
    gpio_init(botaoA);
    gpio_set_dir(botaoA, GPIO_IN);
    gpio_pull_up(botaoA);

    // Configura o callback do botão A
    gpio_set_irq_enabled_with_callback(botaoA, GPIO_IRQ_EDGE_FALL, true, &button_isr); 

    // Inicializa o botão B
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);

    // Configura o callback do botão B
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // Inicializa o I2C para comunicação com o display 
    i2c_init(I2C_PORT, 400 * 1000); // Configura a comunicação I2C com velocidade de 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);

    // Inicializa e configura o Display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}