// //
// // Created by windwolf on 2021/4/10.
// //

// #include "spi.hpp"

// using namespace Windwolf::Driver;
// using namespace Windwolf::Driver::Common;

// SPI_Device::SPI_Device(SPI_DeviceDriver *pDriver)
//     : DeviceBase(pDriver){

//       };

// DEVICE_STATUS SPI_Device::Init(SPI_Config *pConfig)
// {
//     return DeviceBase::Init(pConfig);
// };

// DEVICE_STATUS SPI_Device::ReConfig(SPI_Config *pConfig)
// {
//     return DeviceBase::ReConfig(pConfig);
// }

// DEVICE_STATUS SPI_Device::StartWrite(uint8_t *writeData, uint32_t dataSize, WaitHandle *waitHandle)
// {
//     auto driver = static_cast<SPI_DeviceDriver *>(_pDriver);
//     waitHandle->Start();
//     return driver->WriteIt(writeData, dataSize); //TODO:
// }

// DEVICE_STATUS SPI_Device::Write(uint8_t *writeData, uint32_t dataSize)
// {
//     auto driver = static_cast<SPI_DeviceDriver *>(_pDriver);

//     return driver->WriteIt(writeData, dataSize); //TODO:
// }

// DEVICE_STATUS SPI_Device::StartWriteRead(uint8_t *writeData, uint32_t dataSize, uint8_t *readBuffer, uint32_t bufferSize,
//                                          WaitHandle *waitHandle)
// {
//     auto driver = static_cast<SPI_DeviceDriver *>(_pDriver);
//     if (dataSize <= 2 && bufferSize <= 2)
//     {
//         waitHandle->Start();
//         return driver->WriteReadIt(writeData, dataSize, readBuffer, bufferSize); //TODO:
//     }
//     else
//     {
//         waitHandle->Start();
//         return driver->WriteReadDma(writeData, dataSize, readBuffer, bufferSize); //TODO:
//     }
// }

// DEVICE_STATUS SPI_Device::WriteRead(uint8_t *writeData, uint32_t dataSize, uint8_t *readBuffer, uint32_t bufferSize)
// {
//     auto driver = static_cast<SPI_DeviceDriver *>(_pDriver);
//     if (dataSize <= 2 && bufferSize <= 2)
//     {
//         return driver->WriteReadIt(writeData, dataSize, readBuffer, bufferSize); //TODO:
//     }
//     else
//     {
//         return driver->WriteReadDma(writeData, dataSize, readBuffer, bufferSize); //TODO:
//     }
// }