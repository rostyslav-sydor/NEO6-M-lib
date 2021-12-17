#ifndef MAIN_NEO6_M_H
#define MAIN_NEO6_M_H
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

int (*NEOWrite)(uint8_t *buffer, int length);

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
    int DGPSStationID;
} NEOData ;


void NEOSetWriteCallback(int(*write)(uint8_t *buffer, int length));


void NEOMessageEnable(int msgClass,
                    int msgID,
                    int enable);

void NEOGetData(char* data, NEOData* gpsData);

void NEOPrintData(NEOData* gpsData);

void NEOSendMessage(uint8_t* msg, int length);


#endif //MAIN_NEO6_M_H