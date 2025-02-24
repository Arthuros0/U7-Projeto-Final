#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

#define BUTTON_B 6

ssd1306_t ssd; //Váriavel do tipo ssd1306_t, armazena as informações relacionados ao display

absolute_time_t button_debounce;

void button_callback(uint gpio,uint32_t events);
void init_button(uint8_t pin);

int main()
{
    stdio_init_all();

    i2c_init(I2C_PORT,400*1000);
    init_i2c_pins(I2C_SDA,I2C_SCL);

    init_button(BUTTON_B);

    gpio_set_irq_enabled_with_callback(BUTTON_B,GPIO_IRQ_EDGE_FALL,true,&button_callback);

    init_display(&ssd,ENDERECO,I2C_PORT);

    button_debounce=delayed_by_ms(get_absolute_time(), 200);


    bool cor=false;
    while (true) {
        cor=!cor;
        printf("Hello, world!\n");
        sleep_ms(5000);
    }
}


void init_button(uint8_t pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}
  
void button_callback(uint gpio,uint32_t events){
    if (time_reached(button_debounce)) //Verifica se o tempo de debounce foi atigindo
    {
        if(gpio == BUTTON_B){
        reset_usb_boot(0, 0);
        }
        button_debounce=delayed_by_ms(get_absolute_time(), 200); //Atualiza debounce
    }

}