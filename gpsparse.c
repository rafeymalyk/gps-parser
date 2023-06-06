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
const	char* checksumPos = NULL;  // Initialize a pointer to store the position of the checksum character
	for (int i = 0; packet[i] != '\0'; i++) {  // Iterate over each character in the packet string
	    if (packet[i] == '*') {  // Check if the current character is the checksum character '*'
	        checksumPos = &packet[i];  // Assign the address of the checksum character to the checksumPos pointer
	        break;  // Exit the loop since the checksum character is found
	    }
	}

	if (checksumPos == NULL) {
	    return PACKET_ERROR;  // Return the PACKET_ERROR error code if the checksum character is not found
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
    char* token = NULL;
    char* copy = (char*)malloc(strlen(packet) + 1);  // Allocate memory for the copy of the packet string
    strcpy(copy, packet);  // Copy the contents of the packet string to the newly allocated memory


                   // Initialize token pointer
    int tokenLength = 0;              // Initialize token length variable
    int packetLength = strlen(copy);  // Determine the length of the packet
    int headerLength = strlen("$GPGGA");  // Determine the length of the known header "$GPGGA"

    // Find the position of the delimiter ","
    for (int i = 0; i < packetLength; i++) {
        if (copy[i] == ',') {
            tokenLength = i;  // Store the position of the delimiter as token length
            break;
        }
    }

    if (tokenLength == 0) {
        free(copy);
        return PACKET_ERROR;  // Return error code if delimiter is not found
    }

    // Allocate memory for the token
    token = (char*)malloc(tokenLength + 1);

    // Copy the characters from the start to the delimiter position
    for (int i = 0; i < tokenLength; i++) {
        token[i] = copy[i];
    }

    token[tokenLength] = '\0';  // Add a null terminator to the token string

    // Check if the extracted token matches the known header "$GPGGA"
    if (strncmp(token, "$GPGGA", headerLength) != 0) {
        // Compare the first 'headerLength' characters of 'token' with the expected header "$GPGGA"
        // If the strings do not match, execute the following block of code

        free(token);
        // Free the memory allocated for 'token' since it does not match the expected header
        free(copy);
        // Free the memory allocated for 'copy' to release the copied packet
        return PACKET_ERROR;
        // Return the error code 'PACKET_ERROR' to indicate that the packet header is invalid
    }


    token = strchr(token + 1, ',');
    // Find the position of the next delimiter "," in the token string, starting from the next character
    // This allows the code to continue parsing from where it left the last time
    if (token == NULL || strlen(token) != 9) {
        free(copy);
        return TIME_ERROR; // Return error code if time format is invalid
    }
    strncpy(data->time, token + 1, sizeof(data->time) - 1);
    // Copy the extracted string (excluding the delimiter) to the destination buffer for time
    data->time[sizeof(data->time) - 1] = '\0';
    // Add a null terminator at the end to ensure the string is properly terminated

    token = strchr(token + 1, ',');
    // Find the position of the next delimiter "," in the token string, starting from the next character
    if (token == NULL || strlen(token) < 8) {
        free(copy);
        return LATITUDE_ERROR; // Return error code if latitude format is invalid
    }
    char lat[12];
    strncpy(lat, token + 1, sizeof(lat) - 1);
    // Copy the latitude string (excluding the delimiter) to the temporary buffer
    lat[sizeof(lat) - 1] = '\0';
    // Add a null terminator at the end to ensure the string is properly terminated
    double latDegrees = atof(lat);
    // Convert the latitude string to a double precision floating-point value using atof
    double latMinutes = atof(lat + 2);
    // Convert the minutes part of the latitude string (starting from the third character) to a double
    data->latitude = latDegrees + (latMinutes / 60.0);
    // Calculate the latitude value by adding the degrees and the minutes converted to degrees

    token = strchr(token + 1, ',');
    // Find the position of the next delimiter "," in the token string, starting from the next character
    if (token == NULL || strlen(token) < 5) {
        free(copy);
        return LONGITUDE_ERROR; // Return error code if longitude format is invalid
    }
    char lon[13];
    strncpy(lon, token + 1, sizeof(lon) - 1);
    // Copy the longitude string (excluding the delimiter) to the temporary buffer
    lon[sizeof(lon) - 1] = '\0';
    // Add a null terminator at the end to ensure the string is properly terminated
    double lonDegrees = atof(lon);
    // Convert the longitude string to a double precision floating-point value using atof
    double lonMinutes = atof(lon + 3);
    // Convert the minutes part of the longitude string (starting from the fourth character) to a double
    data->longitude = lonDegrees + (lonMinutes / 60.0);
    // Calculate the longitude value by adding the degrees and the minutes converted to degrees

    // Parse other parameters as required

    free(copy);
    return 0; // Parsing successful, return success code

}
