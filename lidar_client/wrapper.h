/*********************************************************************
 * Assignment 6: 
 * Wrapper function declarations for unix greetings client and server
 * @author Timothy Hodson
 ********************************************************************/
#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>



#define MAXLINE 4096
#define SERV_PORT 9880

typedef void Sigfunc(int);// used in Sigaction for readability

Sigfunc *Sigaction(int signo, Sigfunc *func);

int safeSocket( int domain, int type, int protocol);

void safeConnect( int sockfd, const struct sockaddr *addr,
		  socklen_t addrlen);

void safeInet_pton( int af, const char *src, void *dst);

void safeBind( int socket, const struct sockaddr *address,
	       socklen_t address_len);

void safeListen( int socket, int backlog);

size_t safeRead( int fd, void *buf, size_t count);

int safeClose( int fildes );

void safeFputs( const char *s, FILE *stream );

time_t safeTime( time_t *t_loc );

ssize_t safeWrite( int fildes, const void *buf, size_t nbyte );

int safeAccept( int socket, struct sockaddr * address, 
		socklen_t * address_len );

void safeUnlink( const char *path );


int safeFork();

pid_t
safeWaitpid(pid_t pid, int *stat_loc, int options);

ssize_t
safeRecvfrom(int socket, void *buffer, size_t length, int flags,
	     struct sockaddr *address, socklen_t *address_len);

ssize_t
safeSendto(int socket, const void *buffer, size_t length, int flags,
       const struct sockaddr *dest_addr, socklen_t dest_len);

int
safeSelect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, 
	   struct timeval *timeout);

#endif 
