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
