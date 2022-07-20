#include "st77xx_demo.h"

#include "common/command.h"
#include "common/device.h"
#include "device/st7735.h"
#include "bsp.h"

extern SPI_HandleTypeDef hspi4;
static PinDevice csPin;
static PinDevice dcPin;
static SpiDevice spi4Dev;
static SpiWithPinsDevice spi4pDev;
static CommandSpi st7735_cmd;
#define ST7735_BUFFER_SIZE 160 * 80 * 2
static uint8_t st7735Buffer[ST7735_BUFFER_SIZE];
static ST77XX st7735;

#define LCD_DATA_SIZE 100
static uint16_t lcddata[LCD_DATA_SIZE];

static void test05_init()
{
    st7735.xOffset = 1;
    st7735.yOffset = 26;
    st7735.width = 160;
    st7735.height = 80;
    st7735.colorMode = ST7735_COLOR_MODE_16BIT;
    st7735.orientation = ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_Y_MIRROR | ST7735_DISPLAY_COLOR_DIRECTION_BGR | ST7735_DISPLAY_REFRESH_ORDER_T2B_L2R;
    st7735_reset(&st7735);
    // st77xx_inversion(&st7735, 1);
    for (size_t i = 0; i < LCD_DATA_SIZE; i++)
    {
        lcddata[i] = (0xF800);
    }
}

static void test05()
{
    uint16_t color0 = 0x28A5;
    uint16_t color1 = 0x001F;
    uint16_t color2 = 0xF800;
    uint16_t color3 = 0x04F1;
    // st7735_rect_fill(&st7735, 0, 0, st7735.width, st7735.height, color0); //inv:1=red; inv:0=yellow
    // st7735_hline_draw(&st7735, 10, 10, 20, color1);                       //inv:1=red+green; inv:0=sky+pink
    // st7735_rect_fill(&st7735, 20, 20, 30, 30, color2);                    //inv:1=blue; inv:0=sky
    // st7735_rect_fill(&st7735, 40, 20, 50, 50, color3);                    //inv:1=red; inv:0=yellow
}

static void run()
{
    float num = 0.1;

    test05_init();
    /* This thread simply sits in while-forever-sleep loop.  */
    while (1)
    {
        test05();
        num += 0.15;

        /* Sleep for 1000 ticks.  */
        ww_os_delay(1000 + (uint32_t)(num * 100));

        // printf("thread0 running\n");
        // HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        // HAL_UART_Transmit(&CONVERT_REFERENCE(huart4, UART_HandleTypeDef), memory_area, 10, HAL_MAX_DELAY);

        /* Check status.  */
        // if (status != TX_SUCCESS)
        //     break;
    }
}

void st77xx_demo()
{

    pin_device_create(&dcPin, GPIOE, GPIO_PIN_13, PIN_DEVICE_STATUS_INVERSE_NORMAL);
    spi_device_create(&spi4Dev, &hspi4, 4);
    spi_with_pins_device_create(&spi4pDev, &spi4Dev, NULL, NULL, &dcPin);
    command_spi_create(&st7735_cmd, &spi4pDev);
    Buffer buf1 = {.data = st7735Buffer, .size = ST7735_BUFFER_SIZE};
    st7735_create(&st7735, (Command *)&st7735_cmd);

    /* Create the main thread.  */
    run();
};