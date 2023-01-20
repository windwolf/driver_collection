#ifndef __WWDEVICE_ST7735_HPP__
#define __WWDEVICE_ST7735_HPP__

#include "os.hpp"
#include "soc.hpp"
#include "st77xx.hpp"
#include "graph.hpp"

namespace wibot::device
{
using namespace wibot::accessor;
using namespace wibot::peripheral;
using namespace wibot::graph;
enum ST7735_CMD
{
    ST7735_CMD_NOP = 0x00U,                        /* No Operation: NOP                           */
    ST7735_CMD_SOFTWARE_RESET = 0x01U,             /* Software reset: SWRESET          */
    ST7735_CMD_READ_DISPLAY_ID = 0x04U,            /* Read Display ID: RDDID         */
    ST7735_CMD_READ_DISPLAY_STATUS = 0x09U,        /* Read Display Statu: RDDST     */
    ST7735_CMD_READ_DISPLAY_POWER_MODE = 0x0AU,    /* Read Display Power: RDDPM */
    ST7735_CMD_READ_DISPLAY_MADCTL = 0x0BU,        /* Read Display: RDDMADCTL     */
    ST7735_CMD_READ_DISPLAY_PIXEL_FORMAT = 0x0CU,  /* Read Display Pixel: RDDCOLMOD               */
    ST7735_CMD_READ_DISPLAY_IMAGE_MODE = 0x0DU,    /* Read Display Image: RDDIM */
    ST7735_CMD_READ_DISPLAY_SIGNAL_MODE = 0x0EU,   /* Read Display Signal: RDDSM                  */
    ST7735_CMD_SLEEP_IN = 0x10U,                   /* Sleep in & booster off: SLPIN  */
    ST7735_CMD_SLEEP_OUT = 0x11U,                  /* Sleep out & booster on: SLPOUT */
    ST7735_CMD_PARTIAL_DISPLAY_MODE_ON = 0x12U,    /* Partial mode on: PTLON */
    ST7735_CMD_NORMAL_DISPLAY_MODE_ON = 0x13U,     /* Partial off (Normal): NORON */
    ST7735_CMD_DISPLAY_INVERSION_OFF = 0x20U,      /* Display inversion off: INVOFF               */
    ST7735_CMD_DISPLAY_INVERSION_ON = 0x21U,       /* Display inversion on: INVON */
    ST7735_CMD_GAMMA_SET = 0x26U,                  /* Gamma curve select: GAMSET            */
    ST7735_CMD_DISPLAY_OFF = 0x28U,                /* Display off: DISPOFF          */
    ST7735_CMD_DISPLAY_ON = 0x29U,                 /* Display on: DISPON           */
    ST7735_CMD_COLUMN_ADDRESS_SET = 0x2AU,         /* Column address set: CASET   */
    ST7735_CMD_ROW_ADDRESS_SET = 0x2BU,            /* Row address set: RASET      */
    ST7735_CMD_MEMORY_WRITE = 0x2CU,               /* Memory write: RAMWR         */
    ST7735_CMD_RGBSET = 0x2DU,                     /* LUT for 4k,65k,262k color: RGBSET           */
    ST7735_CMD_MEMORY_READ = 0x2EU,                /* Memory read: RAMRD  */
    ST7735_CMD_PARTIAL_AREA = 0x30U,               /* Partial start/end address set: PTLAR */
    ST7735_CMD_TEARING_EFFECT_LINE_OFF = 0x34U,    /* Tearing effect line off: TEOFF              */
    ST7735_CMD_TEARING_EFFECT_LINE_ON = 0x35U,     /* Tearing effect mode set & on: TEON          */
    ST7735_CMD_MEMORY_DATA_ACCESS_CONTROL = 0x36U, /* Memory data access control: MADCTL          */
    ST7735_CMD_IDLE_MODE_OFF = 0x38U,              /* Idle mode off: IDMOFF */
    ST7735_CMD_IDLE_MODE_ON = 0x39U,               /* Idle mode on: IDMON  */
    ST7735_CMD_INTERFACE_PIXEL_FORMAT = 0x3AU,     /* Interface pixel format: COLMOD              */
    ST7735_CMD_FRAME_RATE_CTRL1 = 0xB1U,           /* In normal mode (Full colors): FRMCTR1       */
    ST7735_CMD_FRAME_RATE_CTRL2 = 0xB2U,           /* In Idle mode (8-colors): FRMCTR2 */
    ST7735_CMD_FRAME_RATE_CTRL3 = 0xB3U,           /* In partial mode + Full colors: FRMCTR3      */
    ST7735_CMD_FRAME_INVERSION_CTRL = 0xB4U,       /* Display inversion control: INVCTR           */
    ST7735_CMD_DISPLAY_SETTING = 0xB6U,            /* Display function setting */
    ST7735_CMD_PWR_CTRL1 = 0xC0U,                  /* Power control setting: PWCTR1       */
    ST7735_CMD_PWR_CTRL2 = 0xC1U,                  /* Power control setting: PWCTR2       */
    ST7735_CMD_PWR_CTRL3 = 0xC2U,                  /* In normal mode (Full colors): PWCTR3 */
    ST7735_CMD_PWR_CTRL4 = 0xC3U,                  /* In Idle mode (8-colors): PWCTR4 */
    ST7735_CMD_PWR_CTRL5 = 0xC4U,                  /* In partial mode + Full colors: PWCTR5 */
    ST7735_CMD_VCOMH_VCOML_CTRL1 = 0xC5U,          /* VCOM control 1: VMCTR1 */
    ST7735_CMD_VMOF_CTRL = 0xC7U,                  /* Set VCOM offset control: VMOFCTR         */
    ST7735_CMD_WRID2 = 0xD1U,                      /* Set LCM version code: WRID2                 */
    ST7735_CMD_WRID3 = 0xD2U,                      /* Customer Project code: WRID3                */
    ST7735_CMD_NV_CTRL1 = 0xD9U,                   /* NVM control status: NVCTR1 */
    ST7735_CMD_READ_ID1 = 0xDAU,                   /* Read ID1: RDID1 */
    ST7735_CMD_READ_ID2 = 0xDBU,                   /* Read ID2: RDID2 */
    ST7735_CMD_READ_ID3 = 0xDCU,                   /* Read ID3: RDID3 */
    ST7735_CMD_NV_CTRL2 = 0xDEU,                   /* NVM Read Command: NVCTR2 */
    ST7735_CMD_NV_CTRL3 = 0xDFU,                   /* NVM Write Command: NVCTR3 */
    ST7735_CMD_POSITIVE_VOLTAGE_GAMMA_CONTROL =
        0xE0U, /* Set Gamma adjustment (+ polarity): GAMCTRP1 */
    ST7735_CMD_NAGATIVE_VALTAGE_GAMMA_CONTROL =
        0xE1U,                      /* Set Gamma adjustment (- polarity): GAMCTRN1 */
    ST7735_CMD_EXT_CTRL = 0xF0U,    /* Extension command control    */
    ST7735_CMD_PWR_CTRL6 = 0xFCU,   /* In partial mode + Idle mode: PWCTR6   */
    ST7735_CMD_VCOM4_LEVEL = 0xFFU, /* VCOM 4 level control */
};
enum ST7735_COLOR_MODE
{
    ST7735_COLOR_MODE_12BIT = 0x03,
    ST7735_COLOR_MODE_16BIT = 0x05,
    ST7735_COLOR_MODE_18BIT = 0x06,
    ST7735_COLOR_MODE_24BIT = 0x07,
};
enum ST7735_DISPLAY_DIRECTION
{
    ST7735_DISPLAY_DIRECTION_NORMAL = 0x00,
    ST7735_DISPLAY_DIRECTION_Y_MIRROR = 0x80,
    ST7735_DISPLAY_DIRECTION_X_MIRROR = 0x40,
    ST7735_DISPLAY_DIRECTION_XY_MIRROR = 0xC0,
    ST7735_DISPLAY_DIRECTION_XY_EXCHANGE = 0x20,
    ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_Y_MIRROR = 0xA0,
    ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_X_MIRROR = 0x60,
    ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_XY_MIRROR = 0xE0
};
enum ST7735_DISPLAY_COLOR_DIRECTION
{
    ST7735_DISPLAY_COLOR_DIRECTION_RGB = 0x00,
    ST7735_DISPLAY_COLOR_DIRECTION_BGR = 0x08
};

enum ST7735_DISPLAY_REFRESH_ORDER
{
    ST7735_DISPLAY_REFRESH_ORDER_T2B_L2R = 0x00,
    ST7735_DISPLAY_REFRESH_ORDER_B2T_L2R = 0x10,
    ST7735_DISPLAY_REFRESH_ORDER_T2B_R2L = 0x04,
    ST7735_DISPLAY_REFRESH_ORDER_B2T_R2L = 0x14
};

class ST7735 : public ST77xx
{
  public:
    ST7735(CommandSpi &cmdSpi, EventGroup &eventGroup) : ST77xx(cmdSpi, eventGroup){};
    Result init();

    Result reset();
    Result inversion(bool on);
    Result sleep(bool on);

    Result display_window_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    Result cursor_set(uint16_t x, uint16_t y);

    Result display(bool on);
    Result pixel_draw(uint16_t x, uint16_t y, uint16_t color);
    Result hline_draw(uint16_t x1, uint16_t y, uint16_t x2, uint16_t *data);
    Result vline_draw(uint16_t x, uint16_t y1, uint16_t y2, uint16_t *data);

    Result rect_draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *data);
    Result rect_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    Result bitmap_draw(uint32_t x, uint32_t y, uint8_t *pBmp);

    Result id_read(uint32_t *id);
};
} // namespace wibot::device

#endif // __WWDEVICE_ST7735_HPP__
