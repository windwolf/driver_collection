#ifndef ___IMAGE_H__
#define ___IMAGE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    typedef struct ImageDirection
    {
        uint8_t xMirror : 1;
        uint8_t yMirror : 1;
        uint8_t xyExchange : 1;
    } ImageDirection;

    typedef enum ImagePixFormat
    {
        IMAGE_PIX_FORMAT_INVALID = 0,
        IMAGE_PIX_FORMAT_RGB565,
        IMAGE_PIX_FORMAT_RGB888,
        IMAGE_PIX_FORMAT_JPEG,
        IMAGE_PIX_FORMAT_YUV422,
        IMAGE_PIX_FORMAT_GRAYSCALE,
        IMAGE_PIX_FORMAT_BAYER,
    } ImagePixFormat;

#ifdef __cplusplus
}
#endif

#endif // ___IMAGE_H__