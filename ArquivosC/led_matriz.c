#include "Bibliotecas/led_matriz.h"

// Define a matriz do número 0
double num0[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 1
double num1[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0
};

// Define a matriz do número 2
double num2[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 3
double num3[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 4
double num4[25] = {
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0
};

// Define a matriz do número 5
double num5[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 6
double num6[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 7
double num7[25] = {
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 8
double num8[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Define a matriz do número 9
double num9[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
};

// Função para converter valores RGB em um valor de 32 bits no formato específico da matriz.
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

//Função para apagar todos os LEDS
void apagar_leds(PIO pio, uint sm) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, matrix_rgb(0.0, 0.0, 0.0));
    }
}

//Função para acender todos os LEDS
void acender_leds(PIO pio, uint sm, double r, double g, double b) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, matrix_rgb(b, r, g));
    }
}

// Função para desenhar a matriz
void desenho_pio(double *desenho, PIO pio, uint sm, double r, double g, double b){
    for(int16_t i = 0; i < NUM_PIXELS; i++){
        pio_sm_put_blocking(pio, sm, matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b));
    }   
}

// Função para imprimir o número recebido 
void imprimir_num(char caracter, PIO pio, uint sm){
    switch(caracter){
        case '0': desenho_pio(num0, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '1': desenho_pio(num1, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '2': desenho_pio(num2, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '3': desenho_pio(num3, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '4': desenho_pio(num4, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '5': desenho_pio(num5, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '6': desenho_pio(num6, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '7': desenho_pio(num7, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '8': desenho_pio(num8, pio, sm, 0.5, 0.6, 0.2);
                  break;
        case '9': desenho_pio(num9, pio, sm, 0.5, 0.6, 0.2);
                  break;
        default: break;
    }
}

