#ifndef __spi_h__
#define __spi_h__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct SPI_CONFIG_STRUCT
    {

    } SPI_CONFIG;
    typedef struct SPI_INSTANCE_STRUCT
    {

    } SPI_INSTANCE;

    void SPI_Init();

    void SPI_Config();

    void SPI_Read();

    void SPI_Write();

#ifdef __cplusplus
}
#endif

#endif // __spi_h__