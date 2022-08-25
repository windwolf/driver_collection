#ifndef ___WWDEVICE_GPS_HPP__
#define ___WWDEVICE_GPS_HPP__

#include "stdint.h"
namespace ww::device {

struct GeoLocation2D {
    float longitude; // degrees
    float latitude;  // degrees
};

struct GeoLocation3D {
    float longitude; // degrees
    float latitude;  // degrees
    float altitude;  // m
};

struct Speed2D {
    float heading; // degrees
    float speed;   // m/s
};

struct Speed3D {
    float x; // m/s
    float y; // m/s
    float z; // m/s
};

struct Time {
    uint8_t hour;
    uint8_t minite;
    uint8_t second;
    float millisescond;
};

struct DateTime {
    int16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minite;
    uint8_t second;
    float millisescond;
};
} // namespace ww::device
#endif // ___WWDEVICE_GPS_HPP__