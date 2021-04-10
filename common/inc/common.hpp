#ifndef __common_hpp__
#define __common_hpp__

namespace Driver::Core
{

    enum DEVICE_STATUS
    {
        ARGUMENT_ERROR = -2,
        GENERAL_ERROR = -1,
        OK = 0,

    };

    class DeviceDriverBase;

    class DeviceBase
    {
    public:
        DeviceBase(DeviceDriverBase *pDriver);
        DEVICE_STATUS Init(void *pConfig);
        DEVICE_STATUS DeInit();
        DEVICE_STATUS ReConfig(void *pConfig);

    protected:
        DeviceDriverBase *_pDriver;
        void *_pInstance;
        void *_pConfig;
    };

    class DeviceDriverBase
    {
    public:
        virtual DEVICE_STATUS Init(void *pConfig, void **pInstance) = 0;
        virtual DEVICE_STATUS DeInit(void *pInstance) = 0;
        virtual DEVICE_STATUS Config(void *pInstance, void *pConfig) = 0;
    };

} // namespace WWDriver

#endif // __common_hpp__