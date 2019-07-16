/*
  idobata_chat.c
*/

#include "mynet.h"
#include "idobata.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>


/////
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#ifdef __CYGWIN__
#include <sys/ioctl.h>
#endif
/////


#define SERVER_LEN 256     /* サーバ名格納用バッファサイズ */
// #define DEFAULT_PORT 50001 /* ポート番号既定値 */
#define NAMELENGTH 15//username 長さ制限
#define BUFSIZE 5
#define R_BUFSIZE 512
#define TIMEOUT_SEC 2

static void action_timeout(int signo);

void argc_error_check(int arg_num,char *argv[],int *port);
void user_name_check(char *input_name,char *name);
void broadcast_enable(int sock);
void signal_handle_set();
void Select_mode(char mode,char *servername,int port_number,char *name);
void send_HELO(int sock,struct sockaddr_in broadcast_adrs);
char check_HERE(int sock,struct sockaddr_in broadcast_adrs);


static void action_received(int signo);
static void show_adrsinfo(struct sockaddr_in *adrs_in);

int main(int argc, char *argv[])
{

  int port_number = PORT;
  char servername[SERVER_LEN] = "localhost";
  char name[NAMELENGTH];
  struct sockaddr_in broadcast_adrs ;
  int sock;
  char mode;


  //引数確認
  argc_error_check(argc,argv,&port_number);

  //ユーザ名確認
  user_name_check(argv[1],name);

  //サーバの情報準備
  set_sockaddr_in(&broadcast_adrs,servername,(in_port_t)port_number);
  sock = init_udpclient();

  // ソケットをブロードキャスト可能にする
  broadcast_enable(sock);

  //シグナルパンドラの設定
  signal_handle_set();

  //HELOパケット送信時のHEREパケットの受信確認
  mode = check_HERE(sock,broadcast_adrs);

  Select_mode(mode,servername,port_number,name);

  close(sock); //プログラム完成時は削除
  exit(EXIT_FAILURE); //プログラム完成時は削除
}


void argc_error_check(int arg_num ,char *argv[],int *port){
  if(arg_num > 3){
	  printf("Number of argument is too many.\n");
	  exit(EXIT_FAILURE);
  } else if(arg_num == 1){
	  printf("Set the username as an argument.\n");
	  exit(EXIT_FAILURE);
  }
  if(arg_num== 3){
  	*port=atoi(argv[2]);
  }
}

void user_name_check(char *input_name,char *name){
  if(strlen(input_name) > NAMELENGTH){
	  printf("error name_length\n");
	  exit(EXIT_FAILURE);
  }
  sprintf(name,"%s",input_name);
}

void broadcast_enable(int sock){
  int broadcast_sw=1;
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
            (void *)&broadcast_sw, sizeof(broadcast_sw)) == -1){
    exit_errmesg("setsockopt()");
  }
}

void signal_handle_set(){
  struct sigaction action;

  action.sa_handler = action_timeout;
  if(sigfillset(&action.sa_mask) == -1){
    exit_errmesg("sigfillset()");
  }
  action.sa_flags = 0;
  if(sigaction(SIGALRM,&action,NULL) == -1){
    exit_errmesg("sigaction()");
  }
}

void Select_mode(char mode,char *servername,int port_number,char *name){
  switch(mode){
  case 'S':
  // {サーバーの関数へ}
    printf("You are Server\n");
    idobata_chat_server(port_number);
    break;
  case 'C':
  // {クライアントの関数へ}
  //idobata_chat_client();
    // idobata_chat_client(servername,port_number,name);

    printf("You are Client\n");
    idobata_chat_client(servername,port_number,name);
    break;
  default:
    printf("switch():error\n");
  }
}

void send_HELO(int sock,struct sockaddr_in broadcast_adrs){
  char HELO_packet[5];
  int strsize;
  create_packet(HELLO,HELO_packet);
  strsize = strlen(HELO_packet);
  Sendto(sock, HELO_packet, strsize, 0,(struct sockaddr *)&broadcast_adrs, sizeof(broadcast_adrs) );
}

char check_HERE(int sock,struct sockaddr_in broadcast_adrs){
  struct sockaddr_in from_adrs;
  socklen_t from_len;
  int strsize;
  int count = 0;
  char r_buf[BUFSIZE];

  //"HELO"パケットを送信
  send_HELO(sock,broadcast_adrs);

  //time out set
  alarm(TIMEOUT_SEC);

  for(;;){
    from_len = sizeof(from_adrs);
    if((strsize=recvfrom(sock,r_buf,BUFSIZE-1,0,(struct sockaddr *)&from_adrs,&from_len)) == -1){
      if(errno == EINTR){
        if(count == 3){
          return 'S';
        }
        count +=1;
        printf("Wait_count:%d\n",count);
//HELOパケット再送
        send_HELO(sock,broadcast_adrs);
//タイマー再設定
        alarm(TIMEOUT_SEC);
      }

      //HEREをチェック
      if(analyze_header(r_buf) == HERE){
        return 'C';
      }
    }
  }
}
static void action_timeout(int signo){
  return;
}
