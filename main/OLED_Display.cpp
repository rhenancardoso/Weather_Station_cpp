#include "OLED_Display.h"
#include "font_Kranky_14.c"
#include "battery_pic.c"

i2c_cmd_handle_t cmd;

OLED_Display::OLED_Display()
{
    send_Command(OLED_CMD_SET_SEGMENT_REMAP);
    send_Command(OLED_CMD_SET_COM_SCAN_MODE);
    send_Command(OLED_CMD_SET_DISPLAY_CLK_DIV);
    send_Command(0x80);
    send_Command(OLED_CMD_SET_MUX_RATIO);
    send_Command(0x3F);
	//
	send_Command(OLED_CMD_SET_DISPLAY_OFFSET);
	send_Command(0x00);
	send_Command(OLED_CMD_SET_DISPLAY_START_LINE);
	send_Command(0x00);
	send_Command(OLED_CMD_SET_MEMORY_ADDR_MODE);
	send_Command(0x00);
	send_Command(OLED_CMD_SET_SEGMENT_MAP | 0x1);
	send_Command(OLED_CMD_SET_COM_SCAN_MODE);
	send_Command(OLED_CMD_SET_COM_PIN_MAP);
	send_Command(0x12);
	send_Command(OLED_CMD_SET_PRECHARGE);
	send_Command(0xF1);
	send_Command(OLED_CMD_SET_VCOMH_DESELCT);
	send_Command(0x30);
    send_Command(OLED_CMD_SET_CHARGE_PUMP);
    send_Command(0x14);
    send_Command(OLED_CMD_DEACTIVATE_SCROLL);
    send_Command(OLED_CMD_DISPLAY_RESUME);
    send_Command(OLED_CMD_DISPLAY_NORMAL);
	//
    send_Command(OLED_CMD_DISPLAY_ON);

	//Data allocation
	buffer = (uint8_t*) malloc(1024);

    printf("%s -> Configured!\n", TAG);
}

void OLED_Display::clear_Display()
{
	memset(buffer,0,1024);
}

void OLED_Display::update_Display()
{	
	send_Command(OLED_CMD_SET_COLUMN_RANGE);
	send_Command(0x00);
	send_Command(127);
	send_Command(OLED_CMD_SET_PAGE_RANGE);
	send_Command(0x00);
	send_Command(0x07);

    for (uint16_t k = 0; k < 1024; k++) {
		i2c_start();
		send_Data((OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE);
		send_Data(OLED_CONTROL_BYTE_DATA_STREAM);
		for (uint8_t j = 0; j < 16; ++j) 
		{
			send_Data(buffer[k]);
			++k;
		}
		--k;
		i2c_databegin();
		i2c_stop();
	}
	send_Command(OLED_CMD_DISPLAY_ON);
}

void OLED_Display::text_Display(const void *arg_text, uint8_t row_Disp, uint8_t col_Disp)
{
	memcpy(buffer,battery_full,14);
}

void OLED_Display::battery_display(int battery_level)
{
	uint8_t i = 0, k = 0, str_size = 0, index = 0;
	uint8_t bat_value[3];

	memset(buffer,0,52);
	if(battery_level > 0)
	{
		if(battery_level >= 75)
		{
			memcpy(buffer,battery_full,14);
		}
		else if(battery_level >= 50 && battery_level < 75)
		{
			memcpy(buffer,battery_medium,14);
		}
		else if(battery_level >= 25 && battery_level < 50)
		{
			memcpy(buffer,battery_low,14);
		}
		else if(battery_level < 25)
		{
			memcpy(buffer,battery_empty,14);
		}

		//Convert percentage into string
		if(battery_level >= 10 && battery_level < 100)
		{
			str_size = 2;
			bat_value[0] = battery_level/10;
			bat_value[1] = battery_level - (bat_value[0]*10);
		}
		else if(battery_level > 0 && battery_level < 10)
		{
			str_size = 1;
			bat_value[0] = battery_level;
		}
		else 
		{
			str_size = 3;
			bat_value[0] = 1;
			bat_value[1] = 0;
			bat_value[2] = 0;
		}

		index = 20;
		for(i = 0; i < str_size; i++)
		{
			for(k = 0; k < 8; k++)
			{
				buffer[index] = font_8x8[bat_value[i]+48][k];
				index++;
			} 
		}
		for(k = 0; k < 8; k++)
		{
			buffer[index] = font_8x8[37][k];
			index++;
		}
		
	}
	
}

void OLED_Display::drawPixel(uint8_t x, uint8_t y)
{
	uint16_t index = x + (y / 8) * OLED_WIDTH;
	buffer[index] |= (1 << (y & 7));
}

void OLED_Display::i2c_start()
{
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
}
void OLED_Display::i2c_stop()
{
	i2c_master_stop(cmd);
	i2c_cmd_link_delete(cmd);
}
void OLED_Display::i2c_databegin()
{
	esp_err_t err_SendData;
	err_SendData = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
}

void OLED_Display::send_Data(uint8_t addr_reg)
{
	i2c_master_write_byte(cmd, addr_reg, true);

}

void OLED_Display::send_Command(uint8_t addr_reg)
{
    esp_err_t err_SendCommand;
	cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_SINGLE, true);

	i2c_master_write_byte(cmd, addr_reg, true);
	i2c_master_stop(cmd);

	err_SendCommand = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}