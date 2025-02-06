#include <stdio.h>
#include <ctype.h>
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

volatile bool botaoA_press = false; 
volatile bool botaoB_press = false;
ssd1306_t ssd;

void button_isr(uint gpio, uint32_t events){
    static absolute_time_t last_press = 0;
    absolute_time_t now = get_absolute_time();

    if(absolute_time_diff_us(last_press, now) > 200000){
        if(gpio == botaoA){
            botaoA_press = !botaoA_press;
            gpio_put(pin_green, botaoA_press);
            if(gpio_get(pin_green)){
                printf("LED verde: ligado\n");
                
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "LED", 5, 30);
                ssd1306_draw_string(&ssd, "verde:", 35, 30);
                ssd1306_draw_string(&ssd, "ligado", 79, 30);
                ssd1306_send_data(&ssd);
            }else{
                printf("LED verde: desligado\n");

                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "LED", 30, 25);
                ssd1306_draw_string(&ssd, "verde:", 60, 25);
                ssd1306_draw_string(&ssd, "desligado", 30, 40);
                ssd1306_send_data(&ssd);
            }
        }else{
            if(gpio == botaoB){
                botaoB_press = !botaoB_press;
                gpio_put(pin_blue, botaoB_press);
                if(gpio_get(pin_blue)){
                    printf("LED azul: ligado\n");

                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_string(&ssd, "LED", 8, 30);
                    ssd1306_draw_string(&ssd, "azul:", 37, 30);
                    ssd1306_draw_string(&ssd, "ligado", 73, 30);
                    ssd1306_send_data(&ssd);
                }else{
                    printf("LED azul: desligado\n");

                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_string(&ssd, "LED", 35, 25);
                    ssd1306_draw_string(&ssd, "azul:", 65, 25);
                    ssd1306_draw_string(&ssd, "desligado", 30, 40);
                    ssd1306_send_data(&ssd);
                }
            }
        }
        last_press = now;
    }
}

void setup(){
    gpio_init(pin_blue);
    gpio_set_dir(pin_blue, GPIO_OUT);

    gpio_init(pin_green);
    gpio_set_dir(pin_green, GPIO_OUT);

    gpio_init(botaoA);
    gpio_set_dir(botaoA, GPIO_IN);
    gpio_pull_up(botaoA);
    gpio_set_irq_enabled_with_callback(botaoA, GPIO_IRQ_EDGE_FALL, true, &button_isr);
    
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // Inicializa o I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);

    // Inicializa o Display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

int main()
{
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, matriz5x5);

    setup();
    stdio_init_all();

    while (true) {
        if(stdio_usb_connected()){
            char caracter = getchar();
            
            if(isdigit(caracter)){
                imprimir_num(caracter, pio, sm);
                printf("Número: %c\n", caracter);
                ssd1306_fill(&ssd, false);
                ssd1306_draw_char(&ssd, caracter, 64, 32);
                ssd1306_send_data(&ssd);
            }else{
                apagar_leds(pio, sm);
                printf("Caracter: %c\n", caracter);
                
                ssd1306_fill(&ssd, false);
                ssd1306_draw_char(&ssd, caracter, 64, 32);
                ssd1306_send_data(&ssd);
            }
        }
        sleep_ms(10);
    }
}
