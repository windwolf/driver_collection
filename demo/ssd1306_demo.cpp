#include "ssd1306_demo.hpp"
#include "device/ssd1306.hpp"
#include "pin.hpp"
#define LOG_MODULE "SSD1306demo"
#include "log.h"
#include "fonts.hpp"
#include "os.hpp"

using namespace wibot;
using namespace wibot::device;
using namespace wibot::peripheral;
using namespace wibot::graph;



Pin scl0_91(*GPIOB, GPIO_PIN_6);
Pin sda0_91(*GPIOB, GPIO_PIN_7);
I2cMaster softi2c0_91(scl0_91, sda0_91);
EventGroup eg("");
SSD1306 ZJ0_91in(softi2c0_91, eg);


CanvasInfo canvasInfo_ZJ0_91in {
    .width = 128,
    .height = 32,
    .pixelSize = PixelSize::Bit1,
    .direction = CanvasMemoryLayoutDirection::Vertical,

};

Pin scl0_96(*GPIOA, GPIO_PIN_12);
Pin sda0_96(*GPIOA, GPIO_PIN_15);
I2cMaster softi2c0_96(scl0_96, sda0_96);

SSD1306 ZJ0_96in(softi2c0_96, eg);

CanvasInfo canvasInfo_ZJ0_96in = {
    .width = 128,
    .height = 64,
    .pixelSize = PixelSize::Bit1,
    .direction = CanvasMemoryLayoutDirection::Vertical,

};

FontDrawInfo fontDrawInfo {
    .foreColor = 0x1,
    .backColor = 0x0,
    .spacing = 0,
    .lineSpacing = 1,
};

static void device_init()
{
    ZJ0_91in.config.width = 128;
    ZJ0_91in.config.height = 32;
    ZJ0_91in.config.memoryMode = SSD1306_MEMORY_ADDRESSING_MODE_VERTICAL;
    ZJ0_91in.config.enableChargePump = true;
    ZJ0_91in.config.comInverted = true;
    ZJ0_91in.config.segmentInverted = true;
    ZJ0_91in.config.comLeftRightRemap = false;
    ZJ0_91in.config.comAlternative = false;
    ZJ0_91in.config.displayInverted = false;
    ZJ0_91in.config.displayStartLine = 0;
    ZJ0_91in.config.displayOffset = 0;
    ZJ0_91in.config.multiplexRatio = 31;
    ZJ0_91in.config.phase1period = 0x02;
    ZJ0_91in.config.phase2period = 0x02;
    ZJ0_91in.config.vcomhDeselectLevel = SSD1306_VCOMH_DESELECT_LEVEL_VCC077;
    ZJ0_91in.config.fosc = 0x0F;
    ZJ0_91in.config.clkDivide = 0x00;
    ZJ0_91in.apply_config();
    ZJ0_91in.init();

    ZJ0_96in.config.width = 128;
    ZJ0_96in.config.height = 64;
    ZJ0_96in.config.memoryMode = SSD1306_MEMORY_ADDRESSING_MODE_VERTICAL;
    ZJ0_96in.config.enableChargePump = true;
    ZJ0_96in.config.comInverted = true;
    ZJ0_96in.config.segmentInverted = true;
    ZJ0_96in.config.comLeftRightRemap = false;
    ZJ0_96in.config.comAlternative = true;
    ZJ0_96in.config.displayInverted = false;
    ZJ0_96in.config.displayStartLine = 0;
    ZJ0_96in.config.displayOffset = 0;
    ZJ0_96in.config.multiplexRatio = 63;
    ZJ0_96in.config.phase1period = 0x01;
    ZJ0_96in.config.phase2period = 0x0F;
    ZJ0_96in.config.vcomhDeselectLevel = SSD1306_VCOMH_DESELECT_LEVEL_VCC077,
    ZJ0_96in.config.fosc = 0x0F;
    ZJ0_96in.config.clkDivide = 0x00;
    ZJ0_96in.apply_config();
    ZJ0_96in.init();
}

void ssd1306_demo(void)
{
    device_init();

    while (1)
    {
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 0, "0123456789ABCDEFGHIJK", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 8, "123456789ABCDEFGHIJK0", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 16, "23456789ABCDEFGHIJK01", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 24, "3456789ABCDEFGHIJK012", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 32, "456789ABCDEFGHIJK0123", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 40, "56789ABCDEFGHIJK01234", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 48, "6789ABCDEFGHIJK012345", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_96in.dataBuffer, &canvasInfo_ZJ0_96in, 0, 56, "789ABCDEFGHIJK0123456", &Font6x8_v, &fontDrawInfo);
        ZJ0_96in.draw();
        LOG_I("0.96in OLED drawed");

        FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 0, "0", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 8, "1", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 16, "2", &Font6x8_v, &fontDrawInfo);
        FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 24, "3", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 0, "0123456789ABCDEFGHIJK", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 8, "123456789ABCDEFGHIJK0", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 16, "23456789ABCDEFGHIJK01", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 24, "3456789ABCDEFGHIJK012", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 32, "456789ABCDEFGHIJK0123", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 40, "56789ABCDEFGHIJK01234", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 48, "6789ABCDEFGHIJK012345", &Font6x8_v, &fontDrawInfo);
        // FONTS_FillData(ZJ0_91in.dataBuffer, &canvasInfo_ZJ0_91in, 0, 56, "789ABCDEFGHIJK0123456", &Font6x8_v, &fontDrawInfo);
        ZJ0_91in.draw();
        LOG_I("0.91in OLED drawed");
    }
}
