#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


int open_unix_connection(char *service)
{
	struct sockaddr_un direction;
	int descriptor;

	strcpy (direction.sun_path, service);
	direction.sun_family = AF_UNIX;

	descriptor = socket (AF_UNIX, SOCK_STREAM, 0);
	if (descriptor == -1)
		return -1;


	if (connect (
			descriptor,
			(struct sockaddr *)&direction,
			strlen (direction.sun_path) + sizeof (direction.sun_family)) == -1)
	{
		return -1;
	}
	else
	{
		return descriptor;
	}
}


int open_inet_connection (
	std::string IP,
	int port)
{
	struct sockaddr_in direction;
	int descriptor;

	direction.sin_family = AF_INET;
	direction.sin_addr.s_addr = inet_addr(IP.c_str());
	direction.sin_port = htons(port);


	descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (descriptor == -1)
		return -1;

	if (connect (
			descriptor,
			(struct sockaddr *)&direction,
			sizeof (direction)) == -1)
	{
		return -1;
	}

	return descriptor;
}
