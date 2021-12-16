#include "windows.h"
#ifndef MAIN_NEO6_M_H
#define MAIN_NEO6_M_H
#include "stdint.h"

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
} NEOData ;

void NEOMessageBuilder(int (*write)(void* buffer, size_t count), char* format, ...);

void NEOSetSerialPort(int (*write)(void* buffer, size_t count),
                      int protocol, // 0 - UBlox proprietary (not implemented), 1 - NMEA
                      int baudrate, // 9600 by default
                      int dataBits, // 8 by default
                      int stopBits, // 1 by default
                      int parity); // 0 - None, 1 - Odd, 2 - Even; 0 by default

void NEONavigationInit(int (*write)(void* buffer, size_t count),
                       int x, int y, int z,
                       int clkOffset,
                       int timeOfWeek, int weekNo,
                       int channelCount,
                       int resetCfg);

void NEOSetDNEOPort(int (*write)(void* buffer, size_t count),
                    int baudrate,
                    int dataBits,
                    int stopBits,
                    int parity);

void NEOMessageQuery(int (*write)(void* buffer, size_t count),
                     int msg,
                     int checksumEnable);

void NEOMessageRate(int (*write)(void* buffer, size_t count),
                    int msg,
                    int rate,
                    int checksumEnable);

void NEOLLANavigationInit(int (*write)(void* buffer, size_t count),
                          double latitude, double longitude, double altitude,
                          int clkOffset,
                          int timeOfWeek, int weekNo,
                          int channelCount,
                          int resetCfg);

void NEODebugMode(int (*write)(void* buffer, size_t count),
                  int mode);

void NEOSelectDatum(int (*write)(void* buffer, size_t count),
                    int datum);

void NEOGetData(char* data, NEOData* gpsData);

void NEOPrintData(NEOData* gpsData);

void NEOSendMessage(int (*write)(void* buffer, size_t count), uint8_t* msg, size_t length);


#endif //MAIN_NEO6_M_H