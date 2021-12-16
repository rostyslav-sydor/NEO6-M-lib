#include "NEO6_M.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"

void NEOGetData(char* data, NEOData* gpsData){
    char* tmpData = strdup(data);
    int starPos = strchr(tmpData, '*') - tmpData;
    tmpData[starPos] = ',';
    printf("%s\n\n",tmpData);
    char *values[30];
    int i = 0;
    char *value = malloc(0);
    long long match, prevMatch = 0;
    while(1){
        match = strchr(tmpData+prevMatch+1, ',') - tmpData;

        if(!(match - prevMatch - 1)) {
            prevMatch += 1;
            values[i] = NULL;
            i++;
            continue;
        }

        if(!(match + tmpData)) break;
        value = realloc(value, 15);
        memcpy(value, tmpData+prevMatch+1, match-prevMatch-1);
        value[match-prevMatch-1] = '\0';
        values[i] = strdup(value);
        prevMatch = match;
        i++;
    }
    free(tmpData);
    free(value);
    char* errptr;

    if (strstr(values[0], "RMC")){
        gpsData->UTCTime = atof(values[1]);
        if(values[2]) gpsData->dataStatus = *values[2];
        gpsData->latitude = atof(values[3]);
        if(values[4]) gpsData->NS = *values[4];
        gpsData->longitude = atof(values[5]);
        if(values[6]) gpsData->EW = *values[6];
        gpsData->date = strtol(values[9], &errptr, 10);
        if(values[10]) gpsData->mode = *values[10];
    }

    else if(strstr(values[0], "GSV")){
        gpsData->satellitesUsed = strtol(values[3], &errptr, 10);

        int msgIndex = (strtol(values[2], &errptr, 10) - 1) * 4;
        int j;
        for (j = 0; j< 4; j++){
            gpsData->satelliteIDs[msgIndex+j] = strtol(values[4 + j*4], &errptr, 10);
            gpsData->elevations[msgIndex+j] = strtol(values[5 + j*4], &errptr, 10);
            gpsData->azimuths[msgIndex+j] = strtol(values[6 + j*4], &errptr, 10);
            gpsData->SNR[msgIndex + j] = strtol(values[7 + j * 4], &errptr, 10);
        }
    }

    else if(strstr(values[0], "GGA")){
        gpsData->positionFixIndicator = strtol(values[6], &errptr, 10);
        gpsData->MSLAltitude = atof(values[9]);
        gpsData->geoidSeparation = atof(values[11]);
        gpsData->ageOfDiffCorr = strtol(values[6], &errptr, 10);
    }

    else if(strstr(values[0], "GSA")){
        gpsData->PDOP = atof(values[15]);
        gpsData->HDOP = atof(values[16]);
        gpsData->VDOP = atof(values[17]);
    }

    else if(strstr(values[0], "VTG")){
        gpsData->courseTrue = atof(values[1]);
        gpsData->courseMagnetic = atof(values[3]);
        gpsData->speedKnots = atof(values[5]);
        gpsData->speedKpH = atof(values[7]);
    }
}

void NEOMessageBuilder(int (*write)(void* buffer, size_t count), char* format, ...){
    int maxLen = 80;
    char* msg = malloc(maxLen);
    va_list args;
    va_start(args, format);
    int length = vsnprintf(msg, maxLen, format, args);
    NEOSendMessage(write, msg, (size_t) length);
    free(msg);
}

void NEOSetSerialPort(int (*write)(void* buffer, size_t count), int protocol, int baudrate,
                      int dataBits, int stopBits, int parity){
    NEOMessageBuilder(write, "PSRF100,%i,%i,%i,%i,%i", 0, baudrate, dataBits, stopBits, parity);

}

void NEONavigationInit(int (*write)(void* buffer, size_t count), int x, int y, int z,
                       int clkOffset, int timeOfWeek, int weekNo, int channelCount, int resetCfg){
    NEOMessageBuilder(write, "PSRF101,%i,%i,%i,%i,%i,%i,%i,%i", x, y, z, clkOffset, timeOfWeek, weekNo, channelCount, resetCfg);
}

void NEOSetDNEOPort(int (*write)(void* buffer, size_t count),int baudrate,
                    int dataBits, int stopBits, int parity){
    NEOMessageBuilder(write, "PSRF102,%i,%i,%i,%i", baudrate, dataBits, stopBits, parity);
}

void NEOMessageQuery(int (*write)(void* buffer, size_t count), int msg, int checksumEnable){
    NEOMessageBuilder(write, "PSRF103,%02i,01,00,%02i", msg, checksumEnable);
}

void NEOMessageRate(int (*write)(void* buffer, size_t count), int msg, int rate, int checksumEnable){
    NEOMessageBuilder(write, "PSRF103,%02i,00,%02i,1", msg, checksumEnable);
}

void NEOLLANavigationInit(int (*write)(void* buffer, size_t count), double latitude, double longitude, double altitude,
                          int clkOffset, int timeOfWeek, int weekNo, int channelCount, int resetCfg){
    NEOMessageBuilder(write, "PSRF104,%f,%f,%f,%i,%i,%i,%i,%i", latitude, longitude, altitude,
                      clkOffset, timeOfWeek, weekNo, channelCount, resetCfg);
}

void NEODebugMode(int (*write)(void* buffer, size_t count), int mode){
    NEOMessageBuilder(write, "PSRF105,%i", mode);
}

void NEOSelectDatum(int (*write)(void* buffer, size_t count), int datum){
    NEOMessageBuilder(write, "PSRF106,%i", datum);
}

void NEOPrintData(NEOData* gpsData){
    printf("Lat: %.3f %c\nLon: %.3f %c\ndate: %i %.3f\n"
           "Course: %.3f\nSpeed: %.3f KpH",
           gpsData->latitude, gpsData->NS,
           gpsData->longitude, gpsData->EW,
           gpsData->date, gpsData->UTCTime,
           gpsData->courseTrue, gpsData->speedKpH);
}

void NEOSendMessage(int (*write)(void* buffer, size_t count), uint8_t* msg, size_t length){
    int decoratorLen = 4; // ub{msg}AB
    int i;
    char* finalMsg;
    int msgLen = sprintf(finalMsg, "µb%s00", msg);

    for(i = 2; i < msgLen - 4; ++i){
        finalMsg[msgLen-2] += finalMsg[i];
        finalMsg[msgLen-1] += finalMsg[msgLen-2];
    }
    write(msg, msgLen);
    free(finalMsg);
    finalMsg = NULL;
}

