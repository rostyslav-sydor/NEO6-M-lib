# NEO6-lib
Library for NEO6 GPS modules

# Usage
## Reading data
1. Initialize `NEOData` struct. \
    ``NEOData neodata = {0};``


2. Pass data line by line to `NEOParseData` function. 
    Last argument is enabling checking checksum of message and discarding it in case of failure. \
    ``NEOParseData(dataStr, &neodata, 1);``


3. Access data by accessing members of `NEOData` struct. \
    ``int dateTimesFive = neodata.date * 5;``

## Configuring module
1. Create callback function with signature `int Write(uint8_t* msg, int length)`


2. Pass message without header and checksum into `NEOSendMessage` function.\
Configuration messages can be found by this link: https://www.u-blox.com/sites/default/files/products/documents/u-blox6_ReceiverDescrProtSpec_%28GPS.G6-SW-10018%29_Public.pdf \
    ``NEOSendMessage(msg, &Write);``


# Example
Using winapi for write function:
```c
#include "NEO6_M.h"
#include "windows.h"

HANDLE fhandle;

int Write(uint8_t* buffer, int counter) {
    DWORD bytesWritten = 0;
    return WriteFile(fhandle, buffer, counter, &bytesWritten, NULL);
}

int main(){
    // disabling VTG message
    uint8_t msg[] = {0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x00}; 
    NEOSendMessage(msg, &Write);
    
    // Example of parsing
    char* dataStr[] =  {"$GNRMC,202812.000,A,4949.0292,N,02401.5177,E,0.00,0.00,131221,,,A*73\n",
                        "$GNVTG,0.00,T,,M,0.00,N,0.00,K,A*23\n",
                        "$GNZDA,202812.000,13,12,2021,00,00*43\n",
                        "$GPGSV,3,2,10,18,35,197,,20,27,064,22,25,50,137,22,26,33,302,*7E\n",
                        "$GPGSV,3,3,10,29,78,042,,31,44,258,*77\n",
                        "$BDGSV,1,1,03,08,19,042,,11,57,061,,12,05,067,*5F\n",
                        "$GPTXT,01,01,01,ANTENNA OK*35\n",
                        "$GNGGA,202813.000,4949.0294,N,02401.5175,E,1,04,5.1,388.5,M,0.0,M,,*70\n",
                        "$GNGLL,4949.0294,N,02401.5175,E,202813.000,A,A*43\n",
                        "$GPGSA,A,3,12,02,25,05,,,,,,,,,6.9,5.1,4.6*38\n",
                        "$BDGSA,A,3,,,,,,,,,,,,,6.9,5.1,4.6*2A\n"};

    
    NEOData neoData = {0};
    int i = 0;
    for(;i < sizeof(dataStr)/sizeof(dataStr[0]); ++i){
        NEOParseData(dataStr[i], &neoData, 1);
    }
    
    NEOPrintData(&neoData);
}
```

# NEOData struct members
* double latitude    (ddmmm.mmmm format)
* double longitude   (ddmmm.mmmm format)
* char NS            (North-South indicator)
* char EW            (East-West indicator)
* int date           (ddmmmyy format)
* double UTCTime     hhmmss.sss format)
* int UTCYear
* int UTCMonth
* int UTCDay
* int localHours
* int localMinutes
* double speedKpH
* double speedKnots
* double courseTrue
* double courseMagnetic
* double HDOP
* double PDOP
* double VDOP
* double MSLAltitude
* double geoidSeparation
* double ageOfDiffCorr
* int DGPSStationID
* char dataStatus
* char mode

    Gsv data:
* int satellitesUsed
* int satelliteID[12]
* int elevation[12]
* int azimuth[12]
* int SNR[12]
