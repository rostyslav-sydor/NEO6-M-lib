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


# Examples
Examples of usage for winapi and stm32 are in src/examples folder.

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
