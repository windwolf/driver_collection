#include "st7735.hpp"

namespace wibot::device {
#define ST7735_INTERNAL_BUFFER_SIZE (16)
using namespace wibot::accessor;
using namespace wibot::peripheral;

Result ST7735::init() {
    _pvGamma[0]  = 0x02U;
    _pvGamma[1]  = 0x1CU;
    _pvGamma[2]  = 0x07U;
    _pvGamma[3]  = 0x12U;
    _pvGamma[4]  = 0x37U;
    _pvGamma[5]  = 0x32U;
    _pvGamma[6]  = 0x29U;
    _pvGamma[7]  = 0x2DU;
    _pvGamma[8]  = 0x29U;
    _pvGamma[9]  = 0x25U;
    _pvGamma[10] = 0x2BU;
    _pvGamma[11] = 0x39U;
    _pvGamma[12] = 0x00U;
    _pvGamma[13] = 0x01U;
    _pvGamma[14] = 0x03U;
    _pvGamma[15] = 0x10U;

    _nvGamma[0]  = 0x03U;
    _nvGamma[1]  = 0x1DU;
    _nvGamma[2]  = 0x07U;
    _nvGamma[3]  = 0x06U;
    _nvGamma[4]  = 0x2EU;
    _nvGamma[5]  = 0x2CU;
    _nvGamma[6]  = 0x29U;
    _nvGamma[7]  = 0x2DU;
    _nvGamma[8]  = 0x2EU;
    _nvGamma[9]  = 0x2EU;
    _nvGamma[10] = 0x37U;
    _nvGamma[11] = 0x3FU;
    _nvGamma[12] = 0x00U;
    _nvGamma[13] = 0x00U;
    _nvGamma[14] = 0x02U;
    _nvGamma[15] = 0x10U;
    return Result::OK;
};
Result ST7735::reset() {
    ST77xx::_command(ST7735_CMD_SOFTWARE_RESET);
    Thread::sleep(120);

    _command(ST7735_CMD_SOFTWARE_RESET);
    Thread::sleep(120);

    _command(ST7735_CMD_SLEEP_OUT);

    static const uint8_t frctl1[3] = {0x01U, 0x2CU, 0x2DU};
    _command_data(ST7735_CMD_FRAME_RATE_CTRL1, (uint8_t *)frctl1, 3, DataWidth::Bit8, true);

    static const uint8_t frctl2[3] = {0x01U, 0x2CU, 0x2DU};
    _command_data(ST7735_CMD_FRAME_RATE_CTRL2, (uint8_t *)frctl2, 3, DataWidth::Bit8, true);

    static const uint8_t frctl3[6] = {0x01U, 0x2CU, 0x2DU, 0x01U, 0x2CU, 0x2DU};
    _command_data(ST7735_CMD_FRAME_RATE_CTRL3, (uint8_t *)frctl3, 6, DataWidth::Bit8, true);

    static const uint8_t fictl[1] = {0x07U};
    _command_data(ST7735_CMD_FRAME_INVERSION_CTRL, (uint8_t *)fictl, 1, DataWidth::Bit8, true);

    static const uint8_t pctl1[3] = {0xA2U, 0x02U, 0x84U};
    _command_data(ST7735_CMD_PWR_CTRL1, (uint8_t *)pctl1, 3, DataWidth::Bit8, true);

    static const uint8_t pctl2[1] = {0xC5U};
    _command_data(ST7735_CMD_PWR_CTRL2, (uint8_t *)pctl2, 1, DataWidth::Bit8, true);

    static const uint8_t pctl3[2] = {0x0AU, 0x00U};
    _command_data(ST7735_CMD_PWR_CTRL3, (uint8_t *)pctl3, 2, DataWidth::Bit8, true);

    static const uint8_t pctl4[2] = {0x8AU, 0x2AU};
    _command_data(ST7735_CMD_PWR_CTRL4, (uint8_t *)pctl4, 2, DataWidth::Bit8, true);

    static const uint8_t pctl5[2] = {0x8AU, 0xEEU};
    _command_data(ST7735_CMD_PWR_CTRL5, (uint8_t *)pctl5, 2, DataWidth::Bit8, true);

    static const uint8_t vcomctl1[1] = {0x0EU};
    _command_data(ST7735_CMD_VCOMH_VCOML_CTRL1, (uint8_t *)vcomctl1, 1, DataWidth::Bit8, true);

    _command(ST7735_CMD_DISPLAY_INVERSION_ON);

    _command_data(ST7735_CMD_INTERFACE_PIXEL_FORMAT, &config.colorMode, 1, DataWidth::Bit8, true);

    _command_data(ST7735_CMD_POSITIVE_VOLTAGE_GAMMA_CONTROL, _pvGamma, 16, DataWidth::Bit8, true);

    _command_data(ST7735_CMD_NAGATIVE_VALTAGE_GAMMA_CONTROL, _nvGamma, 16, DataWidth::Bit8, true);

    _command(ST7735_CMD_NORMAL_DISPLAY_MODE_ON);

    _command(ST7735_CMD_DISPLAY_ON);

    _command_data(ST7735_CMD_MEMORY_DATA_ACCESS_CONTROL, &config.orientation, 1, DataWidth::Bit8,
                  true);

    return Result::OK;
};
Result ST7735::inversion(bool on) {
    _command((on ? ST7735_CMD_DISPLAY_INVERSION_ON : ST7735_CMD_DISPLAY_INVERSION_OFF));

    return Result::OK;
};
Result ST7735::sleep(bool on) {
    _command((on ? ST7735_CMD_SLEEP_IN : ST7735_CMD_SLEEP_OUT));

    return Result::OK;
};

Result ST7735::display_window_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    x1 += config.xOffset;
    x2 += config.xOffset;
    y1 += config.yOffset;
    y2 += config.yOffset;

    uint16_t x[2] = {x1, x2};
    _command_data(ST7735_CMD_COLUMN_ADDRESS_SET, x, 2, DataWidth::Bit16, true);

    uint16_t y[2] = {y1, y2};
    _command_data(ST7735_CMD_ROW_ADDRESS_SET, y, 2, DataWidth::Bit16, true);
    return Result::OK;
};
Result ST7735::cursor_set(uint16_t x, uint16_t y) {
    x += config.xOffset;
    _command_data(ST7735_CMD_COLUMN_ADDRESS_SET, &x, 1, DataWidth::Bit16, true);
    y += config.yOffset;
    _command_data(ST7735_CMD_ROW_ADDRESS_SET, &y, 1, DataWidth::Bit16, true);

    return Result::OK;
};

Result ST7735::display(bool on) {
    _command((on ? ST7735_CMD_DISPLAY_ON : ST7735_CMD_DISPLAY_OFF));

    return Result::OK;
};
Result ST7735::pixel_draw(uint16_t x, uint16_t y, uint16_t color) {
    Result ret = Result::OK;

    if ((x >= config.width) || (y >= config.height)) {
        return Result::InvalidParameter;
    }
    ret = cursor_set(x, y);

    if (ret != Result::OK) {
        return ret;
    }

    _command_data(ST7735_CMD_MEMORY_WRITE, &color, 2, DataWidth::Bit16, true);

    return Result::OK;
};
Result ST7735::hline_draw(uint16_t x1, uint16_t y, uint16_t x2, uint16_t *data) {
    Result ret = Result::OK;

    if ((x1 > config.width) || (x2 > config.width)) {
        return Result::InvalidParameter;
    }

    ret = cursor_set(x1, y);

    if (ret != Result::OK) {
        return ret;
    }

    _command_data(ST7735_CMD_MEMORY_WRITE, data, x2 - x1 + 1, DataWidth::Bit16, true);

    return ret;
};
Result ST7735::vline_draw(uint16_t x, uint16_t y1, uint16_t y2, uint16_t *data) {
    Result ret = Result::OK;

    if ((y1 > config.height) || (y2 > config.height)) {
        return Result::InvalidParameter;
    }

    ret = display_window_set(x, y1, x, y2);

    if (ret != Result::OK) {
        return ret;
    }

    _command_data(ST7735_CMD_MEMORY_WRITE, data, y2 - y1 + 1, DataWidth::Bit16, true);

    return ret;
};

Result ST7735::rect_draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *data) {
    Result ret = Result::OK;

    if (x1 > config.width || x2 > config.width) {
        return Result::InvalidParameter;
    }
    if (y1 > config.height || y2 > config.height) {
        return Result::InvalidParameter;
    }

    ret = display_window_set(x1, y1, x2, y2);

    if (ret != Result::OK) {
        return ret;
    }

    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    _command_data(ST7735_CMD_MEMORY_WRITE, data, size, DataWidth::Bit16, true);
    // st77xx_command_write_8(ST7735_CMD_MEMORY_WRITE, buf, size * 2);
    return ret;
};
Result ST7735::rect_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    Result ret = Result::OK;

    if (x1 > config.width || x2 > config.width) {
        return Result::InvalidParameter;
    }
    if (y1 > config.height || y2 > config.height) {
        return Result::InvalidParameter;
    }

    ret = display_window_set(x1, y1, x2, y2);

    if (ret != Result::OK) {
        return ret;
    }

    uint16_t buf[ST7735_INTERNAL_BUFFER_SIZE];
    for (uint32_t i = 0; i < ST7735_INTERNAL_BUFFER_SIZE; i++) {
        buf[i] = color;
    }
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);

    for (uint32_t i = 0; i < size / 8; i++) {
        _command_data(ST7735_CMD_MEMORY_WRITE, buf, size * 2, DataWidth::Bit16, true);
    }

    _command_data(ST7735_CMD_MEMORY_WRITE, buf, (size % 8) / 2, DataWidth::Bit16, true);

    return ret;
};
Result ST7735::bitmap_draw(uint32_t x, uint32_t y, uint8_t *pBmp) {
    uint32_t index, size, width, height, y_pos;
    uint8_t *pbmp;

    /* Get bitmap data address offset */
    index = (uint32_t)pBmp[10] + ((uint32_t)pBmp[11] << 8) + ((uint32_t)pBmp[12] << 16) +
            ((uint32_t)pBmp[13] << 24);

    /* Read bitmap width */
    width = (uint32_t)pBmp[18] + ((uint32_t)pBmp[19] << 8) + ((uint32_t)pBmp[20] << 16) +
            ((uint32_t)pBmp[21] << 24);

    /* Read bitmap height */
    height = (uint32_t)pBmp[22] + ((uint32_t)pBmp[23] << 8) + ((uint32_t)pBmp[24] << 16) +
             ((uint32_t)pBmp[25] << 24);

    /* Read bitmap size */
    size = (uint32_t)pBmp[2] + ((uint32_t)pBmp[3] << 8) + ((uint32_t)pBmp[4] << 16) +
           ((uint32_t)pBmp[5] << 24);
    size = size - index;

    pbmp = pBmp + index;

    /* Remap Ypos, st7735 works with inverted X in case of bitmap */
    /* X = 0, cursor is on Top corner */
    y_pos = height - y - height;

    display_window_set(x, y_pos, width, height);

    // uint16_t *buf = (uint16_t *)(buffer);

    _command_data(ST7735_CMD_MEMORY_WRITE, (uint16_t *)pbmp, size / 2, DataWidth::Bit16, true);

    return Result::OK;
};

Result ST7735::id_read(uint32_t *id) {
    uint32_t id_temp = 0;
    uint8_t  rd;
    _command_data(ST7735_CMD_READ_ID1, &rd, 1, DataWidth::Bit8, false);
    id_temp += rd;

    _command_data(ST7735_CMD_READ_ID2, &rd, 1, DataWidth::Bit8, false);
    id_temp <<= 8;
    id_temp += rd;

    _command_data(ST7735_CMD_READ_ID3, &rd, 1, DataWidth::Bit8, false);
    id_temp <<= 8;
    id_temp += rd;

    *id = id_temp;

    return Result::OK;
};
}  // namespace wibot::device
