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
    char* checksumPos = strchr(packet, '*');  //In packet it will locate the checksum charachter"*" in the input packet string
    if (checksumPos == 0) {                   //checks if its still zero means charachteer is not found in packet string
        return PACKET_ERROR; // Return error code if checksum character is not found
    }

    unsigned int checksum = 0;
    for (const char* p = packet + 1; p < checksumPos; ++p) {    //for loop iterates through each charachter after $ upto the checksum charachter 1 by 1.
        checksum ^= (unsigned int)*p; // Calculate checksum by XORing each character
    }

    unsigned int receivedChecksum;
    if (sscanf(checksumPos + 1, "%x", &receivedChecksum) != 1) {    // Parsing the received checksum value from string using sscanf and convert it to hexadecimal using'%x' and send it to the address of received checksum variablle
        return PACKET_ERROR; // Return error code if checksum parsing fails
    }

    if (checksum != receivedChecksum) {   // check if received an calculated checksums are same
        return PACKET_ERROR; // Return error code if checksum does not match
    }

    // Extract individual parameters
    char* token;
    char* copy = strdup(packet); // Make a copy of the packet to avoid modifying the original string

    token = strtok(copy, ","); // extracting packet header $GPGGA after "," charachter
    if (token == 0 || strcmp(token, "$GPGGA") != 0) { // validating it with already known header
        free(copy);                                  // free the allocated space if not true
        return PACKET_ERROR; // Return error code if packet is not in GGA format
    }

    token = strtok(0, ","); // starts from where it left last time in the last copy and extracts next token
    if (token == 0 || strlen(token) != 9) { // checking if the format is hhmmss.ss
        free(copy);
        return TIME_ERROR; // Return error code if time format is invalid
    }
    strncpy(data->time, token, sizeof(data->time) - 1); // Copy the extracted string to the destinated buffer to store extracted time
    data->time[sizeof(data->time) - 1] = '\0';// adds null charachter at the end ensuring its null terminated

    token = strtok(0, ","); // gets next token where it last ends
    if (token == 0 || strlen(token) < 4) { // check latitude format
        free(copy);
        return LATITUDE_ERROR; // Return error code if latitude format is invalid
    }
    char lat[12]; // array declared to store latitude string extracted
    strncpy(lat, token, sizeof(lat) - 1); //copy token from string that fits with lat array
    lat[sizeof(lat) - 1] = '\0';
    double latDegrees = strtod(lat, 0); // converts to double precision floating point value because latitude coordinates have decimal values, these values are basically minutes and seconds
    double latMinutes = strtod(lat + 2, 0);//starts from third charachter
    data->latitude = latDegrees + (latMinutes / 60.0); // minutes are converted back to degrees and added to latitudedegrees and final value assigned to latitude

    token = strtok(0, ",");
    if (token == 0 || strlen(token) < 5) { // checks longitude format
        free(copy);
        return LONGITUDE_ERROR; // Return error code if longitude format is invalid
    }
    char lon[13];
    strncpy(lon, token, sizeof(lon) - 1);
    lon[sizeof(lon) - 1] = '\0';
    double lonDegrees = strtod(lon, 0);
    double lonMinutes = strtod(lon + 3, 0);
    data->longitude = lonDegrees + (lonMinutes / 60.0); // minutes are converted to degress and added and assigned

    // Parse other parameters as required

    free(copy);
    return 0; // Parsing successful, return success code
}
