#include "NEO6_M.h"

void NEOSetWriteCallback(int (*write)(uint8_t *buffer, int length)){
    NEOWrite = write;
}

void NEOGetData(char* data, NEOData* gpsData){
    char* tmpData = strdup(data);
    int starPos = strchr(tmpData, '*') - tmpData;
    tmpData[starPos] = ',';
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
        gpsData->speedKnots = atof(values[7]);
        gpsData->courseMagnetic = atof(values[8]);
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
        gpsData->UTCTime = atof(values[1]);
        gpsData->latitude = atof(values[2]);
        if(values[3]) gpsData->NS = *values[3];
        gpsData->longitude = atof(values[4]);
        if(values[5]) gpsData->EW = *values[5];
        gpsData->positionFixIndicator = strtol(values[6], &errptr, 10);
        gpsData->satellitesUsed = strtol(values[7], &errptr, 10);
        gpsData->HDOP = atof(values[8]);
        gpsData->MSLAltitude = atof(values[9]);
        gpsData->geoidSeparation = atof(values[10]);
        gpsData->DGPSStationID = strtol(values[11], &errptr, 10);
    }

    else if(strstr(values[0], "GSA")){
        gpsData->PDOP = atof(values[15]);
        gpsData->HDOP = atof(values[16]);
        gpsData->VDOP = atof(values[17]);
    }

    else if(strstr(values[0], "VTG")){
        gpsData->courseTrue = atof(values[1]);
        gpsData->courseMagnetic = atof(values[2]);
        gpsData->speedKnots = atof(values[3]);
        gpsData->speedKpH = atof(values[4]);
        if(values[5])gpsData->mode = *values[5];
    }
    else if(strstr(values[0], "ZDA")){
        gpsData->UTCTime = atof(values[1]);
        gpsData->UTCDay = strtol(values[2], &errptr, 10);
        gpsData->UTCMonth = strtol(values[3], &errptr, 10);
        gpsData->UTCYear = strtol(values[4], &errptr, 10);
        gpsData->localHours = strtol(values[5], &errptr, 10);
        gpsData->localMinutes = strtol(values[7], &errptr, 10);
    }
}


void NEOMessageEnable(int msgClass, int msgId, int enable){
    uint8_t msg[] = {0x06, 0x01, 0x03, 0x00, msgClass, msgId, enable};
    NEOSendMessage(msg, 7);
}

void NEOPrintData(NEOData* gpsData){
    printf("Lat: %.3f %c\nLon: %.3f %c\ndate: %i %.3f\n"
           "Course: %.3f\nSpeed: %.3f KpH",
           gpsData->latitude, gpsData->NS,
           gpsData->longitude, gpsData->EW,
           gpsData->date, gpsData->UTCTime,
           gpsData->courseTrue, gpsData->speedKpH);
}

void NEOSendMessage(uint8_t* msg, int length){
    int decoratorLen = 4; // ub{msg}AB
    int i;
    uint8_t* finalMsg = malloc(length + decoratorLen);
    int msgLen = length + decoratorLen;

    // set ub sync chars
    finalMsg[0] = 0xB5;
    finalMsg[1] = 0x62;

    // set checksum chars to 0
    finalMsg[msgLen-2] = 0x00;
    finalMsg[msgLen-1] = 0x00;

    // calculate checksum
    for(i = 2; i < msgLen-2; ++i){
        finalMsg[i] = msg[i-2];
        finalMsg[msgLen-2] += finalMsg[i];
        finalMsg[msgLen-1] += finalMsg[msgLen-2];
    }
    char d;
    for(i = 0; i < msgLen; ++i){
        d = finalMsg[i];
    }
    NEOWrite(finalMsg, msgLen);
//    free(msg);
    free(finalMsg);
    finalMsg = NULL;
}
