/**
 * Lidar client
 * Description: UDP client that connects to book server
 * Date: 6/10/2014
 * @author Tim Hodson
 ******************************************************************************/

#include "wrapper.h"
#include "log.h"

/**
 * Prints usage information for program.
 * @param msg Message to print with usage information
 ******************************************************************************/
static void usage (char *msg)
{
  if (msg) 
    safeFputs(msg, stderr);

  fprintf(stderr, "\nUsage: lidar-client <log name>\n");
  exit(1);
}

/**
 * 
 * 
 ********************************************************************/
int main (int argc, char **argv)
{

  /* check for correct usage */
  if (argc == 2) {
    //port = atoi(argv[2]);
  } else if (argc !=2) {
    usage("Invalid arguments");
  }
  // call logger
  logLidar(argv[1]);

  exit(0);
}
