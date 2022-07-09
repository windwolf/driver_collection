#include "ssd1306.h"
#include "os/os.h"
#include "common/device.h"
#include "stdio.h"

#define SSD1306_DATA_STREAM 0x40
#define SSD1306_DATA_SINGLE 0xC0
#define SSD1306_COMMAND_STREAM 0x00
#define SSD1306_COMMAND_SINGLE 0x80

static void ssd1306_cmd(SSD1306 *instance, uint8_t cmdSize)
{
    if (cmdSize == 1)
    {
        i2c_device_mem_write(instance->i2c, 0x78, SSD1306_COMMAND_SINGLE, instance->cmd_buffer, cmdSize, DEVICE_DATAWIDTH_8);
    }
    else
    {
        i2c_device_mem_write(instance->i2c, 0x78, SSD1306_COMMAND_STREAM, instance->cmd_buffer, cmdSize, DEVICE_DATAWIDTH_8);
    }
}
static void ssd1306_data(SSD1306 *instance, uint8_t *data, uint16_t size)
{
    if (size == 1)
    {
        i2c_device_mem_write(instance->i2c, 0x78, SSD1306_DATA_SINGLE, data, size, DEVICE_DATAWIDTH_8);
    }
    else
    {
        i2c_device_mem_write(instance->i2c, 0x78, SSD1306_DATA_STREAM, data, size, DEVICE_DATAWIDTH_8);
    }
}

static void ssd1306_setMemMode(SSD1306 *instance)
{
    instance->cmd_buffer[0] = SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE;
    instance->cmd_buffer[1] = (uint8_t)instance->memoryMode;
    ssd1306_cmd(instance, 2);
}

void ssd1306_create(SSD1306 *instance, I2CDevice *i2c)
{
    instance->i2c = i2c;
    // instance->memoryMode = SSD1306_MEMORY_ADDRESSING_MODE_PAGE;
    // instance->enableChargePump = false;
    // instance->comInverted = false;
    // instance->segmentInverted = false;
    // instance->comLeftRightRemap = false;
    // instance->comAlternative = true;
    // instance->displayStartLine = 0;
    // instance->displayOffset = 0;
    // instance->multiplexRatio = 0x3F;
    // instance->phase1period = 0x02;
    // instance->phase2period = 0x02;
    // instance->vcomhDeselectLevel = 0x20;
    // instance->fosc = 0x80;
    // instance->clkDivide = 0x00;
    instance->buffer_size = instance->width * instance->height / 8;
}

void ssd1306_display(SSD1306 *instance, bool on)
{
    if (on)
    {
        if (instance->enableChargePump)
        {
            instance->cmd_buffer[0] = SSD1306_CMD_SET_CHARGE_PUMP;
            instance->cmd_buffer[1] = 0x14;
            ssd1306_cmd(instance, 2);
        }
        else
        {
            instance->cmd_buffer[0] = SSD1306_CMD_SET_CHARGE_PUMP;
            instance->cmd_buffer[1] = 0x04;
            ssd1306_cmd(instance, 2);
        }
        instance->cmd_buffer[0] = SSD1306_CMD_DISPLAY_ON;
        ssd1306_cmd(instance, 1);
    }
    else
    {
        instance->cmd_buffer[0] = SSD1306_CMD_DISPLAY_OFF;
        ssd1306_cmd(instance, 1);
        instance->cmd_buffer[0] = SSD1306_CMD_SET_CHARGE_PUMP;
        instance->cmd_buffer[1] = 0x04;
        ssd1306_cmd(instance, 2);
    }
}

void ssd1306_set_contrast(SSD1306 *instance, uint8_t contrast)
{
    instance->cmd_buffer[0] = SSD1306_CMD_CONSTRAST_CONTROL;
    instance->cmd_buffer[1] = contrast;
    ssd1306_cmd(instance, 2);
}

void ssd1306_set_pos(SSD1306 *instance, uint8_t page, uint8_t column)
{
    if (instance->memoryMode == SSD1306_MEMORY_ADDRESSING_MODE_PAGE)
    {

        instance->cmd_buffer[0] = SSD1306_CMD_SET_PAGE_START_ADDRESS | page;
        ssd1306_cmd(instance, 1);
        instance->cmd_buffer[0] = SSD1306_CMD_SET_COLUMN_START_ADDRESS_LOWER | (column & 0x0F);
        ssd1306_cmd(instance, 1);
        instance->cmd_buffer[0] = SSD1306_CMD_SET_COLUMN_START_ADDRESS_HIGHER | (column >> 4);
        ssd1306_cmd(instance, 1);
    }
    else
    {

        instance->cmd_buffer[0] = SSD1306_CMD_SET_PAGE_ADDRESS;
        instance->cmd_buffer[1] = page;
        instance->cmd_buffer[2] = (instance->height - 1) / 8;
        ssd1306_cmd(instance, 3);
        instance->cmd_buffer[0] = SSD1306_CMD_SET_COLUMN_ADDRESS;
        instance->cmd_buffer[1] = column;
        instance->cmd_buffer[2] = instance->width - 1;
        ssd1306_cmd(instance, 3);
    }
}

void ssd1306_clear(SSD1306 *instance)
{

    for (uint16_t i = 0; i < instance->buffer_size; i++)
    {
        instance->cmd_buffer[i] = 0x00;
    }

    SSD1306_MEMORY_ADDRESSING_MODE oldMode = instance->memoryMode;
    instance->memoryMode = SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL;
    ssd1306_setMemMode(instance);

    ssd1306_set_pos(instance, 0, 0);

    ssd1306_data(instance, instance->data_buffer, instance->buffer_size);

    instance->memoryMode = oldMode;
    ssd1306_setMemMode(instance);
}

//初始化SSD1306
void ssd1306_init(SSD1306 *instance)
{
    ww_os_delay(100);

    ssd1306_display(instance, false);

    ssd1306_setMemMode(instance);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_DISPLAY_START_LINE | instance->displayStartLine;
    ssd1306_cmd(instance, 1);

    if (instance->comInverted)
    {
        instance->cmd_buffer[0] = SSD1306_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP;
    }
    else
    {
        instance->cmd_buffer[0] = SSD1306_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL;
    }
    ssd1306_cmd(instance, 1);

    if (instance->segmentInverted)
    {
        instance->cmd_buffer[0] = SSD1306_CMD_SET_SEGMENT_REMAP_INVERSE;
    }
    else
    {
        instance->cmd_buffer[0] = SSD1306_CMD_SET_SEGMENT_REMAP_NORMAL;
    }
    ssd1306_cmd(instance, 1);

    if (instance->displayInverted)
    {
        instance->cmd_buffer[0] = SSD1306_CMD_DISPLAY_INVERTED;
    }
    else
    {
        instance->cmd_buffer[0] = SSD1306_CMD_DISPLAY_NORMAL;
    }
    ssd1306_cmd(instance, 1);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_MULTIPLEX_RATIO;
    instance->cmd_buffer[1] = instance->multiplexRatio;
    ssd1306_cmd(instance, 2);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_DISPLAY_OFFSET;
    instance->cmd_buffer[1] = instance->displayOffset;
    ssd1306_cmd(instance, 2);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO;
    instance->cmd_buffer[1] = (uint8_t)((instance->fosc << 4) | instance->clkDivide);
    ssd1306_cmd(instance, 2);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_PRECHARGE_PERIOD;
    instance->cmd_buffer[1] = instance->phase1period | (instance->phase2period << 4);
    ssd1306_cmd(instance, 2);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_COM_PINS_CONFIGURATION;
    instance->cmd_buffer[1] = 0x02 | (instance->comLeftRightRemap << 5) | (instance->comAlternative << 4);
    ssd1306_cmd(instance, 2);

    instance->cmd_buffer[0] = SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL;
    instance->cmd_buffer[1] = instance->vcomhDeselectLevel;
    ssd1306_cmd(instance, 2);

    instance->cmd_buffer[0] = SSD1306_CMD_ENTIRE_DISPLAY_ON;
    ssd1306_cmd(instance, 1);

    ssd1306_clear(instance);

    ww_os_delay(100);

    ssd1306_display(instance, true);

    ww_os_delay(100);
};

void ssd1306_draw(SSD1306 *instance)
{
    ssd1306_set_pos(instance, 0, 0);
    ssd1306_data(instance, instance->data_buffer, instance->buffer_size);
}