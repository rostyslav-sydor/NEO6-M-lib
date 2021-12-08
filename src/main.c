#include "NEO6_M.h"
#include "windows.h"


HANDLE fhandle;

int Read(void* buffer, size_t counter) {
    DWORD bytesRead = 0;
    return ReadFile(fhandle, buffer, counter, &bytesRead, NULL);
}

int Write(void* buffer, size_t counter) {
    DWORD bytesWritten = 0;
    return WriteFile(fhandle, buffer, counter, &bytesWritten, NULL);
}



int main(){
    fhandle = winSetup();
    if (fhandle == (HANDLE) 1) return 1;

    int (*readPtr)(void* buffer, size_t counter) = &Read;
    int (*writePtr)(void* buffer, size_t counter) = &Write;

//    GPSSetSerialPort(writePtr, 0, 9600, 8, 1, 0);
//    GPSLLANavigationInit(writePtr, 11.11, 2.2, 33.33, 96000, 12, 500, 12, 3);
//    GPSMessageRate(writePtr, 3, 0, 1);
    GPSData gpsdata = GPSGetData(readPtr);
    GPSPrintData(gpsdata);

    CloseHandle(fhandle);
    return 0;
}