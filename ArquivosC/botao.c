#include "../Bibliotecas/botao.h"


volatile bool botaoA_press = false;
volatile bool botaoB_press = false;

void inicializar_botao(){
    gpio_init(botaoA);
    gpio_set_dir(botaoA, GPIO_IN);
    gpio_pull_up(botaoA);
    gpio_set_irq_enabled_with_callback(botaoA, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &button_isr);
}

void button_isr(uint gpio, uint32_t events){
    static absolute_time_t last_press = 0;
    absolute_time_t now = get_absolute_time();

    if(absolute_time_diff_us(last_press, now) > 200000){
        if(gpio == botaoA){
            botaoA_press = !botaoA_press;
            gpio_put(pin_green, botaoA_press);
            if(gpio_get(pin_green)){
            printf("LED verde: ligado\n");
            }else{
                printf("LED verde: desligado\n");
            }
        }else{
            if(gpio == botaoB){
                botaoB_press = !botaoB_press;
                gpio_put(pin_blue, botaoB_press);
                if(gpio_get(pin_blue)){
                printf("LED azul: ligado\n");
                }else{
                printf("LED azul: desligado\n");
            }
            }
        }
        last_press = now;
    }
}
