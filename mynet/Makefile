#
# Makefile for tcp_echo
#

MYLIBDIR=.
MYLIB=-lmynet
CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}

all: idobata.exe

idobata.exe: init_tcpserver.o init_udpserver.o init_tcpclient.o init_udpclient.o idobata_chat.o idobata_chat_server.o idobata_chat_client.o other.o
	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}


clean:
	${RM} *.o idobata.exe *~
