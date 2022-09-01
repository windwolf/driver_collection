#include "w25qxx_demo.hpp"
#include "soc.hpp"

#include "accessor/block.hpp"
#include "accessor/command.hpp"
#include "device/w25qxx.hpp"

#define LOG_MODULE "w25qxx_demo"
#include "log.h"

namespace ww::device::demo
{

using namespace ww::peripheral;
using namespace ww::device;
using namespace ww::accessor;
using namespace ww::os;

#define W25QXX_BUFFER_SIZE W25QXX_BLOCK_SIZE

SPI_HandleTypeDef hspi1;
uint32_t w25qxx_1_id;
Pin csPin(*GPIOD, GPIO_PIN_6);
SpiWithPins spi1Dev(hspi1, &csPin, nullptr, nullptr);
uint8_t w25qxx_1_buf[W25QXX_BUFFER_SIZE];
EventGroup eg("");
WaitHandler wh(eg, 0x01, 0x02);
W25QXX w25qxx_1(spi1Dev, 500);
uint8_t data1_buf[500];

static void run()
{
    w25qxx_1.reset();
    uint32_t mdId, jedecId;
    w25qxx_1.id_read(mdId, jedecId);
    LOG_D("W25QXX-1: mdId=%lx jedecId=%lx", mdId, jedecId);

    w25qxx_1_id = 0x11223344;
    LOG_D("W25QXX-1: w=%lx", w25qxx_1_id);
    wh.reset();
    uint32_t scope = wh.scope_begin();
    w25qxx_1.media_write((uint8_t *)&w25qxx_1_id, 0x0000, 4, wh);
    wh.wait(scope, TIMEOUT_FOREVER);

    wh.reset();
    w25qxx_1.media_read(data1_buf, 0x0000, 256, wh);
    wh.wait(scope, TIMEOUT_FOREVER);

    LOG_D("W25QXX-1: r=%lx", *((uint32_t *)data1_buf));
    while (1)
    {
        Thread::sleep(10);
    }
};

void w25qxx_demo()
{
    spi1Dev.pinconfig_get().csPinHighIsDisable = true;
    while (1)
    {
        run();
    }
};
} // namespace ww::device::demo
