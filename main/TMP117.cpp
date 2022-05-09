#include "TMP117.h"

TMP117_Temp::TMP117_Temp()
{
    temp_value = 0; 
	max_temp = -50;
	min_temp = 100;

    esp_err_t err_ConfigSensor;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, TMP117_WRITE_ADDRESS, true);
	i2c_master_write_byte(cmd, TMP117_CONFIG, true);
	i2c_master_write_byte(cmd, MSB_CONFIG, true);
	i2c_master_write_byte(cmd, LSB_CONFIG, true);
	i2c_master_stop(cmd);
	err_ConfigSensor = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
    if(err_ConfigSensor != ESP_OK){printf("%s -> ERROR: Temp configuring!\n", TAG);}
    else {printf("%s -> Configured!\n", TAG);}  
}

void TMP117_Temp::read_Temp()
{
    uint8_t data[2] = {0};
	uint16_t datac = 0;
    datac = 0;
	//Request Temp Reading
    send_Command(TMP117_TEMP_RESULT);
	//Read Temperature values
    esp_err_t err_SendData;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, TMP117_READ_ADDRESS, true);
	i2c_master_read(cmd, data, 2, I2C_MASTER_ACK);
	i2c_master_stop(cmd);
	err_SendData = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
    //if(err_SendData != ESP_OK){printf("%s -> ERROR: Temp reading!\n", TAG);}

	datac = ((data[0] << 8) | data[1]);
	temp_value = datac * 0.0078125;
	//Set extremes values
	if(temp_value < min_temp){min_temp = temp_value;}
	if(temp_value > max_temp){max_temp = temp_value;}
}

void TMP117_Temp::send_Command(uint8_t addr_reg)
{
    esp_err_t err_SendCommand;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, TMP117_WRITE_ADDRESS, true);
	i2c_master_write_byte(cmd, addr_reg, true);
	i2c_master_stop(cmd);
	err_SendCommand = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    //if(err_SendCommand != ESP_OK){printf("%s -> ERROR: Send Command!\n", TAG);}
	i2c_cmd_link_delete(cmd);
}