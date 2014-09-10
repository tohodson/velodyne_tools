/**
 *
 ******************************************************************************/
#include "velodyne.h"


/* angle from vertical of each of the laser in the bank */
const double theta[32] = {-30.67  DEG2RAD,
			  -9.33   DEG2RAD,
			  -29.33  DEG2RAD,
			  -8.00   DEG2RAD,
			  -28.00  DEG2RAD,
			  -6.66   DEG2RAD,
			  -26.66   DEG2RAD,
			  -5.33   DEG2RAD,
			  -25.33   DEG2RAD,
			  -4.00   DEG2RAD,
			  -24.00   DEG2RAD,
			  -2.67   DEG2RAD,
			  -22.67   DEG2RAD,
			  -1.33   DEG2RAD,
			  -21.33   DEG2RAD,
			  0.00   DEG2RAD,
			  -20.00   DEG2RAD,
			  1.33   DEG2RAD,
			  -18.67   DEG2RAD,
			  2.67   DEG2RAD,
			  -17.33   DEG2RAD,
			  4.00   DEG2RAD,
			  -16.00   DEG2RAD,
			  5.33   DEG2RAD,
			  -14.67   DEG2RAD,
			  6.67   DEG2RAD,
			  -13.33   DEG2RAD,
			  8.0   DEG2RAD,
			  -12.00   DEG2RAD,
			  9.33   DEG2RAD,
			  -10.67   DEG2RAD,
			  10.67   DEG2RAD, };




static const laserCalibration rawCal[32] = { // combine raw calibration w/ theta to make table
  {-5.3328056,-7.2988362,111,118,118,19.736338,2.5999999,0,0},
  {-3.2344019,-6.9644198,146,149,151,19.778963,-2.5999999,0,0},
  {2.4376695, 0.250889, 131.76823, 136, 136, 20.688799, 2.5999999,0,0},
  {4.7373252,0.55538797,138.12656,142,144,20.727015,-2.5999999,0,0},
  {-1.0502493,-6.6410818,119,127,128,19.82012,2.5999999,0,0},
  {1.2386309,-6.2594609,135,140,144,19.868624,-2.5999999,0,0},
  {-1.8405367,-8.6656351,132,135,137,19.561426,2.5999999,0,0},
  {0.4511103,-8.3104696,145,145,149,19.606993,-2.5999999,0,0},
  {3.2611551,-5.9352221,116,122,121,19.909781,2.5999999,0,0},
  {5.4685535,-5.587399,133.99889,140,142,19.953875,-2.5999999,0,0},
  {2.4743285,-7.9891219,117,125,123,19.648148,2.5999999,0,0},//
  {4.7189918, -7.644258, 145, 146, 151, 19.692244,-2.5999999,0,0},//
  {-5.3511744,-5.3511744,118.71672,124,123,20.249313,2.5999999,0,0},
  {-3.1158857,-2.850472,142.90839,148,149, 20.29929, -2.5999999,0,0},
  {-6.1270261,-5.204318,120,127,124,20.00238, 2.5999999,0,0},
  {-3.852011, -4.9137921, 137, 144, 144, 20.039125,-2.5999999,0,0},
  {-1.1109436,-2.4998751, 101.71324, 111, 109, 20.343384, 2.5999999,0,0},
  {1.1519098,-2.184176,145.31258, 153, 157, 20.38307, -2.5999999,0,0},
  {-1.8682934,-4.5764661,130,135,134,20.081753,2.5999999,0,0},
  {0.43604341,-4.2038751,147,149,152,20.128786,-2.5999999,0,0},
  //
  {3.1763444,-1.798143,115.82812,124,123,20.431576,2.5999999,0,0},
  {5.4284201,-1.49388, 146,152,154,20.469791,-2.5999999,0,0},
  {2.4024715,-1.877563,129.29713,137,136,20.169943,2.5999999,0,0},
  {4.6698937,-3.492661,157.97737,162,165,20.218447,-2.5999999,0,0},
  {-5.3977456,0.88327599,124.12312,132,129,20.768169,2.5999999,0,0},
  {-3.1504908,1.234519,128,135,138,20.812265,-2.5999999,0,0},
  {-6.1759849,-1.177825,133,140,139,20.509478,2.5999999,0,0},
  //
  {-3.8819003,-0.86169797,130,135,134,20.549164,-2.5999999,0,0},
  {-1.1136208,1.585669,131,137,137,20.856361,2.5999999,0,0},
  {1.0969903,1.925001,131,139,142,20.898987,-2.5999999,0,0},
  {-1.9088749,-0.486963,138.48773,145,143,20.596197,2.5999999,0,0},
  {0.36758029,-0.123898,137.21994,143,145,20.641764,-2.5999999,0,0},
};


/**
 *
 ******************************************************************************/
int stationCap2Las(surveyState *ss, FILE *in, FILE *out)
{
  lidar_packet lidar_in;
  headerLAS h;

  /* prepare to write points to LAS by seeking to the end of the header */
  fseek(out, sizeof(headerLAS), SEEK_SET); 

  while ( fread(&lidar_in, sizeof(lidar_packet), 1, in ) > 0) {
    writePacket(ss, &lidar_in, out);
  }

  /* setup LAS file header and write to file */
  headerSetupHDL32E(ss, &h);   
  writeHeader(&h,out);
  return 1;
}

/**
 * Writes a LAS header to the beginning of a LAS file
 *
 * @param *h pointer to struct containing LAS header information
 * @param *las FILE* of open .LAS file.
 * @note seek to beginning of file before writing header
 ******************************************************************************/
void writeHeader(headerLAS* h, FILE *las) 
{
  rewind(las);
  fwrite(h, sizeof(headerLAS), 1,  las);
}

/**
 * Setup LAS header based on HDL-32 defaults and current survey statitistics
 *
 * @note seek to zero before writing header
 ******************************************************************************/
void headerSetupHDL32E(surveyState *ss, headerLAS* h){
  memset( h, 0, sizeof(headerLAS) );
  strncpy(h->signature,"LASF", 4);
  h->source = 0; 
  h->versionMajor = 1;
  h->versionMinor = 4;
  strncpy(h->systemID, "HDL-32E", sizeof(h->systemID));
  strncpy(h->genSoft, "test", sizeof(h->genSoft));
  h->createDOY = 1; // XXX call function to return DOY
  h->createYear = 2014; // XXX similar to above
  h->headerSize = sizeof(headerLAS); //375
  h->dataOffset = sizeof(headerLAS);
  h->variableRecords = 0;
  h->pt_format = 0; // format 0 of LAS 1.4
  h->pt_length = sizeof(pt0); // 20 bytes

  h->numberOfPointsOld = (uint32_t) ss->totalPts;
  h->ptsByReturnOld[0] = (uint32_t) ss->totalPts;
  h->xScale = XSCALE_HDL32E;
  h->yScale = XSCALE_HDL32E;
  h->zScale = XSCALE_HDL32E;
  h->xMax = (float64_t) ss->max.x;
  h->yMax = (float64_t) ss->max.y;
  h->zMax = (float64_t) ss->max.z;
  h->xMin = (float64_t) ss->min.x;
  h->yMin = (float64_t) ss->min.y;
  h->zMin = (float64_t) ss->min.z;
  h->totalPts = ss->totalPts;
  h->ptsByReturn[0] = ss->totalPts;

}

/**
 * Reads in a data packet from the HDL-32 and appends points to *.LAS LIDAR file. 
 *
 * @param *ss pointer to struct containing survey state and summary stats.
 * @param *in pointer to struct containing HDL-32 LIDAR data packet.
 * @note seek by sizeof(headerLAS) befor writing packets
 ******************************************************************************/
void writePacket(surveyState *ss, lidar_packet *in, FILE *lasFile)
{
  /* double xyDistance, distance, bearing; */
  pt0 pt;

  for (int i = 0; i < 12; i++) {

    ss->bearing = in->block[i].bearing / (double)100; //#define a scale
    
    for ( int j = 0; j < 32; j++ ) {
      memset( &pt, 0, sizeof(pt) );

      if ( projectPt(&pt, &in->block[i].shot[j], &ss->cal[j], ss) ) {
	fwrite(&pt, sizeof(pt), 1,  lasFile);
	ss->totalPts++;
      } //else printf("DOH!\n");
    }
  }
}
/**
 *
 ******************************************************************************/
int projectPt(pt0 *pt, laserHDL32 *shot, calibTable *cal, surveyState *ss){
  double xyDistance, distance;

  distance = ss->cal->distC + (double)shot->distance;
  
  /* cull points closer than blanking distance and out of range*/

  /* if (shot->distance < BLANK_DIST || shot->distance == 0) */
  if (shot->distance < BLANK_DIST || shot->distance > 2000 || shot->distance == 0) //XXXDEBUG
    return 0;

  xyDistance = distance * cal->cosVertC;
  
  pt->x = (int32_t)(ss->refPt.x + xyDistance * sin( ss->bearing DEG2RAD + cal->rotC) );
  pt->y = (int32_t)(ss->refPt.y + xyDistance * cos( ss->bearing DEG2RAD + cal->rotC) );
  pt->z = (int32_t)(ss->refPt.z + distance * cal->sinVertC);
  pt->intensity = shot->intensity * ISCALE_HDL32E;
  
  updateMinMax(pt, &ss->min, &ss->max);
  pt->returnNum = 1;
  pt->returnsTot = 1;

  return 1;
  
}

/**
 * Set x, y, z, and i values of a LAS point.
 *
 * @param *pt pointer to point structure
 * @param x x-coordinate of point
 ******************************************************************************/
void setPoint(pt0 *pt, int x, int y, int z, int i)
{
  pt->x = x;
  pt->y = y;
  pt->z = z;
  pt->intensity = i;
}

/**
 * Checks if a point is a min/max, updating the min/max.
 *
 * @param *pt point 
 * @param *min pt0 (point) structure containing minimum xyz values
 * @param *min pt0 (point) structure containing maximum xyz values
 ******************************************************************************/
void updateMinMax(const pt0 *pt, pt0 *min, pt0 *max)
{
  /* update max's */
  if (pt->x > max->x) max->x = pt->x;
  if (pt->y > max->y) max->y = pt->y;
  if (pt->z > max->z) max->z = pt->z; 
  if (pt->intensity > max->intensity) max->intensity= pt->intensity; //XXX DEBUG
  /* update min's */
  if (pt->x < min->x) min->x = pt->x;
  if (pt->y < min->y) min->y = pt->y;
  if (pt->z < min->z) min->z = pt->z;
}

/**
 * Generate calibration table
 ******************************************************************************/
void setupCalibTable(surveyState *ss) 
{
  for (int i = 0; i < 32; i++) {
    ss->cal[i].rotC = rawCal[i].rotC DEG2RAD;
    ss->cal[i].sinRotC = sin(ss->cal[i].rotC);
    ss->cal[i].cosRotC = cos(ss->cal[i].rotC);
    ss->cal[i].vertC = theta[i] + rawCal[i].vertC DEG2RAD; //mind brackets
    ss->cal[i].cosVertC = cos(ss->cal[i].vertC);
    ss->cal[i].sinVertC = sin(ss->cal[i].vertC);
    ss->cal[i].distC =rawCal[i].distC;
    ss->cal[i].distC_X = rawCal[i].distC_X;
    ss->cal[i].distC_Y = rawCal[i].distC_Y;
    ss->cal[i].vOffset = rawCal[i].vOffset;
    ss->cal[i].hOffset = rawCal[i].hOffset;
    ss->cal[i].focalDistance = rawCal[i].focalDistance;
    ss->cal[i].focalSlope = rawCal[i].focalSlope;
  }
}

double sind(double angle)
{
  double angleradians = angle * M_PI / 180.0f;
  return sin(angleradians) * M_PI / 180.0f;
}

double cosd(double angle)
{
  double angleradians = angle * M_PI / 180.0f;
  return cos(angleradians) * M_PI / 180.0f;
}

