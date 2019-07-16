/*
  mynet.h
*/
#ifndef MYNET_H_
#define MYNET_H_


#define HELLO   1
#define HERE    2
#define JOIN    3
#define POST    4
#define MESSAGE 5
#define QUIT    6
#define PORT    50001

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

//init_tcpserver.c
int init_tcpserver(in_port_t myport, int backlog);
//init_tcpclient.c
int init_tcpclient(char *servername, in_port_t serverport);
//init_udpserver.c
int init_udpserver(in_port_t myport);
//init_udpclient.c
int init_udpclient();
void set_sockaddr_in(struct sockaddr_in *server_adrs,
		     char *servername, in_port_t port_number );
void set_sockaddr_in_broadcast(struct sockaddr_in *server_adrs,
		               in_port_t port_number );
//other.c
void exit_errmesg(char *errmesg);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int Send(int s, void *buf, size_t len, int flags);
int Recv(int s, void *buf, size_t len, int flags);
int Sendto( int sock, const void *s_buf, size_t strsize, int flags, const struct sockaddr *to, socklen_t tolen);
int Recvfrom(int sock, void *r_buf, size_t len, int flags,
       struct sockaddr *from, socklen_t *fromlen);
int analyze_header( char *header );
void create_packet(int type, char *message );

/* メッセージ情報管理 */
struct  idobata_packet{
  char header[5];   /* パケットのヘッダ部(4バイト) */
  char sep;         /* セパレータ(空白、またはゼロ) */
  char data[];      /* データ部分(メッセージ本体) */
};

#endif  /* MYNET_H_ */
