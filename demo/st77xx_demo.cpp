#include "st77xx_demo.hpp"
#include "soc.hpp"
#include "accessor/command.hpp"
#include "device/st7735.hpp"
#include "graph.hpp"

namespace ww::device::demo
{
using namespace ww::accessor;
using namespace ww::device;
using namespace ww::os;
using namespace ww::peripheral;
using namespace ww::graph;

#define ST7735_BUFFER_SIZE 160 * 80 * 2
#define LCD_DATA_SIZE 100

struct ST77xxDemo
{
    ST77xxDemo(Pin dcPin, SpiWithPins spi4pDev)
        : dcPin(dcPin), spi4pDev(spi4pDev), eg(""), st7735_cmd(spi4pDev, 500), wh(eg, 0x01, 0x02),
          st7735(st7735_cmd, wh){};
    Pin &dcPin;
    SpiWithPins &spi4pDev;
    EventGroup eg;
    CommandSpi st7735_cmd;
    WaitHandler wh;
    ST7735 st7735;
    uint16_t lcddata[LCD_DATA_SIZE];
};

static void test05_init(ST77xxDemo &demo)
{
    demo.dcPin.config_get().inverse = false;
    auto &cfg = demo.st7735.config_get();
    cfg.xOffset = 1;
    cfg.yOffset = 26;
    cfg.width = 160;
    cfg.height = 80;
    cfg.colorMode = ST7735_COLOR_MODE_16BIT;
    cfg.orientation = ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_Y_MIRROR |
                      ST7735_DISPLAY_COLOR_DIRECTION_BGR | ST7735_DISPLAY_REFRESH_ORDER_T2B_L2R;
    demo.st7735.init();
    demo.st7735.reset();
    // st77xx_inversion(&st7735, 1);
    for (size_t i = 0; i < LCD_DATA_SIZE; i++)
    {
        demo.lcddata[i] = (0xF800);
    }
}

static void test05(ST77xxDemo &demo)
{
    Color565 color0 = {.value = 0x28A5};
    // Color565 color1 = {.value = 0x001F};
    Color565 color2 = {.value = 0xF800};
    Color565 color3 = {.value = 0x04F1};
    demo.st7735.rect_fill(0, 0, demo.st7735.config_get().width, demo.st7735.config_get().height,
                          color0.value); // inv:1=red; inv:0=yellow
    // st7735.hline_draw(10, 10, 20,
    //                   &color1.value); // inv:1=red+green; inv:0=sky+pink
    demo.st7735.rect_fill(20, 20, 30, 30, color2.value); // inv:1=blue; inv:0=sky
    demo.st7735.rect_fill(40, 20, 50, 50, color3.value); // inv:1=red; inv:0=yellow
}

static void run()
{
    ST77xxDemo *demo;
    float num = 0.1;

    test05_init(*demo);
    /* This thread simply sits in while-forever-sleep loop.  */
    while (1)
    {
        test05(*demo);
        num += 0.15;

        /* Sleep for 1000 ticks.  */
        Thread::sleep(1000 + (uint32_t)(num * 100));

        // printf("thread0 running\n");
        // HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        // HAL_UART_Transmit(&CONVERT_REFERENCE(huart4, UART_HandleTypeDef),
        // memory_area, 10, HAL_MAX_DELAY);

        /* Check status.  */
        // if (status != TX_SUCCESS)
        //     break;
    }
}

void st77xx_demo()
{

    /* Create the main thread.  */
    run();
};
} // namespace ww::device::demo