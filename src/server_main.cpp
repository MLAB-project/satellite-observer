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

#include "server_socket.h"
#include "socket.h"
#include "utilities.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

main ()
{
    int port;
    std::vector<Sat*> sat_tracking;

    /********************************************************************************/
    /************************************** PIPE ***********************************/
    /********************************************************************************/
	int ttt; //PIPE
	char *pipe = "/tmp/satObPIPE";

	mkfifo(pipe,0666);

    /********************************************************************************/
    /********************************** SEMAPHORE CAPTURE **************************/
    /********************************************************************************/
    key_t key = ftok ("/bin/ls", 1990);
    if (key == (key_t)-1){
        std::cout << "Fatal error in shared memory. Can not create key.\n";
        return -1;
    }
    int sem_id = semget (key,1,0600 | IPC_CREAT);
    if (sem_id == -1){
        std::cout << "Fatal error in shared memory. Can not create/open semaphore.\n";
        return -1;
    }
	semctl(sem_id,0,SETVAL,1);
    /********************************************************************************/
    /********************************** SEMAPHORE FREQUENCY ************************/
    /********************************************************************************/
    key_t keyf = ftok ("/bin/ls", 1995);
	if (keyf == (key_t)-1)
	{
		std::cout << "Fatal error in shared memory. Can not create key.\n";
        return -1;
	}
    int sem_idf = semget (keyf,1,0600 | IPC_CREAT);
	if (sem_idf == -1)
	{
		std::cout << "Fatal error in shared memory. Can not create/open semaphore.\n";
        return -1;
	}
	semctl(sem_idf,0,SETVAL,1);

    /********************************************************************************/
    /********************************** SHARED MEMORY ******************************/
    /********************************************************************************/
	key_t memory_key = ftok ("/bin/ls", 1968);
	if (memory_key == -1)
	{
        std::cout << "Fatal error in shared memory. Can not create key.\n";
        return -1;
	}
	int memory_id = shmget (memory_key, sizeof(int), 0777 | IPC_CREAT);
	if (memory_id == -1)
	{
        std::cout << "Fatal error in shared memory. Can not create shared memory.\n";
        return -1;
	}
	int* memory = (int *)shmat(memory_id, (char *)0, 0);
	if (memory == NULL)
	{
        std::cout << "Fatal error in shared memory. Can not create shared memory.\n";
        return -1;
	}
	*memory=0;
    /********************************************************************************/
    /********************************** CONFIGURATION ******************************/
    /********************************************************************************/
    char result[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    std::string path( result, (count > 0) ? count : 0 );
    path=path.substr(0,path.length()-7);
    path.insert(path.length(),"/default.ini");

    boost::property_tree::ptree conf;
    boost::property_tree::ini_parser::read_ini(path, conf);
    port=std::atoi(conf.get<std::string>("CONNECTION.Port").c_str());

    /********************************************************************************/
    /********************************** CONNECTION *********************************/
    /********************************************************************************/
	int socket_server;
	int socket_client;

	socket_server = open_inet_socket(port);
	if (socket_server == -1)
	{
		printf ("Can not open server socket.\n");
		exit (-1);
	}


    /********************************************************************************/
    /********************************** SATELLITES RECEPTION ***********************/
    /********************************************************************************/
    while(true){
        std::string satellite_number;
        std::string frequency;
        char str;
        time_t timer;

        socket_client = accept_client_connection (socket_server);
        if (socket_server == -1)
        {
            printf ("Can not open client socket.\n");
            exit (-1);
        }
        for(int i=1;i>0;++i){
            read_socket (socket_client, &str, 1);
            if(str=='|'){
                i=-1;
            }
            else{
                satellite_number=satellite_number.insert(satellite_number.size(),1,str);
            }
        }

        for(int i=1;i>0;++i){
            read_socket (socket_client, &str, 1);
            if(str=='&'){
                i=-1;
            }
            else{
                frequency=frequency.insert(frequency.size(),1,str);
            }
        }
        close (socket_client);

        /********************************************************************************/
        /********************************** SAVE SATELLITES TRACKED ********************/
        /********************************************************************************/
        bool exist_sat=false;
        //If satellite is having tracking only change (or not) the frequency and date
        for(int i = 0; (i< sat_tracking.size()) && (exist_sat==false); i++)
        {
            if(std::atoi(satellite_number.c_str())==sat_tracking[i]->satellite_number ){
                exist_sat=true;
            }
        }

        if(exist_sat==false){
            Sat* p_sat=new Sat();
            p_sat->satellite_number=std::atoi(satellite_number.c_str());
            p_sat->frequency=std::atof(frequency.c_str());
            std::time(&timer);
            p_sat->time=timer;

            /********************************************************************************/
            /********************************** SAT-OBSERVER INSTANCE **********************/
            /********************************************************************************/

            //Create a sat-observer instance.
            int child = fork();
            switch(child){
                case -1:
                    std::cout << "Error: Can not create sat-observer instance.\n";
                break;
                //Child process: sat-observer
                case 0:
                    char result[PATH_MAX];
                    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
                    std::string path( result, (count > 0) ? count : 0 );
                    path=path.substr(0,path.length()-6);
                    path.insert(path.length(),"sat-observer ");
                    path.insert(path.length(),satellite_number);
                    path.insert(path.length()," ");
                    path.insert(path.length(),frequency);
                    system(path.c_str());
                break;
            }
            p_sat->proccess_id=child;
            sat_tracking.push_back(p_sat);
        }

        for(int i = 0; i < sat_tracking.size(); i++)
        {
            char* dt = ctime(&(sat_tracking[i]->time));
            std::cout << "Satellite number: " << sat_tracking[i]->satellite_number << " || Frequency: " << sat_tracking[i]->frequency << " || " << dt <<  "\n";
        }
        std::cout << "\n\n\n";
    }


	close (socket_client);
	close (socket_server);
}
