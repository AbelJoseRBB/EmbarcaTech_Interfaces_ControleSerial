#ifndef BOTAO_H
#define BOTAO_H

#include <stdio.h>
#include "pico/stdlib.h"

#define pin_blue 12
#define pin_green 11
#define botaoA 5
#define botaoB 6

extern volatile bool botaoA_press;
extern volatile bool botaoB_press;

void inicializar_botao();
void button_isr(uint gpio, uint32_t events);

#endif