#ifndef VELODYNE_H
#define VELODYNE_H

#include <stdint.h>
#include <stdio.h>
#include <libc.h>
#include <strings.h>
#include <math.h>

#define IRANGE_HDL32E     255 // Intensity range of HDL-32E
#define ISCALE_HDL32E     65,536/IRANGE_HDL32E 
#define XSCALE_HDL32E     0.002
#define LIDAR_LEN_HDL32E  1206
#define GPS_LEN_HDL32E    512
#define BLANK_DIST        500
#define DEG2RAD           *M_PI/180.0f
#define CAL_DIM_SCALE     *5
#define LASER_UDP_PORT     2368
#define ODOMETRY_UDP_PORT  8308


typedef float  float32_t;
typedef double float64_t;

/* pragma'd section is for structs that are sensetive to byte alignment by the
   compiler */
#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
 /**
  * Laser Data
  ******************************************************************************/
typedef struct
{
  uint16_t                      distance;
  uint8_t                       intensity;
} laserHDL32;

typedef struct
{

  uint16_t	                blockID;
  uint16_t                      bearing;
  laserHDL32                    shot[32];
  
} laserBlock;

/**
 * Lidar Data Packet
 ******************************************************************************/
typedef struct
{
  laserBlock                    block[12];
  uint32_t                      gpsTime;
  uint8_t                       statusType;
  uint8_t                       statusValue;
} lidar_packet;

/**
 * Odometry Packet:
 * This structure houses IMU and GPS information sent from the HDL-32
 ******************************************************************************/
typedef struct
{
  char                          pad[14];
  uint16_t                      gyro1;
  uint16_t                      temp1;
  uint16_t                      accel1_x;
  uint16_t                      accel1_y;
  uint16_t                      gyro2;
  uint16_t                      temp2;
  uint16_t                      accel2_x;
  uint16_t                      accel2_y;
  uint16_t                      gyro3;
  uint16_t                      temp3;
  uint16_t                      accel3_x;
  uint16_t                      accel3_y;
  char                          pad1[160];
  uint32_t                      timestamp;
  char                          pad2[4];
  char                          NMEA[72];
  char                          pad3[234];

} odometry_packet;

typedef struct
{

} NMEA_string;


/**
 * LAS Header:
 * This struct contains the header information for a LAS format LIDAR file
 ******************************************************************************/
typedef struct
{
  char            signature[4];
  uint16_t        source;
  uint16_t        encoding;
  uint32_t        guid1;
  uint16_t        guid2; 
  uint16_t        guid3;
  unsigned char   guid4[8];
  uint8_t         versionMajor;
  uint8_t         versionMinor;
  char            systemID[32];
  char            genSoft[32];
  uint16_t        createDOY;
  uint16_t        createYear;
  uint16_t        headerSize;
  uint32_t        dataOffset;
  uint32_t        variableRecords;
  uint8_t         pt_format;
  uint16_t        pt_length;
  uint32_t        numberOfPointsOld;
  uint32_t        ptsByReturnOld[5];
  float64_t       xScale;
  float64_t       yScale;
  float64_t       zScale;
  float64_t       xOffset;
  float64_t       yOffset;
  float64_t       zOffset;
  float64_t       xMax;
  float64_t       xMin;
  float64_t       yMax;
  float64_t       yMin;
  float64_t       zMax;
  float64_t       zMin;
  uint64_t        startWave;
  uint64_t        startVLR;
  uint32_t        totalVLR;
  uint64_t        totalPts;
  uint64_t        ptsByReturn[15];
} headerLAS;

/**
 * LAS Point 0 Format:
 * 
 * This struct conforms to the most basic point data format used by the LAS
 * LIDAR format specification.
 ******************************************************************************/
typedef struct 
{
  int32_t   x;
  int32_t   y;
  int32_t   z;
  uint16_t  intensity;
  uint8_t   returnNum:3;
  uint8_t   returnsTot:3;
  uint8_t   scanDir:1;  
  uint8_t   eol:1; //edge of line
  uint8_t   class;
  int8_t    scanRank;
  uint8_t   userData;
  uint16_t  ptSourceID;
} pt0;


#pragma pack(pop)   /* restore original alignment from stack */

/**
 * Laser Calibration Data:
 * 
 * Laser calibration data includes all calibration coefficients used to correct
 * raw HDL-32 captures and reproject them into xyz space.
 ******************************************************************************/
typedef struct
{
  double    rotC;
  double    vertC;
  double    distC;
  double    distC_X;
  double    distC_Y;
  double    vOffset;
  double    hOffset;
  double    focalDistance;
  double    focalSlope;
} laserCalibration;

typedef struct
{
  double    rotC;
  double    cosRotC;
  double    sinRotC;
  double    vertC;
  double    cosVertC;
  double    sinVertC;
  double    distC;
  double    distC_X;
  double    distC_Y;
  double    vOffset;
  double    hOffset;
  double    focalDistance;
  double    focalSlope;
} calibTable;

/**
 * Survey state:
 * 
 * The surveyState struct houses current information about the state of the 
 * sensor (i.e. position, attitude, velocity) and summary statistics of the 
 * survey. This function is intended to simplify the API, allowing the coder
 * to pass ALL survey state infomation to functions with a single parameter.
 * ....foo(x, y, z, theta, lambda, apple, hotel, president) == FAIL!!
 ******************************************************************************/
typedef struct
{
  /* survey summary statistics */
  pt0       min;
  pt0       max;
  pt0       refPt;   /* current ground reference point */
  uint64_t  totalPts;

  /* survey state variables */
  double    bearing;
  calibTable cal[32];  

} surveyState; 


/**
 * Function Prototypes
 ******************************************************************************/
int stationCap2Las(surveyState *restrict ss, FILE *restrict in, FILE *restrict out);
void setupCalibTable(surveyState *restrict ss);
void headerSetupHDL32E(surveyState *restrict ss, headerLAS *restrict h);
void writeHeader(headerLAS *restrict h, FILE *restrict las);
void writePacket( surveyState *restrict ss, lidar_packet *restrict in, FILE *restrict lasFile );

int projectPt(pt0 *restrict pt, laserHDL32 *restrict shot, calibTable *restrict cal, surveyState *restrict ss);
void setPoint(pt0 *restrict pt, int x, int y, int z, int i);
void updateMinMax(const pt0 *restrict pt, pt0 *restrict min, pt0 *restrict max);

double sind(double angle);
double cosd(double angle);

#endif
