#include "wrapper.h"
#include "log.h"

// declare functions here
static int openUDP(int port);

static int max(int num1, int num2)
{
  if ( num1 > num2)
    return num1;
  else
    return num2;
}

/**
   
*******************************************************************************/
int logLidar (char *logname)
{
  printf("Log Name: %s\n", logname);
  int laser_port = 2368; // laser data
  int gps_port = 8308;

  int laserfd, gpsfd; 
  FILE *gpsfile, *laserfile;
  int nready, maxfdp1;
  fd_set rset;
  socklen_t len;
  const int on = 1;

  //open log files
  

  char laser_fname[120];
  char gps_fname[120];
  
  strcpy (gps_fname, logname);
  strcat(gps_fname, ".pos");
  gpsfile = fopen(gps_fname, "w");

  strcpy (laser_fname, logname);
  strcat (laser_fname, ".hdl32");
  laserfile = fopen(laser_fname,"w");

  //open listeners
  laserfd = openUDP(laser_port);
  printf("open laser listener\n");
  gpsfd = openUDP(gps_port);

  FD_ZERO(&rset);
  maxfdp1 = max(laserfd, gpsfd) + 1; // need to impliment max function

  // main logging select loop
  // TODO: implement a check to stop recording

  while (1){

    //check user input to change state
    // add stdin to FD_SET to monitor for user quit
    FD_SET(0, &rset); //stdin
    FD_SET(laserfd, &rset);
    FD_SET(gpsfd, &rset);
    
    if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
      if (errno == EINTR) {
	continue;
      } else {
	fprintf(stderr, "select error: %s\n", strerror(errno));
	exit(1);
      }
    }

    /* if stdin socket has been written to */
    if (FD_ISSET(0, &rset)) {
      int ch;
      if (scanf("%x", &ch) == EOF)
	exit(0);
    }

    /* if lidar socket has been written to */
    if (FD_ISSET(gpsfd, &rset)) {
      logGPS(gpsfd, gpsfile);
    }

    /* if gps socket has been written to */
    if (FD_ISSET(laserfd, &rset)) {
      logLaser(laserfd, laserfile);
    }

  }

  fclose(laserfile);
  fclose(gpsfile);
  return 0;
}

// listener function to create UDP listener socket
static int openUDP(int port)
{
  int udpfd;
  struct sockaddr_in servaddr;   
  udpfd = safeSocket(AF_INET, SOCK_DGRAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_port = htons(port);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  safeBind(udpfd, (struct sockaddr *) &servaddr, sizeof(servaddr) );
  return udpfd;
}


void logGPS(int gpsfd, FILE *gpsfile) { 
  char record[512 + 1];
  bzero( record,  sizeof(record)); // safe to remove this line

  if( recvfrom(gpsfd, record, sizeof(record), 0, NULL, 0) == 512 )
        fwrite(record, 512, 1, gpsfile);
}

void logLaser(int laserfd, FILE *laserfile){
  char record[1206 + 1]; //check length
  bzero( record,  sizeof(record)); 
  
  if (recvfrom(laserfd, record, sizeof(record), 0, NULL, 0) == 1206)
      fwrite(record, 1206, 1, laserfile);
}
