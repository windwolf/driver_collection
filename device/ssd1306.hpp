#ifndef ___SSD1306_HPP__
#define ___SSD1306_HPP__

#include "os.hpp"
#include "soc.hpp"
namespace ww::device
{

using namespace ww::peripheral;

#define SSD1306_CMD_CONSTRAST_CONTROL 0x81 // CMD0
#define SSD1306_CMD_ENTIRE_DISPLAY_ON 0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_OFF 0xA5
#define SSD1306_CMD_DISPLAY_NORMAL 0xA6
#define SSD1306_CMD_DISPLAY_INVERTED 0xA7
#define SSD1306_CMD_DISPLAY_OFF 0xAE
#define SSD1306_CMD_DISPLAY_ON 0xAF

#define SSD1306_CMD_HORIZONTAL_SCROLL_RIGHT 0x26
#define SSD1306_CMD_HORIZONTAL_SCROLL_LEFT 0x27
#define SSD1306_CMD_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_CMD_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_CMD_DEACTIVATE_SCROLL 0x2E
#define SSD1306_CMD_ACTIVATE_SCROLL 0x2F
#define SSD1306_CMD_SET_VERTICAL_SCROLL_AREA 0xA3

#define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE 0x20

#define SSD1306_CMD_SET_COLUMN_START_ADDRESS_LOWER 0x00  // 0x00-0x0F  set column start address
#define SSD1306_CMD_SET_COLUMN_START_ADDRESS_HIGHER 0x10 // 0x10-0x1F
#define SSD1306_CMD_SET_COLUMN_ADDRESS 0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS 0x22
#define SSD1306_CMD_SET_PAGE_START_ADDRESS 0xB0 // 0xB0-0xB7 set page start address

#define SSD1306_CMD_SET_MULTIPLEX_RATIO 0xA8

#define SSD1306_CMD_SET_DISPLAY_START_LINE 0x40 // 0x40-0x7F map RAM to ROW offset.
#define SSD1306_CMD_SET_DISPLAY_OFFSET 0xD3     // map ROW to COM offset.
#define SSD1306_CMD_SET_MULTIPLEX_RATIO 0xA8    // Act on the ROW.
#define SSD1306_CMD_SET_SEGMENT_REMAP_NORMAL 0xA0
#define SSD1306_CMD_SET_SEGMENT_REMAP_INVERSE 0xA1
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL 0xC0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP 0xC8
#define SSD1306_CMD_SET_COM_PINS_CONFIGURATION 0xDA

#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO 0xD5
#define SSD1306_CMD_SET_PRECHARGE_PERIOD 0xD9
#define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL 0xDB
#define SSD1306_CMD_NOP 0xE3

#define SSD1306_CMD_SET_FADE_OUT_AND_BLINKING 0x23
#define SSD1306_CMD_SET_ZOOM_IN 0xD6

#define SSD1306_CMD_SET_CHARGE_PUMP 0x8D // 0x8D 0x14

#define SSD1306_MAX_COLUMN (128)
#define SSD1306_MAX_PAGE (8)
#define SSD1306_DATA_BUFFER_SIZE (SSD1306_MAX_COLUMN * SSD1306_MAX_PAGE)
#define SSD1306_CMD_BUFFER_SIZE (8)

enum SSD1306_MEMORY_ADDRESSING_MODE : uint8_t
{
    SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL = 0x00,
    SSD1306_MEMORY_ADDRESSING_MODE_VERTICAL = 0x01,
    SSD1306_MEMORY_ADDRESSING_MODE_PAGE = 0x02
};

enum SSD1306_VCOMH_DESELECT_LEVEL : uint8_t
{
    SSD1306_VCOMH_DESELECT_LEVEL_VCC065 = 0x00, // 0.65*Vcc
    SSD1306_VCOMH_DESELECT_LEVEL_VCC077 = 0x20, // 0.77*Vcc
    SSD1306_VCOMH_DESELECT_LEVEL_VCC083 = 0x30, // 0.83*Vcc
};

struct SSD1306Config
{
    uint8_t width : 8;
    uint8_t height : 8;
    SSD1306_MEMORY_ADDRESSING_MODE memoryMode : 8;
    bool enableChargePump : 1;
    bool comInverted : 1;
    bool segmentInverted : 1;
    bool comLeftRightRemap : 1;
    bool comAlternative : 1;
    uint8_t displayStartLine : 6; // 0-63
    uint8_t displayOffset : 6;    // 0-63
    uint8_t multiplexRatio : 6;   // 16-63
    bool displayInverted : 1;
    uint8_t phase1period : 4; // 0-15
    uint8_t phase2period : 4; // 0-15

    SSD1306_VCOMH_DESELECT_LEVEL vcomhDeselectLevel : 8;

    uint8_t fosc : 4;      // 0-15
    uint8_t clkDivide : 4; // 0-15
};

class SSD1306 : public Initializable
{
  public:
    uint8_t dataBuffer[SSD1306_DATA_BUFFER_SIZE];
    uint16_t bufferSize;

    SSD1306(I2cMaster &i2c, EventGroup &eventGroup, uint32_t doneFlag, uint32_t errorFlag);

    SSD1306Config &config_get();

    void lcd_init();
    void pos_set(uint8_t page, uint8_t column);
    void contrast_set(uint8_t contrast);
    void display(bool on);
    void clear();
    void draw();

  private:
    uint8_t _cmdBuffer[SSD1306_CMD_BUFFER_SIZE];
    SSD1306Config _config;
    I2cMaster &_i2c;
    WaitHandler _waitHandler;
    uint32_t _scope;
    void data_send(uint8_t *data, uint16_t dataSize);
    void cmd_send(uint8_t cmdSize);
    void mem_mode_set();
    void on_op_cplt();
    void on_op_err();
};

} // namespace ww::device

#endif // ___SSD1306_HPP__