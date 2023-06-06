ESP-IDF template app
====================

This is a template application to be used with [Espressif IoT Development Framework](https://github.com/espressif/esp-idf).

Please check [ESP-IDF docs](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for getting started instructions.

*Code in this repository is in the Public Domain (or CC0 licensed, at your option.)
Unless required by applicable law or agreed to in writing, this
software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.*

If you want to open the whole project in ESP-IDF download the Zip file uploaded seperately in the files section
====================

GPS Parser Library
====================

The GPS Parser library is a C library for parsing GPS data packets and extracting individual parameters such as time, latitude, and longitude. This library provides a simple and efficient way to process GPS data in your embedded systems or IoT projects.

Features
========
1. Parse GPS data packets in the GGA format
2. Extract time, latitude, longitude, and other parameters from GPS data
3. Check packet integrity using checksum.

Installation
Clone the repository:
git clone https://github.com/your-username/gps-parser-library.git


Include the library in your project by adding gpsparse.h and gpsparse.c to your source files.

Main file to open is gpsparser.c
===================================


Usage
=====
Include the library header in your code:

#include "gpsparse.h"

Create a GPSData structure to hold the parsed GPS parameters:

GPSData gpsData;

Call the parse_gps_data function to parse the GPS packet and populate the GPSData structure:

UnitTest
========

const char* gpsPacket = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";

int result = parse_gps_data(gpsPacket, &gpsData);

Check the result of the parsing operation:

if (result == 0) {

    // Parsing successful, access the individual parameters
    printf("Time: %s\n", gpsData.time);
    printf("Latitude: %.4lf\n", gpsData.latitude);
    printf("Longitude: %.4lf\n", gpsData.longitude);
    // Access other parameters as required
} else {

    // Parsing failed, handle the error
    printf("Parsing failed. Error code: %d\n", result);
}
Error Codes

The library uses the following error codes to indicate specific parsing errors:

PACKET_ERROR: Invalid or incomplete packet format

TIME_ERROR: Invalid time format

LATITUDE_ERROR: Invalid latitude format

LONGITUDE_ERROR: Invalid longitude format

You can check the returned error code to handle parsing errors appropriately in your application.

Contributing
============
Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.
