#include "NEO6.h"
#include "windows.h"

HANDLE fhandle;

int Write(uint8_t* buffer, int counter) {
    DWORD bytesWritten = 0;
    return WriteFile(fhandle, buffer, counter, &bytesWritten, NULL);
}

int main(){
//    fhandle = winSetup();
//    if (fhandle == (HANDLE) 1) return 1;

    char* dataStr[] =  {"$GPRMC,,V,,,,,,,,,,N*53\n",
                        "$GPVTG,,,,,,,,,N*30\n",
                        "$GPGGA,,,,,,0,00,99.99,,,,,,*48\n",
                        "$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30\n",
                        "$GPGSV,1,1,00*79\n",
                        "$GPGLL,,,,,,V,N*64\n",
                        "$GPGSV,3,2,10,18,35,197,,20,27,064,22,25,50,137,22,26,33,302,*7E\n",
                        "$GPGSV,3,3,10,29,78,042,,31,44,258,*77\n",
                        "$BDGSV,1,1,03,08,19,042,,11,57,061,,12,05,067,*5F\n",
                        "$GNRMC,202812.000,A,4949.0292,N,02401.5177,E,0.00,0.00,131221,,,A*73\n",
                        "$GNVTG,0.00,T,,M,0.00,N,0.00,K,A*23\n",
                        "$GNZDA,202812.000,13,12,2021,00,00*43\n",
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
    uint8_t msg[] = {0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x00};
    NEOSendMessage(msg, &Write);

    NEOPrintData(&neoData);

    CloseHandle(fhandle);
    return 0;
} 