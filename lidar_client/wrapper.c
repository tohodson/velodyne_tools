/**
 * Description: generic wrapper functions for client and server apps
 * @author Timothy Hodson
 ******************************************************************************/
#include "wrapper.h"

/**
 * Wrapper for sigaction so can call it like the old signal(2)
 * function.
 ******************************************************************************/
Sigfunc *Sigaction(int signo, Sigfunc *func)
{
  struct sigaction NewAction;
  struct sigaction OldAction;

  NewAction.sa_handler = func;
  sigemptyset(&NewAction.sa_mask);
  NewAction.sa_flags = 0;
  if (sigaction(signo, &NewAction, &OldAction) < 0)
    {
      fprintf(stderr, "sigaction(%d,...) failed : %s\n", signo, strerror(errno));
      exit(1);
    }
  return(OldAction.sa_handler);// return any old handler in case caller wants to know
}


/**
 * Safely create a socket
 * @see man(2) socket for params
 * @return socket file descriptor
 ******************************************************************************/
int safeSocket(int domain, int type, int protocol) 
{
  int sockfd;
  errno = 0;
  if ( (sockfd = socket( domain, type, protocol)) < 0 ) {
    perror("socket error");
    exit(1);
  }
  return sockfd;
}

/**
 * Safely connect socket file descriptor to server address. 
 * @see man(2) connect for params
 ******************************************************************************/
void safeConnect( int sockfd, const struct sockaddr *addr,
		 socklen_t addrlen)
{
  errno = 0;
  if (connect( sockfd, addr, addrlen ) < 0 ) {
    perror("connect error"); 
    safeClose(sockfd);
    exit(1);
  }
}

/**
 * Safely call inet_pton, converting a presentation format address to 
 * a network format address.
 * @see man(3) inet_pton for description of params
 ******************************************************************************/
void safeInet_pton(int af, const char *src, void *dst)
{
  errno = 0;
  if (inet_pton(af, src, dst) <= 0)
  {
    if( errno ) {
      perror("inet_pton error: Invalid address family");
      exit(1);

    } else {
      fprintf(stderr, "inet_pton error: Invalid address\n"); 
      exit(1);
    }
      
  }
}

/**
 * Safely read from socket 
 * @see man(2) read for description of params
 ******************************************************************************/
size_t safeRead( int fd, void *buf, size_t count)
{
  int n;
  errno = 0;
  if ( (n = read(fd, buf, count)) < 0 ) {
    perror("read error");
    exit(1); 
  }
  return n;
}

/**
 * Safely bind an address to a socked file descriptor.
 * @param socket created by socket()
 * @param address pointer to sockaddr struct describing the server
 * @see man(2) bind for wrapped function description.
 ******************************************************************************/
void safeBind(int socket, const struct sockaddr *address,
     socklen_t address_len)
{
  errno = 0;
  if ( bind(socket, address, address_len) < -1) {
    perror("bind error"); 
    safeClose(socket);
    exit(1);
  }
}

/**
 * Safely listen
 * @see man(2) listen for wrapped function description.
 ******************************************************************************/
void safeListen(int socket, int backlog)
{
  errno = 0;
  if ( listen(socket, backlog) < -1) {
    perror("listen error");
    exit(1);
  }
}

/**
 * Safely close the socket associated with fildes. 
 * @see man(2) close for wrapped function description.
 ******************************************************************************/
 int safeClose( int fildes )
{
  int n;
  errno = 0;
  if( (n = close(fildes))) {
    perror("close error"); 
    exit(1);
  }
  return n;
}

/**
 * Safely fputs() the string pointed to by s to stream
 * @see man(3) fputs for parameters
 ******************************************************************************/
void safeFputs( const char *s, FILE *stream ) 
{
  int n;
  if( (n = fputs( s, stream )) == EOF ) {
    perror("fputs error"); 
    exit(1);
  }
}

/**
 * Safely check time
 * @param *tloc pointer storing time information
 * @see man(3) time for description of wrapped functions
 ******************************************************************************/
time_t safeTime(time_t *tloc){
  time_t t;
  if( (t = time(tloc)) < 0) {
    safeFputs("time() error", stderr);
    exit(1);
  } 
  else return t;
}

/**
 * Safely writes the string pointed to by buf to a file descriptor
 * @param filedes file descriptor
 * @param *buf buffer containing string to write
 * @param *nbyte number of bytes to write
 * @see man(2) write for description of wrapped function
 ******************************************************************************/
ssize_t safeWrite(int fildes, const void *buf, size_t nbyte)
{
  ssize_t s;
  if ( (s = write(fildes, buf, nbyte) < 0) ) {
    perror("write() error"); 
    exit(1);
  }
  else return s;
}

/**
 * Safely accepts a connection
 * @param socket socket file descriptor
 * @param *address 
 * @param *address_len 
 * @see man(2) accept for description of wrapped function
 ******************************************************************************/
int safeAccept(int socket, struct sockaddr * address, 
	   socklen_t * address_len)
{
  int fd;
  errno = 0;
  if ( (fd = accept(socket, address, address_len)) == -1 ) {
    
    perror("accept() error");
    exit(1);
  }
  return fd;
}

/**
 * Safely unlinks a directory entry
 * @param path path to unlink
 * @see man(2) unlink for description of wrapped function
 ******************************************************************************/
void safeUnlink(const char *path)
{
  errno = 0;
  if ( unlink(path) == -1 && errno != ENOENT) {
    perror("unlink() error");
    exit(1);
  }
}

/**
 * Safely forks process
 * 
 * @see man fork for details
 * @return If parent return child pid, if child return 0
 ******************************************************************************/
int safeFork()
{
  int pid;
  errno = 0;
  if( (pid = fork()) == -1 ) {
    perror("Processes failed to fork"); 
    exit(1); 
  }

  else
    return pid;
}

/**
 * Safely calls waitpid
 * 
 * @see man waitpid for details
 * @return returns pid of the child
 ******************************************************************************/
pid_t
safeWaitpid(pid_t pid, int *stat_loc, int options)
{
  int return_pid;
  if( (return_pid = waitpid(pid, stat_loc, options)) == -1 ) {
    if (errno != ECHILD)
    {
      perror("waitpid() error");
      exit(1);
    }
    return 0;
  }
  else
    return return_pid;
}

/**
 * Safely calls recvfrom
 * 
 * @see man recvfrom for details
 * @return returns the number of bites received 
 ******************************************************************************/
ssize_t
safeRecvfrom(int socket, void *buffer, size_t length, int flags,
	     struct sockaddr *address, socklen_t *address_len)
{
  int n;
  errno = 0;

  if ( ( n = recvfrom(socket, buffer, length, flags, address, 
		      address_len) ) == -1 ) {
    perror("recvfrom() error");
    exit(1);

  } else {
    return n;
  }
}
/**
 * Safely calls sendto
 * 
 * @see man sendto for details
 * @return returns the number of bytes written
 ******************************************************************************/
ssize_t
safeSendto(int socket, const void *buffer, size_t length, int flags,
	   const struct sockaddr *dest_addr, socklen_t dest_len)
{
  int n;
  errno = 0;
  
  if (( n = sendto(socket, buffer, length, flags, dest_addr, dest_len )) == -1 )
  {
    perror("sendto() error");
    exit(1);

  } else {
    return n;
  }

}

/**
 * Safely calls select
 * 
 * @see man select for details
 * @return returns the number of ready file descriptors
 ******************************************************************************/
int
safeSelect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, 
	   struct timeval *timeout)
{
  int n;
  errno = 0;
  if (( n = select( nfds, readfds, writefds, errorfds, timeout)) == -1) {
    if (errno != EINTR) {
      perror("select() error");
      exit(1);
    }

  } 
  
  return n;
}

