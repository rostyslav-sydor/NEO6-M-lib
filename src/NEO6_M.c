#include "NEO6_M.h"
#include <stdio.h>
#include "string.h"

GPSData GPSGetData(int (*read)(void* buffer, size_t count)){
    GPSData gpsData;
//    char dataStr[] =    "$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598, ,*10\n"
//                        "$GPVTG,309.62,T, ,M,0.13,N,0.2,K,A*23\n"
//                        "$GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M, , , ,0000*18\n"
//                        "$GPGSA,A,3,07,02,26,27,09,04,15, , , , , ,1.8,1.0,1.5*33\n"
//                        "$GPGSV,2,1,07,07,79,048,42,02,51,062,43,26,36,256,42,27,27,138,42*71\n"
//                        "$GPGSV,2,2,07,09,23,313,42,04,19,159,41,15,12,041,42*41\n"
//                        "$GPGLL,3723.2475,N,12158.3416,W,161229.487,A,A*41";

    START:;
    char* dataStr = GPSReadData(read);
    printf("%s\n\n",dataStr);
    char *tmp, *value;
    char *values[30];
    int dataPointer = 0;

    for (tmp = strtok(dataStr,"\n"); tmp != NULL; tmp = strtok(dataStr + dataPointer, "\n")){
        dataPointer += (int) strlen(tmp) + 1;
        int i = 0;
        for (value = strtok(tmp, ",*"); value != NULL; value = strtok(NULL, ",*")){
            values[i] = strdup(value);
            i++;
        }
        char* endptr;
        if (!strcmp(values[0], "$GPRMC")){
            if (!strcmp(values[1], "V")){
                printf("Invalid data. Trying again...\n\n");
                goto START;
            }
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
char* GPSReadData(int (*read)(void* buffer, size_t count))
{
    char* bytesToReceive = (char*) calloc(500, 1);

    if (!read(bytesToReceive, 500)) {
        printf("SetCommState failed.");
        return bytesToReceive;
    }

    char* dataStart,* dataEnd,* output;
    if((dataStart = strstr(bytesToReceive, "$GPRMC")))
    {
        dataEnd = dataStart; // move end to start of the string
        dataEnd = strstr(dataEnd, "GPGLL");
        if (!dataEnd){
            return "error";
        }
        dataEnd = strstr(dataEnd, "\n");
        output = malloc(dataEnd - dataStart);
        memcpy(output, dataStart, dataEnd-dataStart);
    }
    return output;
}

void GPSMessageBuilder(int (*write)(void* buffer, size_t count), char* format, ...){
    int maxLen = 80;
    char* msg = malloc(maxLen);
    va_list args;
    va_start(args, format);
    int length = vsnprintf(msg, maxLen, format, args);
    GPSSendMessage(write, msg, (size_t) length);
    free(msg);
}

void GPSSetSerialPort(int (*write)(void* buffer, size_t count), int protocol, int baudrate,
                      int dataBits, int stopBits, int parity){
    GPSMessageBuilder(write, "PSRF100,%i,%i,%i,%i,%i", 0, baudrate, dataBits, stopBits, parity);

}

void GPSNavigationInit(int (*write)(void* buffer, size_t count), int x, int y, int z,
                       int clkOffset, int timeOfWeek, int weekNo, int channelCount, int resetCfg){
    GPSMessageBuilder(write, "PSRF101,%i,%i,%i,%i,%i,%i,%i,%i", x, y, z, clkOffset, timeOfWeek, weekNo, channelCount, resetCfg);
}

void GPSSetDGPSPort(int (*write)(void* buffer, size_t count),int baudrate,
                    int dataBits, int stopBits, int parity){
    GPSMessageBuilder(write, "PSRF102,%i,%i,%i,%i", baudrate, dataBits, stopBits, parity);
}

void GPSMessageQuery(int (*write)(void* buffer, size_t count), int msg, int checksumEnable){
    GPSMessageBuilder(write, "PSRF103,%02i,01,00,%02i", msg, checksumEnable);
}

void GPSMessageRate(int (*write)(void* buffer, size_t count), int msg, int rate, int checksumEnable){
    GPSMessageBuilder(write, "PSRF103,%02i,00,%02i,1", msg, checksumEnable);
}

void GPSLLANavigationInit(int (*write)(void* buffer, size_t count), double latitude, double longitude, double altitude,
                          int clkOffset, int timeOfWeek, int weekNo, int channelCount, int resetCfg){
    GPSMessageBuilder(write, "PSRF104,%f,%f,%f,%i,%i,%i,%i,%i", latitude, longitude, altitude,
                      clkOffset, timeOfWeek, weekNo, channelCount, resetCfg);
}

void GPSDebugMode(int (*write)(void* buffer, size_t count), int mode){
    GPSMessageBuilder(write, "PSRF105,%i", mode);
}

void GPSSelectDatum(int (*write)(void* buffer, size_t count), int datum){
    GPSMessageBuilder(write, "PSRF106,%i", datum);
}

void GPSPrintData(GPSData gpsdata){
    printf("Lat: %.3f %c\nLon: %.3f %c\ndate: %i %.3f\n"
           "Course: %.3f\nSpeed: %.3f KpH",
           gpsdata.latitude, gpsdata.NS,
           gpsdata.longitude, gpsdata.EW,
           gpsdata.date, gpsdata.UTCTime,
           gpsdata.courseTrue, gpsdata.speedKpH);
}

void GPSSendMessage(int (*write)(void* buffer, size_t count), char* msg, size_t length){
    int decoratorLen = 8; // ${msg}*OC\r\n
    int checksum = GPSChecksum(msg, length);
    char* finalMsg = malloc(length + decoratorLen);
    int msgLen = snprintf(finalMsg, length+decoratorLen, "$%s*%02X\r\n", msg, checksum);
    write(msg, msgLen);

    free(finalMsg);
    finalMsg = NULL;
}

int GPSChecksum(const char* msg, size_t length){
    int checksum = 0, i = 0;
    for(; i < length; i++){
        checksum ^= msg[i];
    }
    return checksum;
}

HANDLE winSetup(){
    DCB dcb={ sizeof(dcb) };
    HANDLE hCom;
    char *pcCommPort = "COM3";
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 200;
    timeouts.WriteTotalTimeoutConstant = 100;

    hCom = CreateFile( pcCommPort,
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ,    // must be opened with exclusive-access
                       NULL, // no security attributes
                       OPEN_EXISTING, // must use OPEN_EXISTING
                       FILE_ATTRIBUTE_NORMAL,    // not overlapped I/O
                       NULL  // hTemplate must be NULL for comm devices
    );

    if (hCom == INVALID_HANDLE_VALUE){
        printf ("CreateFile failed with error %d.\n", GetLastError());
        CloseHandle(hCom);
        return (HANDLE) 1;
    }

    if (!GetCommState(hCom, &dcb)){
        printf ("GetCommState failed with error %d.\n", GetLastError());
        CloseHandle(hCom);
        return (HANDLE) 2;
    }
    dcb.BaudRate = 9600;     // set the baud rate
    dcb.ByteSize = 8;             // byte size
    dcb.Parity = NOPARITY;        // no parity bit
    dcb.StopBits = ONESTOPBIT;    // one stop bit

    if (!SetCommState(hCom, &dcb)){
        printf ("SetCommState failed. Error: %d.\n", GetLastError());
        CloseHandle(hCom);
        return (HANDLE) 3;
    }
    printf ("Serial port %s successfully configured.\n", pcCommPort);

    return hCom;
}