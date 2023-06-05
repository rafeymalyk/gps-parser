#include <stdio.h>
#include "gpsparse.h"

void test_parse_gps_data() {
    const char* gpsPacket = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";
    GPSData gpsData;

    int result = parse_gps_data(gpsPacket, &gpsData);
    if (result == 0) {
        printf("Time: %s\n", gpsData.time);
        printf("Latitude: %.4lf\n", gpsData.latitude);
        printf("Longitude: %.4lf\n", gpsData.longitude);
    } else {
        printf("Error: Failed to parse GPS data\n");
    }
}

int main() {
    test_parse_gps_data();
    return 0;
}
