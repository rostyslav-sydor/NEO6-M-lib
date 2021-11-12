
//#include "libserialport.h"
#include "NEO6_M.h"
#include "windows.h"
#include "stdio.h"

int main(int argc, char *argv[]){
    hello();
    HANDLE hCom = setup();
//    HANDLE hCom;
    GPSData gpsdata = getData(hCom);
    printf("Lat: %f\nLon: %f", gpsdata.latitude, gpsdata.longitude);
//    printf("%s", );
    return(0);
}