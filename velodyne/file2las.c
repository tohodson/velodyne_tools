/**
 * A simple CL utility that pipes a HDL-32E capture to a *.LAS format
 ******************************************************************************/
#include "velodyne.h"
#include <string.h>

static void usage(char *);
/**
 *
 ******************************************************************************/
int main (int argc, char **argv)
{
  FILE *in, *out;
  surveyState ss;
  
  memset( &ss, 0, sizeof(ss) );
  
  setupCalibTable(&ss);

  if (argc == 6) {
    setPoint( &ss.refPt, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), 0);
  } else if (argc == 3) {
    setPoint( &ss.refPt, 0, 0, 0, 0);
  } else {
    usage("Invalid arguments");
  }

  in = fopen( argv[1], "r");
  out = fopen( argv[2],"w");
  stationCap2Las( &ss, in, out );

  fclose(in);
  fclose(out);
  
  return 0;
}

/**
 * Prints usage information for program.
 * @param msg Message to print with usage information
 ******************************************************************************/
static void usage (char *msg)
{
  if (msg) 
    fputs(msg, stderr);

  fprintf(stderr, "\nUsage: file2las <input capture> <output LAS> [x] [y] [z]\n");
  exit(0);
}

