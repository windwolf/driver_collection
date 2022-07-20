#include "w25qxx_demo.h"
#include "bsp.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32h7xx_hal.h"

#include "common/command.h"
#include "common/device.h"
#include "common/block.h"
#include "device/w25qxx.h"

#define LOG_MODULE "w25qxx_demo"
#include "log.h"

#define W25QXX_BUFFER_SIZE W25QXX_BLOCK_SIZE

extern SPI_HandleTypeDef hspi1;
static uint32_t w25qxx_1_id;
static PinDevice csPin_1;
static SpiDevice spi1Dev;
static SpiWithPinsDevice spi1pDev;
static CommandSpi w25qxx_1_cmd;
static uint8_t w25qxx_1_buf[W25QXX_BUFFER_SIZE];
static W25QXX w25qxx_1;
static Block block1;

uint8_t data1_buf[500];

void w25qxx_demo()
{
    pin_device_create(&csPin_1, GPIOD, GPIO_PIN_6, PIN_DEVICE_STATUS_INVERSE_INVERSE);
    spi_device_create(&spi1Dev, &hspi1, 4);
    spi_with_pins_device_create(&spi1pDev, &spi1Dev, &csPin_1, NULL, NULL);
    command_spi_create(&w25qxx_1_cmd, &spi1pDev);
    Buffer buf2 = {.data = w25qxx_1_buf, .size = W25QXX_BUFFER_SIZE};
    w25qxx_create(&w25qxx_1, (Command *)&w25qxx_1_cmd, 0);
    w25qxx_block_create(&w25qxx_1, &block1, buf2);

    while (1)
    {
        run();
    }
};

static void run()
{
    w25qxx_reset(&w25qxx_1);

    w25qxx_id_read(&w25qxx_1);
    LOG_D("W25QXX-1: mdId=%#x jedecId=%#x", w25qxx_1.mdId, w25qxx_1.jedecId);
    w25qxx_status_get(&w25qxx_1);
    LOG_D("W25QXX-1: s1=%u, s2=%u, s3=%u", w25qxx_1.status1, w25qxx_1.status2, w25qxx_1.status3);

    w25qxx_1_id = 0x11223344;
    LOG_D("W25QXX-1: w=%#x", w25qxx_1_id);
    block_write(&block1, (uint8_t *)&w25qxx_1_id, 0x0000, 4);
    block_read(&block1, data1_buf, 0x0000, 256);
    LOG_D("W25QXX-1: r=%#x", *((uint32_t *)data1_buf));
    while (1)
    {
        tx_thread_sleep(10);
    }
}
