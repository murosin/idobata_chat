#include "mynet.h"
#include "string.h"


// #define HELLO   1
// #define HERE    2
// #define JOIN    3
// #define POST    4
// #define MESSAGE 5
// #define QUIT    6

#define MSGBUF_SIZE 512

static char Buffer[MSGBUF_SIZE];

void exit_errmesg(char *errmesg)
{
  perror(errmesg);
  exit(EXIT_FAILURE);
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
  int r;
  if((r=accept(s,addr,addrlen))== -1){
    exit_errmesg("accept()");
  }
  return(r);
}

int Send(int s, void *buf, size_t len, int flags)
{
  int r;
  if((r=send(s,buf,len,flags))== -1){
    exit_errmesg("send()");
  }
  return(r);
}

int Recv(int s, void *buf, size_t len, int flags)
{
  int r;
  if((r=recv(s,buf,len,flags))== -1){
    exit_errmesg("recv()");
  }
  return(r);
}
int Sendto( int sock, const void *s_buf, size_t strsize, int flags, const struct sockaddr *to, socklen_t tolen)
{
  int r;
  if( (r=sendto(sock, s_buf, strsize, 0, to, tolen))== -1){
    exit_errmesg("sendto()");
  }

  return(r);
}

int Recvfrom(int sock, void *r_buf, size_t len, int flags,
       struct sockaddr *from, socklen_t *fromlen)
{
  int r;
  if((r=recvfrom(sock, r_buf, len, 0, from, fromlen))== -1){
    exit_errmesg("recvfrom()");
  }

  return(r);
}

int analyze_header( char *header )
{
  if( strncmp( header, "HELO", 4 )==0 ) return(HELLO);
  if( strncmp( header, "HERE", 4 )==0 ) return(HERE);
  if( strncmp( header, "JOIN", 4 )==0 ) return(JOIN);
  if( strncmp( header, "POST", 4 )==0 ) return(POST);
  if( strncmp( header, "MESG", 4 )==0 ) return(MESSAGE);
  if( strncmp( header, "QUIT", 4 )==0 ) return(QUIT);
  return 0;
}

void create_packet(int type, char *message )
{

  char *add_header;

  switch( type ){
  case HELLO:
    snprintf( message,5, "HELO");
    break;
    // snprintf( Buffer, MSGBUF_SIZE, "HELO" );
    // break;
  case HERE:
    snprintf( message,5, "HERE");
    break;
    // snprintf( Buffer, MSGBUF_SIZE, "HERE" );
    // break;
  case JOIN:
    add_header = (char *)malloc((strlen(message)+6) * sizeof(char));
    strcpy(add_header,"JOIN ");
    strncat(add_header,message,strlen(message));
    snprintf( message, MSGBUF_SIZE , "%s",add_header);
    free(add_header);
    break;

    // snprintf( Buffer, MSGBUF_SIZE, "JOIN %s", message );
    // break;
  case POST:
    add_header = (char *)malloc((strlen(message)+6) * sizeof(char));
    strcpy(add_header,"POST ");
    strncat(add_header,message,strlen(message));
    snprintf( message, MSGBUF_SIZE , "%s",add_header);
    free(add_header);
    break;

    // snprintf( Buffer, MSGBUF_SIZE, "POST %s", message );
    // break;
  case MESSAGE:
    add_header = (char *)malloc((strlen(message)+6) * sizeof(char));
    strcpy(add_header,"MESG ");
    strncat(add_header,message,strlen(message));
    snprintf( message, MSGBUF_SIZE , "%s",add_header);
    free(add_header);
    break;

    // snprintf( Buffer, MSGBUF_SIZE, "MESG %s", message );
    // break;
  case QUIT:
    snprintf( message, MSGBUF_SIZE , "QUIT");
    break;

    // snprintf( Buffer, MSGBUF_SIZE, "QUIT" );
    // break;
  default:
    /* Undefined packet type */
    break;
  }
}
