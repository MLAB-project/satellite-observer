#ifndef _SOCKET_SERVIDOR_H
#define _SOCKET_SERVIDOR_H

int open_inet_socket (int port);
int open_unix_socket (char *service);
int accept_client_connection (int descriptor);

#endif
