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
            values[i] = 0;
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

    void* parameters[45] = {0};
    Types types[45] = {0};
    int paramCount = 0;

    if (strstr(values[0], "RMC")){
        void* tmpParameters[] = {&gpsData->UTCTime, &gpsData->dataStatus,
                                 &gpsData->latitude, &gpsData->NS,
                                 &gpsData->longitude, &gpsData->EW,
                                 &gpsData->speedKnots, &gpsData->courseMagnetic,
                                 &gpsData->date, NULL, NULL, &gpsData->mode};

        Types tmpTypes[] = {Edouble, Echar,
                            Edouble, Echar,
                            Edouble, Echar,
                            Edouble, Edouble,
                            Eint, Enull, Enull, Echar};

        paramCount = sizeof(tmpParameters)/sizeof(tmpParameters[0]);
        memcpy(parameters, tmpParameters, paramCount*sizeof(void*));
        memcpy(types, tmpTypes, paramCount*sizeof(Types));
    }

    else if(strstr(values[0], "GSV")){
        int msgNum = strtol(values[2], &errptr, 10);


        void* tmpParameters[] = {&gpsData->satellitesUsed, &gpsData->satellitesUsed, // skip first 2 values
                                 &gpsData->satellitesUsed,
                                 &gpsData->satelliteIDs[(msgNum-1)*4], &gpsData->elevations[(msgNum-1)*4],
                                 &gpsData->azimuths[(msgNum-1)*4], &gpsData->SNR[(msgNum-1)*4],
                                 &gpsData->satelliteIDs[(msgNum-1)*4+1], &gpsData->elevations[(msgNum-1)*4+1],
                                 &gpsData->azimuths[(msgNum-1)*4+1], &gpsData->SNR[(msgNum-1)*4+1],
                                 &gpsData->satelliteIDs[(msgNum-1)*4+2], &gpsData->elevations[(msgNum-1)*4+2],
                                 &gpsData->azimuths[(msgNum-1)*4+2], &gpsData->SNR[(msgNum-1)*4+2],
                                 &gpsData->satelliteIDs[(msgNum-1)*4+3], &gpsData->elevations[(msgNum-1)*4+3],
                                 &gpsData->azimuths[(msgNum-1)*4+3], &gpsData->SNR[(msgNum-1)*4+3]};

        Types tmpTypes[] = {Eint, Eint,
                            Eint,
                            Eint, Eint, Eint, Eint,
                            Eint, Eint, Eint, Eint,
                            Eint, Eint, Eint, Eint,
                            Eint, Eint, Eint, Eint};

        paramCount = sizeof(tmpParameters)/sizeof(tmpParameters[0]);
        memcpy(parameters, tmpParameters, paramCount*sizeof(void*));
        memcpy(types, tmpTypes, paramCount*sizeof(Types));
    }

    else if(strstr(values[0], "GGA")){
        void* tmpParameters[] = {&gpsData->UTCTime,
                                 &gpsData->latitude, &gpsData->NS,
                                 &gpsData->longitude, &gpsData->EW,
                                 &gpsData->positionFixIndicator, &gpsData->satellitesUsed,
                                 &gpsData->HDOP, &gpsData->MSLAltitude, &gpsData->geoidSeparation,
                                 &gpsData->DGPSStationID};

        Types tmpTypes[] = {Edouble,
                            Edouble, Echar,
                            Edouble, Echar,
                            Eint, Eint,
                            Edouble, Edouble, Edouble,
                            Eint};

        paramCount = sizeof(tmpParameters)/sizeof(tmpParameters[0]);
        memcpy(parameters, tmpParameters, paramCount*sizeof(void*));
        memcpy(types, tmpTypes, paramCount*sizeof(Types));

    }

    else if(strstr(values[0], "GSA")){
        void* tmpParameters[] = {&gpsData->mode, &gpsData->positionFixIndicator,
                                 &gpsData->satelliteIDs[0], &gpsData->satelliteIDs[1],
                                 &gpsData->satelliteIDs[2], &gpsData->satelliteIDs[3],
                                 &gpsData->satelliteIDs[4], &gpsData->satelliteIDs[5],
                                 &gpsData->satelliteIDs[6], &gpsData->satelliteIDs[7],
                                 &gpsData->satelliteIDs[8], &gpsData->satelliteIDs[9],
                                 &gpsData->satelliteIDs[10], &gpsData->satelliteIDs[11],
                                 &gpsData->PDOP, &gpsData->HDOP, &gpsData->VDOP};

        Types tmpTypes[] = {Echar, Eint,
                            Eint, Eint, Eint, Eint, Eint, Eint,
                            Eint, Eint, Eint, Eint, Eint, Eint,
                            Edouble, Edouble, Edouble};

        paramCount = sizeof(tmpParameters)/sizeof(tmpParameters[0]);
        memcpy(parameters, tmpParameters, paramCount*sizeof(void*));
        memcpy(types, tmpTypes, paramCount*sizeof(Types));

    }

    else if(strstr(values[0], "VTG")){
        void* tmpParameters[] = {&gpsData->courseTrue, &gpsData->mode,
                                 &gpsData->courseMagnetic, &gpsData->mode,
                                 &gpsData->speedKnots, &gpsData->speedKpH,
                                 &gpsData->mode};

        Types tmpTypes[] = {Edouble, Echar,
                            Edouble, Echar,
                            Edouble, Edouble,
                            Echar};

        paramCount = sizeof(tmpParameters)/sizeof(tmpParameters[0]);
        memcpy(parameters, tmpParameters, paramCount*sizeof(void*));
        memcpy(types, tmpTypes, paramCount*sizeof(Types));

    }
    else if(strstr(values[0], "ZDA")){
        void* tmpParameters[] = {&gpsData->UTCTime,
                                 &gpsData->UTCDay, &gpsData->UTCMonth, &gpsData->UTCYear,
                                 &gpsData->localHours, &gpsData->localMinutes};

        Types tmpTypes[] = {Edouble,
                            Eint, Eint, Eint,
                            Eint, Eint};

        paramCount = sizeof(tmpParameters)/sizeof(tmpParameters[0]);
        memcpy(parameters, tmpParameters, paramCount*sizeof(void*));
        memcpy(types, tmpTypes, paramCount*sizeof(Types));

    }
    
    if(paramCount){
        i = 0;
        for(; i < paramCount; ++i){
            switch (types[i]) {
                case Eint: {
                    int tmp = strtol(values[i+1], &errptr, 10);
                    memcpy(parameters[i],&tmp, sizeof(int));
                    break;
                }

                case Edouble: {
                    double tmp = atof(values[i+1]);
                    memcpy(parameters[i], &tmp, sizeof(double));
                    break;
                }

                case Echar: {
                    char tmp = values[i+1][0];
                    memcpy(parameters[i], &tmp, 1);
                    break;
                }

                default:
                    break;
            }

        }
    } 
}

void NEOPrintData(NEOData* gpsData){
    printf("Lat: %.3f %c\nLon: %.3f %c\ndate: %i %.3f\n"
           "Course: %.3f\nSpeed: %.3f KpH\n"
           "dataStatus: %c\n",
           gpsData->latitude, gpsData->NS,
           gpsData->longitude, gpsData->EW,
           gpsData->date, gpsData->UTCTime,
           gpsData->courseTrue, gpsData->speedKpH,
           gpsData->dataStatus);
}

void NEOMessageEnable(int msgClass, int msgId, int enable){
    uint8_t msg[] = {0x06, 0x01, 0x03, 0x00, msgClass, msgId, enable};
    NEOSendMessage(msg, 7);
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

    NEOWrite(finalMsg, msgLen);
    free(finalMsg);
    finalMsg = NULL;
}
