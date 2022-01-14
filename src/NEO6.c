#include "NEO6.h"

void NEOParseData(const char* data, NEOData* gpsData){

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
                                 &gpsData->satelliteID[(msgNum-1)*4], &gpsData->elevation[(msgNum-1)*4],
                                 &gpsData->azimuth[(msgNum-1)*4], &gpsData->SNR[(msgNum-1)*4],
                                 &gpsData->satelliteID[(msgNum-1)*4+1], &gpsData->elevation[(msgNum-1)*4+1],
                                 &gpsData->azimuth[(msgNum-1)*4+1], &gpsData->SNR[(msgNum-1)*4+1],
                                 &gpsData->satelliteID[(msgNum-1)*4+2], &gpsData->elevation[(msgNum-1)*4+2],
                                 &gpsData->azimuth[(msgNum-1)*4+2], &gpsData->SNR[(msgNum-1)*4+2],
                                 &gpsData->satelliteID[(msgNum-1)*4+3], &gpsData->elevation[(msgNum-1)*4+3],
                                 &gpsData->azimuth[(msgNum-1)*4+3], &gpsData->SNR[(msgNum-1)*4+3]};

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
                                 &gpsData->satelliteID[0], &gpsData->satelliteID[1],
                                 &gpsData->satelliteID[2], &gpsData->satelliteID[3],
                                 &gpsData->satelliteID[4], &gpsData->satelliteID[5],
                                 &gpsData->satelliteID[6], &gpsData->satelliteID[7],
                                 &gpsData->satelliteID[8], &gpsData->satelliteID[9],
                                 &gpsData->satelliteID[10], &gpsData->satelliteID[11],
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

    i = 0;
    for(; i < paramCount; ++i){
        if(values[i+1]) {
            switch (types[i]) {
                case Eint: {
                    int tmp = strtol(values[i + 1], &errptr, 10);
                    memcpy(parameters[i], &tmp, sizeof(int));
                    break;
                }

                case Edouble: {
                    double tmp = atof(values[i + 1]);
                    memcpy(parameters[i], &tmp, sizeof(double));
                    break;
                }

                case Echar: {
                    char tmp = values[i + 1][0];
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

void NEOSendMessage(const uint8_t* msg, int (NEOWrite)(uint8_t *buffer, int length)){
    int decoratorLen = 4; // ub{msg}AB
    int i;
    int length = strlen(msg);
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
