#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "char_font.h"
#include "driver/i2c.h"

#define OLED_WIDTH                      128
#define OLED_HEIGHT                     64
// SSD1306 OLED DISPLAY DEFINITIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// SLA (0x3C) + WRITE_MODE (0x00) =  0x78 (0b01111000)

#define OLED_I2C_ADDRESS                0x3C

// Control byte
#define OLED_CONTROL_BYTE_CMD_SINGLE    0x80
#define OLED_CONTROL_BYTE_CMD_STREAM    0x00
#define OLED_CONTROL_BYTE_DATA_STREAM   0x40

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST           0x81    // follow with 0x7F
#define OLED_CMD_DISPLAY_RESUME         0xA4
#define OLED_CMD_DISPLAY_ALLON          0xA5
#define OLED_CMD_DISPLAY_NORMAL         0xA6
#define OLED_CMD_DISPLAY_INVERTED       0xA7
#define OLED_CMD_DISPLAY_OFF            0xAE
#define OLED_CMD_DISPLAY_ON             0xAF

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE   0x20    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define OLED_CMD_SET_COLUMN_RANGE       0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE         0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE 0x40
#define OLED_CMD_SET_SEGMENT_MAP        0xA0  
#define OLED_CMD_SET_SEGMENT_REMAP      0xA1    
#define OLED_CMD_SET_MUX_RATIO          0xA8    // follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE      0xC8    
#define OLED_CMD_SET_DISPLAY_OFFSET     0xD3    // follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP        0xDA    // follow with 0x12
#define OLED_CMD_NOP                    0xE3    // NOP

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV    0xD5    // follow with 0x80
#define OLED_CMD_SET_PRECHARGE          0xD9    // follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT      0xDB    // follow with 0x30

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_DEACTIVATE_SCROLL      0x2E    
#define OLED_CMD_ACTIVATE_SCROLL        0x2F    
#define OLED_CMD_VET_ACTV_SCROLL        0xA3    

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP        0x8D    // follow with 0x14

#define TAG "OLED DISPLAY"

class OLED_Display
{
    public:
        OLED_Display();
        void clear_Display(void);
        void drawPixel(uint8_t x, uint8_t y);
        void battery_display(int battery_level);
        void update_Display(void);
        void text_Display(const void *arg_text, uint8_t row_Disp, uint8_t col_Disp);
        //void image_Display(uint8_t *img_array, uint8_t row_Disp, uint8_t col_Disp, uint8_t sizeX, uint8_t sizeY, bool inverse);
        //void full_image_Display(uint8_t *img_array, bool inverse);
        
    private:
        uint8_t *buffer;
        void send_Command(uint8_t addr_reg);
        void send_Data(uint8_t addr_reg);
        void i2c_start();
        void i2c_stop();
        void i2c_databegin();
};