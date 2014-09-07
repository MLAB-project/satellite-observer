#ifndef _SOCKET_CLIENTE_H
#define _SOCKET_CLIENTE_H

#include <string>
#include <iostream>

int open_unix_connection(char *service);
int open_inet_connection (std::string IP, int port);

#endif
