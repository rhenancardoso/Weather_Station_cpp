#include "driver/i2c.h"

// TMP117 DEFINITIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#define TMP117_READ_ADDRESS         0x91
#define TMP117_WRITE_ADDRESS        0x90

// TMP117 REGISTERS
#define TMP117_TEMP_RESULT          0x00
#define TMP117_CONFIG               0x01
#define TMP117_THIGH_LIMIT          0x02
#define TMP117_TLOW_LIMIT           0x03
#define TMP117_TEMP_OFFSET          0x07
#define TMP117_DEVICE_ID            0x0F

#define MSB_CONFIG 0x60
#define LSB_CONFIG 0x01


#define TAG "TMP117"

class TMP117_Temp
{
    public:
        uint8_t device_id;
        float temp_value;
        float max_temp;
        float min_temp;

        TMP117_Temp();
        void read_Temp(void);
        
    private:
        void send_Command(uint8_t addr_reg);
};