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
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


int read_socket (int fd, char *dat, int lng)
{
	int le = 0;
	int Aux = 0;


	if ((fd == -1) || (dat == NULL) || (lng < 1))
		return -1;

	while (le < lng)
	{
		Aux = read (fd, dat + le, lng - le);
		if (Aux > 0)
		{

			le = le + Aux;
		}
		else
		{

			if (Aux == 0)
				return le;
			if (Aux == -1)
			{
				switch (errno)
				{
					case EINTR:
					case EAGAIN:
						usleep (100);
						break;
					default:
						return -1;
				}
			}
		}
	}
	return le;
}


int write_socket(int fd, char *dat, int lng)
{
	int esc = 0;
	int Aux = 0;


	if ((fd == -1) || (dat == NULL) || (lng < 1))
		return -1;


	while (esc < lng)
	{
		Aux = write (fd, dat + esc, lng - esc);
		if (Aux > 0)
		{

			esc = esc + Aux;
		}
		else
		{

			if (Aux == 0)
				return esc;
			else
				return -1;
		}
	}

	return esc;
}
