#ifndef MAIN_NEO6_H
#define MAIN_NEO6_H

#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

//int (*NEOWrite)(uint8_t *buffer, int length);

typedef struct {
    double latitude, longitude;
    char NS, EW;
    char dataStatus, mode;
    int date;
    int UTCDay, UTCMonth, UTCYear;
    int localHours, localMinutes;
    double UTCTime;
    int satellitesUsed;
    int satelliteID[12];
    int elevation[12], azimuth[12], SNR[12];
    int positionFixIndicator;
    double HDOP, PDOP, VDOP;
    double MSLAltitude, geoidSeparation;
    double ageOfDiffCorr;
    double courseTrue, courseMagnetic;
    double speedKpH, speedKnots;
    int DGPSStationID;
} NEOData ;

typedef enum {
    Eint, Edouble, Echar, Enull
} Types;

void NEOParseData(const char* data, NEOData* gpsData, int checkChecksum);

void NEOPrintData(NEOData* gpsData);

void NEOSendMessage(const uint8_t* msg, int (NEOWrite)(uint8_t *buffer, int length));

#endif //MAIN_NEO6_H