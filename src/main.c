
//#include "libserialport.h"
#include "NEO6_M.h"
#include "windows.h"
#include "stdio.h"

int main(int argc, char *argv[]){
    HANDLE hCom = setup();
    GPSData gpsdata = getData(hCom);
    printf("Lat: %f\nLon: %f\nPDOP: %f", gpsdata.latitude, gpsdata.longitude, gpsdata.PDOP);
    finish(hCom);
    return 0;
}