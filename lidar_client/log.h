#ifndef LOG_H
#define LOG_H

#include "wrapper.h"

int logLidar( char *logname);

void logGPS(int gpsfd, FILE* gpsfile);
void logLaser(int laserfd, FILE* laserfile);
#endif
