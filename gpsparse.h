#ifndef GPS_PARSE_H
#define GPS_PARSE_H

typedef struct {
    char time[10];
    double latitude;
    double longitude;
    // Add other parameters as required
}


 GPSData;

int parse_gps_data(const char* packet, GPSData* data);

#endif  // GPS_PARSER_H
