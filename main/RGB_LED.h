// LEDC Library
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html

#include "driver/ledc.h"
#include "driver/gpio.h"

#define TAG "RGB LED"
#define NUM_MAX_LEDS 100

class RGB_LED 
{
    public:
        RGB_LED(uint8_t leds_n, gpio_num_t gpio_num);
        void set_ALL_and_updateRGB( uint8_t R, uint8_t G, uint8_t B);
        void set_LED_24bitColor(uint8_t led_position, uint8_t R, uint8_t G, uint8_t B);
        void update_RGB_LEDs(void);
        
    private:
        gpio_num_t n_gpio;
        uint8_t n_leds;
        uint8_t RGC_Color24bit[NUM_MAX_LEDS][24];

        void config_ESP32_GPIO(void);
        void reset_RGB_LED(void);
        void RGB_0_bit(void);
        void RGB_1_bit(void);
        void convert_24bitColor_uint(uint8_t led_position, uint8_t Color_value,  uint8_t Color_index);
};