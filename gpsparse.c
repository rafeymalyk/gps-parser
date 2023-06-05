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
    if (checksumPos == 0) {
        return PACKET_ERROR; // Return error code if checksum character is not found
    }

    unsigned int checksum = 0;
    for (const char* p = packet + 1; p < checksumPos; ++p) {
        checksum ^= (unsigned int)*p; // Calculate checksum by XORing each character
    }

    unsigned int receivedChecksum;
    if (sscanf(checksumPos + 1, "%x", &receivedChecksum) != 1) {
        return PACKET_ERROR; // Return error code if checksum parsing fails
    }

    if (checksum != receivedChecksum) {
        return PACKET_ERROR; // Return error code if checksum does not match
    }

    // Extract individual parameters
    char* token;
    char* copy = strdup(packet); // Make a copy of the packet to avoid modifying the original string

    token = strtok(copy, ",");
    if (token == 0 || strcmp(token, "$GPGGA") != 0) {
        free(copy);
        return PACKET_ERROR; // Return error code if packet is not in GGA format
    }

    token = strtok(0, ",");
    if (token == 0 || strlen(token) != 9) {
        free(copy);
        return TIME_ERROR; // Return error code if time format is invalid
    }
    strncpy(data->time, token, sizeof(data->time) - 1);
    data->time[sizeof(data->time) - 1] = '\0';

    token = strtok(0, ",");
    if (token == 0 || strlen(token) < 4) {
        free(copy);
        return LATITUDE_ERROR; // Return error code if latitude format is invalid
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
        return LONGITUDE_ERROR; // Return error code if longitude format is invalid
    }
    char lon[13];
    strncpy(lon, token, sizeof(lon) - 1);
    lon[sizeof(lon) - 1] = '\0';
    double lonDegrees = strtod(lon, 0);
    double lonMinutes = strtod(lon + 3, 0);
    data->longitude = lonDegrees + (lonMinutes / 60.0);

    // Parse other parameters as required

    free(copy);
    return 0; // Parsing successful, return success code
}
