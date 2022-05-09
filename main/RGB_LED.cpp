#include "RGB_LED.h"

RGB_LED::RGB_LED(uint8_t leds_n, gpio_num_t gpio_num)
{
    n_leds = leds_n;
    n_gpio = gpio_num;
    config_ESP32_GPIO();
    reset_RGB_LED();
    for(uint8_t ind = 0; ind <= leds_n; ind++){set_LED_24bitColor(ind, 0, 0, 0);}
    update_RGB_LEDs();
    printf("%s -> Configured!\n", TAG);
}

void RGB_LED::config_ESP32_GPIO()
{
    gpio_set_direction(n_gpio, GPIO_MODE_OUTPUT);
}

void RGB_LED::reset_RGB_LED()
{
    uint16_t delay_us = 0;
    gpio_set_level(n_gpio,0);
    while(delay_us < 1500){delay_us++;}     // > 80us
}

void RGB_LED::RGB_0_bit()
{
    uint16_t delay_us = 0;
    gpio_set_level(n_gpio,1);
    while(delay_us < 1){delay_us++;}        // 0.3us
    delay_us = 0;
    gpio_set_level(n_gpio,0);
    while(delay_us < 5){delay_us++;}        // 0.9us
}

void RGB_LED::RGB_1_bit()
{
    uint16_t delay_us = 0;
    gpio_set_level(n_gpio,1);
    while(delay_us < 5){delay_us++;}        // 0.9us
    delay_us = 0;
    gpio_set_level(n_gpio,0);
    while(delay_us < 1){delay_us++;}        // 0.3us
}

void RGB_LED::set_LED_24bitColor(uint8_t led_position, uint8_t R, uint8_t G, uint8_t B)
{
    convert_24bitColor_uint(led_position, G,  0);
    convert_24bitColor_uint(led_position, R,  1);
    convert_24bitColor_uint(led_position, B,  2);
}

void RGB_LED::convert_24bitColor_uint(uint8_t led_position, uint8_t Color_value,  uint8_t Color_index)
{
    uint8_t hex_value = 0;
    uint8_t array_index = Color_index*8;

    //MSB -> 7 = 128
    if(Color_value >= 128){RGC_Color24bit[led_position][array_index] = 1; hex_value = hex_value + 128;}
    else{RGC_Color24bit[led_position][array_index] = 0;}
    //Bit -> 6 = 64
    if(Color_value >= hex_value + 64){RGC_Color24bit[led_position][array_index+1] = 1; hex_value = hex_value + 64;}
    else{RGC_Color24bit[led_position][array_index+1] = 0;}
    //Bit -> 5 = 32
    if(Color_value >= hex_value + 32){RGC_Color24bit[led_position][array_index+2] = 1; hex_value = hex_value + 32;}
    else{RGC_Color24bit[led_position][array_index+2] = 0;}
    //Bit -> 4 = 16
    if(Color_value >= hex_value + 16){RGC_Color24bit[led_position][array_index+3] = 1; hex_value = hex_value + 16;}
    else{RGC_Color24bit[led_position][array_index+3] = 0;}
    //Bit -> 3 = 8
    if(Color_value >= hex_value + 8){RGC_Color24bit[led_position][array_index+4] = 1; hex_value = hex_value + 8;}
    else{RGC_Color24bit[led_position][array_index+4] = 0;}
    //Bit -> 2 = 4
    if(Color_value >= hex_value + 4){RGC_Color24bit[led_position][array_index+5] = 1; hex_value = hex_value + 4;}
    else{RGC_Color24bit[led_position][array_index+5] = 0;}
    //Bit -> 1 = 2
    if(Color_value >= hex_value + 2){RGC_Color24bit[led_position][array_index+6] = 1; hex_value = hex_value + 2;}
    else{RGC_Color24bit[led_position][array_index+6] = 0;}
    //Bit -> 0 = 1
    if(Color_value >= hex_value + 1){RGC_Color24bit[led_position][array_index+7] = 1; hex_value = hex_value + 1;}
    else{RGC_Color24bit[led_position][array_index+7] = 0;}
}

void RGB_LED::set_ALL_and_updateRGB( uint8_t R, uint8_t G, uint8_t B)
{
    for (uint8_t led_ind = 0;  led_ind < n_leds; led_ind++)
    {
        convert_24bitColor_uint(led_ind, G,  0);
        convert_24bitColor_uint(led_ind, R,  1);
        convert_24bitColor_uint(led_ind, B,  2);
    }
    update_RGB_LEDs();
}

void RGB_LED::update_RGB_LEDs()
{
    //for loop for each LED
    reset_RGB_LED();
    for(uint8_t num_RGB_led = 0; num_RGB_led < n_leds; num_RGB_led++)
    {
        //Set GPIO output for each Bit within the array
        for(uint bit_index = 0; bit_index < 24; bit_index++)
        {
            if(RGC_Color24bit[num_RGB_led][bit_index] == 0){RGB_0_bit();}
            else{RGB_1_bit();}
        }
    }
    reset_RGB_LED();
}
