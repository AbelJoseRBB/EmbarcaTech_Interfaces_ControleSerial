#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include <ctype.h>
#include "pio_matrix.pio.h"
#include "./Bibliotecas/led_matriz.h"
#include "./Bibliotecas/botao.h"

#define pin_blue 12
#define pin_green 11

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void inicializar_pinos(){
    gpio_init(pin_blue);
    gpio_set_dir(pin_blue, GPIO_OUT);

    gpio_init(pin_green);
    gpio_set_dir(pin_green, GPIO_OUT);
}

int main()
{
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, matriz5x5);

    inicializar_pinos();
    inicializar_botao();

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    stdio_init_all();
    
    while (true) {
        char comando = getchar();
        
        if(isdigit(comando)){
            imprimir_num(comando, pio, sm);
            printf("Número: %c\n", comando);
        }else{
            apagar_leds(pio, sm);
            printf("Caracter: %c\n", comando);
        }
        sleep_ms(10);
    }
}
