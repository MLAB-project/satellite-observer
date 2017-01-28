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

#include <stdio.h>
#include "client_socket.h"
#include "socket.h"
#include <stdlib.h>
#include <string.h>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/detail/throw_error.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "utilities.h"


int main(int argc, char *argv[]){
    if(argc!=5){
        std::cout << "Error. Use: ./client IP port satellite_number frequency.\n";
        return -1;
    }

    std::string IP=argv[1];
    std::string port=argv[2];
    std::string satellite_number=argv[3];
    std::string frequency=argv[4];

    /********************************************************************************/
    /********************************** CHECKS FORMAT ******************************/
    /********************************************************************************/
    boost::system::error_code ec;
    boost::asio::ip::address::from_string( IP, ec );
    if ( ec ){
        std::cout << "Error: invalid IP format.\n";
        return -1;
    }

    if(std::stoi(port)>65535 or std::stoi(port)<0){
        std::cout << "Error: port out of range.\n";
        return -1;
    }

    if(std::stoi(satellite_number)>99999 or std::stoi(satellite_number)<0){
        std::cout << "Error: satellite number out of range.\n";
        return -1;
    }

    /********************************************************************************/
    /********************************** SERVER CONNECTION **************************/
    /********************************************************************************/
	/*
	* Descriptor and buffer
	*/
	int socket_with_server;
	char str[100];

	/*
	* Open connection with server
	*/
	socket_with_server = open_inet_connection(IP,std::stoi(port));
	if (socket_with_server == -1)
	{
		printf ("Can not connect.\n");
		exit (-1);
	}

    /********************************************************************************/
    /********************************** SEND PARAMETERS ****************************/
    /********************************************************************************/
    //message = satellite_number|frequency&
    std::string message=satellite_number;
    message.insert(message.size(),"|");
    message.insert(message.size(),frequency);
    message.insert(message.size(),"&");

	strcpy (str, message.c_str());
	write_socket(socket_with_server, str, message.size());

	close (socket_with_server);
}
