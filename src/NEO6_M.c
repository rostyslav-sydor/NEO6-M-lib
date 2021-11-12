#include "NEO6_M.h"
#include <stdio.h>
#include "string.h"

void hello(){
    printf("Hola, el mundo!\n");
}

HANDLE setup(){
    DCB dcb={0};
    HANDLE hCom;
    char *pcCommPort = "COM3";
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=0;

    hCom = CreateFile( pcCommPort,
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ,    // must be opened with exclusive-access
                       NULL, // no security attributes
                       OPEN_EXISTING, // must use OPEN_EXISTING
                       FILE_ATTRIBUTE_NORMAL,    // not overlapped I/O
                       NULL  // hTemplate must be NULL for comm devices
    );
    /***************************************SET*UP*COM*PORT**************************************/
    if (hCom == INVALID_HANDLE_VALUE)
    {
        printf ("CreateFile failed with error %d.\n", GetLastError());
        CloseHandle(hCom);
        return (HANDLE) 1;
    }

    if (!GetCommState(hCom, &dcb))
    {
        printf ("GetCommState failed with error %d.\n", GetLastError());
        CloseHandle(hCom);
        return (HANDLE) 2;
    }
    dcb.BaudRate = 9600;     // set the baud rate
    dcb.ByteSize = 8;             // byte size
    dcb.Parity = NOPARITY;        // no parity bit
    dcb.StopBits = ONESTOPBIT;    // one stop bit

    if (!SetCommState(hCom, &dcb))
    {
        printf ("SetCommState failed. Error: %d.\n", GetLastError());
        CloseHandle(hCom);
        return (HANDLE) 3;
    }
    printf ("Serial port %s successfully configured.\n", pcCommPort);
    return hCom;
}
void finish(HANDLE hCom){
    CloseHandle(hCom);
}

GPSData getData(HANDLE hCom){
    GPSData gpsData;
    char data[] = "$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598, ,*10\n"
                  "$GPVTG,309.62,T, ,M,0.13,N,0.2,K,A*23\n"
                  "$GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M, , , ,0000*18\n"
                  "$GPGSA,A,3,07,02,26,27,09,04,15, , , , , ,1.8,1.0,1.5*33\n"
                  "$GPGSV,2,1,07,07,79,048,42,02,51,062,43,26,36,256,42,27,27,138,42*71\n"
                  "$GPGSV,2,2,07,09,23,313,42,04,19,159,41,15,12,041,42*41\n"
                  "$GPGLL,3723.2475,N,12158.3416,W,161229.487,A,A*41";
    char *tmp, *value;
    char *values[30];
    int data_pointer = 0;
    for (tmp = strtok(data,"\n"); tmp != NULL; tmp = strtok(data+data_pointer, "\n")){
        data_pointer += (int) strlen(tmp) + 1;
        int i = 0;
        values[0] = "hola";
        for (value = strtok(tmp, ",*"); value != NULL; value = strtok(NULL, ",*")){
            values[i] = strdup(value);
            i++;
        }
        char* endptr;
        if (!strcmp(values[0], "$GPRMC")){
            gpsData.UTCTime = strtod(values[1], &endptr);
            gpsData.dataStatus = values[2][0];
            gpsData.latitude = strtod(values[3], &endptr);
            gpsData.NS = values[4][0];
            gpsData.longitude = strtod(values[5], &endptr);
            gpsData.EW = values[6][0];
            gpsData.date = strtol(values[9], &endptr, 10);
            gpsData.mode = values[10][0];
        }
        else if(!strcmp(values[0], "$GPGSV")){
            int satellitesUsed = strtol(values[3], &endptr, 10);
            gpsData.satellitesUsed = satellitesUsed;
//            if (gpsData.SNR == NULL){
//                gpsData.satelliteIDs = (int*) malloc(satellitesUsed * sizeof (int));
//                gpsData.elevations = (int*) malloc(satellitesUsed * sizeof (int));
//                gpsData.azimuths = (int*) malloc(satellitesUsed * sizeof (int));
//                gpsData.SNR = (int*) malloc(satellitesUsed * sizeof (int));
//            }

            int msgIndex = (strtol(values[2], &endptr, 10)-1)*4;
            int j;
            for (j = 0; j< 4; j++){
                gpsData.satelliteIDs[msgIndex+j] = strtol(values[4 + j*4], &endptr, 10);
                gpsData.elevations[msgIndex+j] = strtol(values[5 + j*4], &endptr, 10);
                gpsData.azimuths[msgIndex+j] = strtol(values[6 + j*4], &endptr, 10);
                gpsData.SNR[msgIndex + j] = strtol(values[7 + j * 4], &endptr, 10);
            }
        }

        else if(!strcmp(values[0], "$GPGGA")){
            gpsData.positionFixIndicator = strtol(values[6], &endptr, 10);
            gpsData.MSLAltitude = strtod(values[9], &endptr);
            gpsData.geoidSeparation = strtod(values[11], &endptr);
            gpsData.ageOfDiffCorr = strtol(values[6], &endptr, 10);
        }

        else if(!strcmp(values[0], "$GPGSA")){
            gpsData.PDOP = strtod(values[15], &endptr);
            gpsData.HDOP = strtod(values[16], &endptr);
            gpsData.VDOP = strtod(values[17], &endptr);
        }

        else if(!strcmp(values[0], "$GPVTG")){
            gpsData.courseTrue = strtod(values[1], &endptr);
            gpsData.courseMagnetic = strtod(values[3], &endptr);
            gpsData.speedKnots = strtod(values[5], &endptr);
            gpsData.speedKpH = strtod(values[7], &endptr);
        }
    }

    return gpsData;
}
//int sample_read(HANDLE hCom, int numMessages)
//{
//    DWORD bytesRead = 0;
//    int i = 0;
//    char* bytesToReceive = (char*) calloc(400, 1);
//    for (; i < numMessages; i++) {
//        if (!ReadFile(hCom, bytesToReceive, 400, &bytesRead, NULL)) {
//            printf("SetCommState failed. Error: %d.\n", GetLastError());
//            CloseHandle(hCom);
//            return (4);
//        } else {
//            printf("%s\n\n", bytesToReceive);
//        }
//    }
//
//    return 0;
//
//}

void processData(){}