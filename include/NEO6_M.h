#include "windows.h"
#ifndef MAIN_NEO6_M_H
#define MAIN_NEO6_M_H

typedef struct {
    double latitude, longitude;
    char NS, EW;
    char dataStatus, mode;
    int date;
    double UTCTime;
    int satellitesUsed;
    int satelliteIDs[12];
    int elevations[12], azimuths[12], SNR[12];
    int positionFixIndicator;
    float HDOP, PDOP, VDOP;
    double MSLAltitude, geoidSeparation;
    double ageOfDiffCorr;
    double courseTrue, courseMagnetic;
    double speedKpH, speedKnots;
} GPSData ;

void GPSMessageBuilder(int (*write)(void* buffer, size_t count), char* format, ...);

void GPSSetSerialPort(int (*write)(void* buffer, size_t count),
                      int protocol, // 0 - UBlox proprietary (not implemented), 1 - NMEA
                      int baudrate, // 9600 by default
                      int dataBits, // 8 by default
                      int stopBits, // 1 by default
                      int parity); // 0 - None, 1 - Odd, 2 - Even; 0 by default

void GPSNavigationInit(int (*write)(void* buffer, size_t count),
                       int x, int y, int z,
                       int clkOffset,
                       int timeOfWeek, int weekNo,
                       int channelCount,
                       int resetCfg);

void GPSSetDGPSPort(int (*write)(void* buffer, size_t count),
                    int baudrate,
                    int dataBits,
                    int stopBits,
                    int parity);

void GPSMessageQuery(int (*write)(void* buffer, size_t count),
                     int msg,
                     int checksumEnable);

void GPSMessageRate(int (*write)(void* buffer, size_t count),
                    int msg,
                    int rate,
                    int checksumEnable);

void GPSLLANavigationInit(int (*write)(void* buffer, size_t count),
                          double latitude, double longitude, double altitude,
                          int clkOffset,
                          int timeOfWeek, int weekNo,
                          int channelCount,
                          int resetCfg);

void GPSDebugMode(int (*write)(void* buffer, size_t count),
                  int mode);

void GPSSelectDatum(int (*write)(void* buffer, size_t count),
                    int datum);

GPSData GPSGetData(int (*read)(void* buffer, size_t count));
char* GPSReadData(int (*read)(void* buffer, size_t count));
void GPSPrintData(GPSData gpsdata);

void GPSSendMessage(int (*write)(void* buffer, size_t count), char* msg, size_t length);
int GPSChecksum(const char* msg, size_t length);

HANDLE winSetup();

#endif //MAIN_NEO6_M_H