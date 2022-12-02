#include "w25qxx_demo.hpp"
#include "soc.hpp"

#include "accessor/block.hpp"
#include "accessor/command.hpp"
#include "device/w25qxx.hpp"

#define LOG_MODULE "w25qxx_demo"
#include "log.h"

namespace wibot::device::demo
{

using namespace wibot::peripheral;
using namespace wibot::device;
using namespace wibot::accessor;
using namespace wibot::os;

#define W25QXX_BUFFER_SIZE W25QXX_BLOCK_SIZE

struct W25QxxDemo
{
    W25QxxDemo(SpiWithPins &spi1Dev) : spi1Dev(spi1Dev), w25qxx_1(spi1Dev, 500){};

    SpiWithPins &spi1Dev;
    uint8_t w25qxx_1_buf[W25QXX_BUFFER_SIZE];

    W25QXX w25qxx_1;
    uint8_t data1_buf[500];
};

static void run(W25QxxDemo &demo)
{
    uint32_t w25qxx_1_id;
    EventGroup eg("");
    WaitHandler wh(eg, 0x01, 0x02);
    demo.w25qxx_1.reset();
    uint32_t mdId, jedecId;
    demo.w25qxx_1.id_read(mdId, jedecId);
    LOG_D("W25QXX-1: mdId=%lx jedecId=%lx", mdId, jedecId);

    w25qxx_1_id = 0x11223344;
    LOG_D("W25QXX-1: w=%lx", w25qxx_1_id);
    wh.reset();
    uint32_t scope = wh.scope_begin();
    demo.w25qxx_1.media_write((uint8_t *)&w25qxx_1_id, 0x0000, 4, wh);
    wh.wait(scope, TIMEOUT_FOREVER);

    wh.reset();
    demo.w25qxx_1.media_read(demo.data1_buf, 0x0000, 256, wh);
    wh.wait(scope, TIMEOUT_FOREVER);

    LOG_D("W25QXX-1: r=%lx", *((uint32_t *)demo.data1_buf));
    while (1)
    {
        Thread::sleep(10);
    }
};

void w25qxx_demo()
{
    W25QxxDemo *demo;
    demo->spi1Dev.pinconfig_get().csPinHighIsDisable = true;
    while (1)
    {
        run(*demo);
    }
};
} // namespace wibot::device::demo
