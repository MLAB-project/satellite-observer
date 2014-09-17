/* 	Satellite Observer. 
*	The main objective of Satellite Observer is the autonomous capture 
*	of signal from small satellites and the subsequent 
*	decoding. It  keeps track of the trajectory, continuously updated of 
*	Doppler shift and tunes the receiver.
*	
*	Copyright (C) 2014  Carlos Alberto Ruiz Naranjo
*	carlosruiznaranjo@gmail.com
*	
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU Affero General Public License as
*	published by the Free Software Foundation, either version 3 of the
*	License, or (at your option) any later version.

*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU Affero General Public License for more details.
*	
*	You should have received a copy of the GNU Affero General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>


int open_unix_socket (char *service)
{
	struct sockaddr_un direction;
	int descriptor;


	descriptor = socket (AF_UNIX, SOCK_STREAM, 0);
	if (descriptor == -1)
	 	return -1;


	strcpy (direction.sun_path, service);
	direction.sun_family = AF_UNIX;

	if (bind (
			descriptor,
			(struct sockaddr *)&direction,
			strlen (direction.sun_path) + sizeof (direction.sun_family)) == -1)
	{
		close (descriptor);
		return -1;
	}


	if (listen (descriptor, 1) == -1)
	{
		close (descriptor);
		return -1;
	}


	return descriptor;
}


int accept_client_connection (int Descriptor)
{
	socklen_t client_long;
	struct sockaddr client;
	int child;


	client_long = sizeof (client);
	child = accept (Descriptor, &client, &client_long);
	if (child == -1)
		return -1;

	return child;
}


int open_inet_socket (int port)
{
	struct sockaddr_in direction;
	struct sockaddr client;
	socklen_t client_long;
	int descriptor;


	descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (descriptor == -1)
	 	return -1;

	direction.sin_family = AF_INET;
	direction.sin_port = htons(port);
	direction.sin_addr.s_addr =INADDR_ANY;
	if (bind (
			descriptor,
			(struct sockaddr *)&direction,
			sizeof (direction)) == -1)
	{
		close (descriptor);
		return -1;
	}


	if (listen (descriptor, 1) == -1)
	{
		close (descriptor);
		return -1;
	}


	return descriptor;
}
