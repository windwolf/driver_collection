#include "common.hpp"

using namespace Driver::Core;

DeviceBase::DeviceBase(DeviceDriverBase *pDriver)
    : _pDriver(pDriver){

      };

DEVICE_STATUS DeviceBase::Init(void *pConfig)
{
    DEVICE_STATUS rst;
    if (!pConfig)
    {
        return DEVICE_STATUS::ARGUMENT_ERROR;
    }
    this->_pConfig = pConfig;
    rst = this->_pDriver->Init(this->_pConfig, &(this->_pInstance));
    if (rst < OK)
    {
        return rst;
    }
    return this->_pDriver->Config(this->_pConfig, &(this->_pInstance));
};

DEVICE_STATUS DeviceBase::DeInit()
{
    if (this->_pInstance)
    {
        this->_pDriver->DeInit(this->_pInstance);
        this->_pInstance = static_cast<void *>(0);
    }
    return DEVICE_STATUS::OK;
};

DEVICE_STATUS DeviceBase::ReConfig(void *pConfig)
{
    if (!pConfig)
    {
        return DEVICE_STATUS::ARGUMENT_ERROR;
    }
    return this->_pDriver->Config(this->_pConfig, &(this->_pInstance));
}
