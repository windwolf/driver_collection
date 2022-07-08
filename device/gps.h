#ifndef ___GPS_H__
#define ___GPS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "shared.h"

    typedef struct GeoLocation2D
    {
        float longitude; // degrees
        float latitude;  // degrees
    } GeoLocation2D;

    typedef struct GeoLocation3D
    {
        float longitude; // degrees
        float latitude;  // degrees
        float altitude;  // m
    } GeoLocation3D;

    typedef struct Speed2D
    {
        float heading; // degrees
        float speed;   // m/s
    } Speed2D;

    typedef struct Speed3D
    {
        float x; // m/s
        float y; // m/s
        float z; // m/s
    } Speed3D;

    typedef struct Date
    {
        int16_t year;
        uint8_t month;
        uint8_t day;

    } Date;

    typedef struct Time
    {
        uint8_t hour;
        uint8_t minite;
        uint8_t second;
        float millisescond;
    } Time;

    typedef struct DateTime
    {
        int16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minite;
        uint8_t second;
        float millisescond;
    } DateTime;



#ifdef __cplusplus
}
#endif
#endif // ___GPS_H__