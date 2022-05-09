#include "I2C_Config.h"

I2C_Config::I2C_Config()
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf_i2c;
    conf_i2c.mode = I2C_MODE_MASTER;
    conf_i2c.sda_io_num = I2C_MASTER_SDA_IO;
    conf_i2c.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_i2c.scl_io_num = I2C_MASTER_SCL_IO;
    conf_i2c.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_i2c.master.clk_speed = 400000;
    conf_i2c.clk_flags = 0;
    i2c_param_config(i2c_master_port, &conf_i2c);
    I2C_status = i2c_driver_install(i2c_master_port, conf_i2c.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if(I2C_status == ESP_OK)
    {printf("%s -> Configured!\n", TAG);}
    else{printf("%s  -> ERROR!\n", TAG);}
}
