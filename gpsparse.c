#include "gpsparse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PACKET_ERROR -1
#define TIME_ERROR -2
#define LATITUDE_ERROR -3
#define LONGITUDE_ERROR -4

int parse_gps_data(const char* packet, GPSData* data) {
    // Check packet integrity using checksum
    char* checksumPos = strchr(packet, '*');
    if (checksumPos == 0)
        return PACKET_ERROR;

    unsigned int checksum = 0;
    for (const char* p = packet + 1; p < checksumPos; ++p) {
        checksum ^= (unsigned int)*p;
    }

    unsigned int receivedChecksum;
    if (sscanf(checksumPos + 1, "%x", &receivedChecksum) != 1)
        return PACKET_ERROR;

    if (checksum != receivedChecksum)
        return PACKET_ERROR;

    // Extract individual parameters
    char* token;
    char* copy = strdup(packet);  // Make a copy of the packet to avoid modifying the original string

    token = strtok(copy, ",");
    if (token == 0 || strcmp(token, "$GPGGA") != 0) {
        free(copy);
        return PACKET_ERROR;  // Invalid packet format or not a GGA packet
    }

    token = strtok(0, ",");
    if (token == 0 || strlen(token) != 9) {
        free(copy);
        return TIME_ERROR;  // Invalid time format
    }
    strncpy(data->time, token, sizeof(data->time) - 1);
    data->time[sizeof(data->time) - 1] = '\0';

    token = strtok(0, ",");
    if (token == 0 || strlen(token) < 4) {
        free(copy);
        return LATITUDE_ERROR;  // Invalid latitude format
    }
    char lat[12];
    strncpy(lat, token, sizeof(lat) - 1);
    lat[sizeof(lat) - 1] = '\0';
    double latDegrees = strtod(lat, 0);
    double latMinutes = strtod(lat + 2, 0);
    data->latitude = latDegrees + (latMinutes / 60.0);

    token = strtok(0, ",");
    if (token == 0 || strlen(token) < 5) {
        free(copy);
        return LONGITUDE_ERROR;  // Invalid longitude format
    }
    char lon[13];
    strncpy(lon, token, sizeof(lon) - 1);
    lon[sizeof(lon) - 1] = '\0';
    double lonDegrees = strtod(lon, 0);
    double lonMinutes = strtod(lon + 3, 0);
    data->longitude = lonDegrees + (lonMinutes / 60.0);

    // Parse other parameters as required

    free(copy);
    return 0;  // Parsing successful
}
