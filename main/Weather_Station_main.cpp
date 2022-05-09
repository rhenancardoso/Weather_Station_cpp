
#include <stdio.h>
#include <driver/adc.h>
#include <driver/adc_common.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"    
#include "I2C_Config.h" 
#include "OLED_Display.h" 
#include "RGB_LED.h" 
#include "TMP117.h" 
#include "fonts.h"

#define num_leds        3

class RGB_LED RGB_leds(num_leds,GPIO_NUM_25);

void print_ESP32_info()
{
        /* Print chip information */
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);
        printf("This is %s chip with %d CPU cores, WiFi%s%s",
                CONFIG_IDF_TARGET,
                chip_info.cores,
                (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
                (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
        printf("silicon revision %d, ", chip_info.revision);
        printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
                (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
        printf("Free heap: %d\n", esp_get_free_heap_size());
}

void ADC_config()
{
        adc2_config_channel_atten(ADC2_CHANNEL_9, ADC_ATTEN_6db);
}
int battery_read_percent()
{
        int read_raw = 0, bat_pcnt = 0;
        esp_err_t r = adc2_get_raw( ADC2_CHANNEL_9, ADC_WIDTH_BIT_10, &read_raw);
        if(r == ESP_OK)
        {
                printf("Raw Value: %d\n", read_raw);
                //10bits - 1024 -> 1.33V = 413 / 1.067V = 332
                bat_pcnt = ((float)(read_raw-550)/(695-550))*100;
                printf("Battery %d\n",bat_pcnt);

                if(bat_pcnt >= 75){RGB_leds.set_ALL_and_updateRGB(0,30,0);}
                else if(bat_pcnt >= 50){RGB_leds.set_ALL_and_updateRGB(18,30,0);}
                else if(bat_pcnt >= 25){RGB_leds.set_ALL_and_updateRGB(30,18,0);}
                else {RGB_leds.set_ALL_and_updateRGB(30,0,0);}
        }
        return bat_pcnt;     
}

void reset_ESP32()
{
        printf("Restarting now.\n");
        fflush(stdout);
        esp_restart();
}

extern "C" void app_main()
{
        print_ESP32_info();
        //SETUP
        ADC_config();
        class I2C_Config I2C_comm_setup;
        class OLED_Display OLED_disp;
        class TMP117_Temp Temp_Sensor;

        //MAIN CODE
        OLED_disp.clear_Display();
        printf("Weather Station v2.0\n");
        RGB_leds.set_ALL_and_updateRGB(0,3,32);vTaskDelay(750 / portTICK_PERIOD_MS);
        RGB_leds.set_ALL_and_updateRGB(0,0,0);vTaskDelay(250 / portTICK_PERIOD_MS);
        while(1)
        {
                Temp_Sensor.read_Temp();
                printf("Temp Sensor: %2.1f\n", Temp_Sensor.temp_value);
                printf("MIN: %2.1f, MAX: %2.1f\n", Temp_Sensor.min_temp, Temp_Sensor.max_temp);
                vTaskDelay(500 / portTICK_PERIOD_MS);

                //Check Battery color
                OLED_disp.battery_display(battery_read_percent());

                //Update Screen
                OLED_disp.update_Display();                                                                                                                                                                                                                                                     ;
                vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        
}
