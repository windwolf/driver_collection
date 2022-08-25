#include "ssd1306.hpp"

#include "os.hpp"

#define SSD1306_DATA_STREAM 0x40
#define SSD1306_DATA_SINGLE 0xC0
#define SSD1306_COMMAND_STREAM 0x00
#define SSD1306_COMMAND_SINGLE 0x80

namespace ww::device
{
using namespace ww::peripheral;
using namespace ww::os;

Result SSD1306::init()
{
    bufferSize = _config.width * _config.height / 8;
    _i2c.config_get().slaveAddress = 0x78;
    _i2c.config_get().dataWidth = DATAWIDTH_8;
    return _i2c.init();
};
Result SSD1306::deinit()
{
    return Result_OK;
};

SSD1306Config &SSD1306::config_get()
{
    return _config;
};

void SSD1306::on_op_cplt(){

};

void SSD1306::on_op_err(){

};

void SSD1306::cmd_send(uint8_t cmdSize)
{
    if (cmdSize == 1)
    {
        _i2c.write(SSD1306_COMMAND_SINGLE, _cmdBuffer, cmdSize, _waitHandler);
        _waitHandler.wait(TIMEOUT_FOREVER);
    }
    else
    {
        _i2c.write(SSD1306_COMMAND_STREAM, _cmdBuffer, cmdSize, _waitHandler);
        _waitHandler.wait(TIMEOUT_FOREVER);
    }
};
void SSD1306::data_send(uint8_t *data, uint16_t dataSize)
{
    if (dataSize == 1)
    {
        _i2c.write(SSD1306_DATA_SINGLE, data, dataSize, _waitHandler);
        _waitHandler.wait(TIMEOUT_FOREVER);
    }
    else
    {
        _i2c.write(SSD1306_DATA_STREAM, data, dataSize, _waitHandler);
        _waitHandler.wait(TIMEOUT_FOREVER);
    }
};

void SSD1306::mem_mode_set()
{
    _cmdBuffer[0] = SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE;
    _cmdBuffer[1] = (uint8_t)_config.memoryMode;
    cmd_send(2);
};

void SSD1306::display(bool on)
{
    if (on)
    {
        if (_config.enableChargePump)
        {
            _cmdBuffer[0] = SSD1306_CMD_SET_CHARGE_PUMP;
            _cmdBuffer[1] = 0x14;
            cmd_send(2);
        }
        else
        {
            _cmdBuffer[0] = SSD1306_CMD_SET_CHARGE_PUMP;
            _cmdBuffer[1] = 0x04;
            cmd_send(2);
        }
        _cmdBuffer[0] = SSD1306_CMD_DISPLAY_ON;
        cmd_send(1);
    }
    else
    {
        _cmdBuffer[0] = SSD1306_CMD_DISPLAY_OFF;
        cmd_send(1);
        _cmdBuffer[0] = SSD1306_CMD_SET_CHARGE_PUMP;
        _cmdBuffer[1] = 0x04;
        cmd_send(2);
    }
};

void SSD1306::contrast_set(uint8_t contrast)
{
    _cmdBuffer[0] = SSD1306_CMD_CONSTRAST_CONTROL;
    _cmdBuffer[1] = contrast;
    cmd_send(2);
};

void SSD1306::pos_set(uint8_t page, uint8_t column)
{
    if (_config.memoryMode == SSD1306_MEMORY_ADDRESSING_MODE_PAGE)
    {
        _cmdBuffer[0] = SSD1306_CMD_SET_PAGE_START_ADDRESS | page;
        cmd_send(1);
        _cmdBuffer[0] =
            SSD1306_CMD_SET_COLUMN_START_ADDRESS_LOWER | (column & 0x0F);
        cmd_send(1);
        _cmdBuffer[0] =
            SSD1306_CMD_SET_COLUMN_START_ADDRESS_HIGHER | (column >> 4);
        cmd_send(1);
    }
    else
    {
        _cmdBuffer[0] = SSD1306_CMD_SET_PAGE_ADDRESS;
        _cmdBuffer[1] = page;
        _cmdBuffer[2] = (_config.height - 1) / 8;
        cmd_send(3);
        _cmdBuffer[0] = SSD1306_CMD_SET_COLUMN_ADDRESS;
        _cmdBuffer[1] = column;
        _cmdBuffer[2] = _config.width - 1;
        cmd_send(3);
    }
};

void SSD1306::clear()
{
    for (uint16_t i = 0; i < bufferSize; i++)
    {
        _cmdBuffer[i] = 0x00;
    }

    SSD1306_MEMORY_ADDRESSING_MODE oldMode = _config.memoryMode;
    _config.memoryMode = SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL;
    mem_mode_set();

    pos_set(0, 0);

    data_send(dataBuffer, bufferSize);

    _config.memoryMode = oldMode;
    mem_mode_set();
};

//初始化SSD1306
void SSD1306::lcd_init()
{
    Thread::sleep(100);

    display(false);

    mem_mode_set();

    _cmdBuffer[0] =
        SSD1306_CMD_SET_DISPLAY_START_LINE | _config.displayStartLine;
    cmd_send(1);

    if (_config.comInverted)
    {
        _cmdBuffer[0] = SSD1306_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP;
    }
    else
    {
        _cmdBuffer[0] = SSD1306_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL;
    }
    cmd_send(1);

    if (_config.segmentInverted)
    {
        _cmdBuffer[0] = SSD1306_CMD_SET_SEGMENT_REMAP_INVERSE;
    }
    else
    {
        _cmdBuffer[0] = SSD1306_CMD_SET_SEGMENT_REMAP_NORMAL;
    }
    cmd_send(1);

    if (_config.displayInverted)
    {
        _cmdBuffer[0] = SSD1306_CMD_DISPLAY_INVERTED;
    }
    else
    {
        _cmdBuffer[0] = SSD1306_CMD_DISPLAY_NORMAL;
    }
    cmd_send(1);

    _cmdBuffer[0] = SSD1306_CMD_SET_MULTIPLEX_RATIO;
    _cmdBuffer[1] = _config.multiplexRatio;
    cmd_send(2);

    _cmdBuffer[0] = SSD1306_CMD_SET_DISPLAY_OFFSET;
    _cmdBuffer[1] = _config.displayOffset;
    cmd_send(2);

    _cmdBuffer[0] = SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO;
    _cmdBuffer[1] = (uint8_t)((_config.fosc << 4) | _config.clkDivide);
    cmd_send(2);

    _cmdBuffer[0] = SSD1306_CMD_SET_PRECHARGE_PERIOD;
    _cmdBuffer[1] = _config.phase1period | (_config.phase2period << 4);
    cmd_send(2);

    _cmdBuffer[0] = SSD1306_CMD_SET_COM_PINS_CONFIGURATION;
    _cmdBuffer[1] =
        0x02 | (_config.comLeftRightRemap << 5) | (_config.comAlternative << 4);
    cmd_send(2);

    _cmdBuffer[0] = SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL;
    _cmdBuffer[1] = _config.vcomhDeselectLevel;
    cmd_send(2);

    _cmdBuffer[0] = SSD1306_CMD_ENTIRE_DISPLAY_ON;
    cmd_send(1);

    clear();

    Thread::sleep(100);

    display(true);

    Thread::sleep(100);
};

void SSD1306::draw()
{
    pos_set(0, 0);
    data_send(dataBuffer, bufferSize);
}

} // namespace ww::device