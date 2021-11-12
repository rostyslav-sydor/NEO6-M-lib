#include "windows.h"
#ifndef MAIN_NEO6_M_H
#define MAIN_NEO6_M_H

#endif //MAIN_NEO6_M_H

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

void hello();
int sample_read(HANDLE hCom, int numMessages);
HANDLE setup();
void finish(HANDLE hCom);
GPSData getData(HANDLE hCom);
//int fakeRead();