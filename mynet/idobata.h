#ifndef IDOBATA_H_
#define IDOBATA_H_
/*
    idobata.h
*/
#include "mynet.h"

void idobata_chat_server(int port_number);

void idobata_chat_client(char *server_name,int port_number,char *name);

#endif/* IDOBATA_H_ */