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
#include "TLE_param.h"
#include "Satellite.h"
#include "utilities.h"

#include "stdafx.h"
#include "cTLE.h"
#include "cEci.h"
#include "cOrbit.h"
#include "cSite.h"
#include "time.h"

#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

inline bool exists(const std::string&);

int main(int argc, char *argv[]){
    std::string sat_number=argv[1];
    std::string freq=argv[2];
    int intsat=atoi(argv[1]);

    std::string tle_file;
    std::vector<Satellite*> satellites;
    Satellite sat_track;
    double elevation0=9999;
    double azimuth0=9999;
    double elevation1=9999;
    double azimuth1=9999;
    int aos=0;

    double min_AOS;
    double min_LOS;

    float x0;
    float y0;
    float z0;

    float x1;
    float y1;
    float y2;

    float sx;
    float sy;
    float sz;

    float v;
    double frequency=0;


    /********************************************************************************/
    /********************************** SEMAPHORE CAPTURE **************************/
    /********************************************************************************/
    key_t key = ftok ("/bin/ls", 1990);
	if (key == (key_t)-1)
	{
		std::cout << "Fatal error in shared memory. Can not create key.\n";
        return -1;
	}
    int sem_id = semget (key,1,0600 | IPC_CREAT);
	if (sem_id == -1)
	{
		std::cout << "Fatal error in shared memory. Can not create/open semaphore.\n";
        return -1;
	}
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

    /********************************************************************************/
    /********************************** FILE PATH **********************************/
    /********************************************************************************/
    char result[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    std::string path( result, (count > 0) ? count : 0 );
    path=path.substr(0,path.length()-13);
    path.insert(path.length(),"/default.ini");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(path, pt);

    /********************************************************************************/
    /************************** DOWNLOAD TLE FILE ***********************************/
    /********************************************************************************/
    //TLE file is in a local directory
    if(pt.get<std::string>("TLE.FileSource")=="local"){
        //tle_file = pt.get<std::string>("TLE.Local_path");
        std::cout << "ERROR: local file is not accepted.";
        return -1;
    }
    //Donwload a TLE file from a server
    if(pt.get<std::string>("TLE.FileSource")=="server"){
        if(pt.get<std::string>("TLE.Server")=="spacetrack"){
            //wget command
            std::string query = "wget --post-data='identity=";
            query.insert(query.length(),pt.get<std::string>("SPACETRACK.User"));
            query.insert(query.length(),"&password=");
            query.insert(query.length(),pt.get<std::string>("SPACETRACK.Pass"));
            query.insert(query.length(),"&query=");
            query.insert(query.length(),"https://www.space-track.org/basicspacedata/query/class/tle_latest/ORDINAL/1/NORAD_CAT_ID/");
            query.insert(query.length(),sat_number);
            query.insert(query.length(),"/EPOCH/%3Enow-30/orderby/NORAD_CAT_ID/format/3le' 'https://www.space-track.org/ajaxauth/login' -q ");
            query.insert(query.length(),"-O ");
            query.insert(query.length(),pt.get<std::string>("TLE.Local_path"));
            query.insert(query.length(),"/");
            query.insert(query.length(),sat_number);
            system(query.c_str());
        }
    }
    /********************************************************************************/
    /************************** EXTRACT KEPLER PARAMETERS ***************************/
    /********************************************************************************/
    std::string tle_file_name=pt.get<std::string>("TLE.Local_path");
    tle_file_name.insert(tle_file_name.size(),"/");
    tle_file_name.insert(tle_file_name.size(),sat_number);
    std::ifstream f(tle_file_name.c_str(),std::ios::in);
    std::string line0="";
    std::string line1="";
    std::string line2="";

    while(line0==""){
        std::getline(f,line0);
    }
    std::getline(f,line1);
    std::getline(f,line2);

    frequency = std::stod(freq);


    //Extract TLE parameters
    const Zeptomoby::OrbitTools::cTle tl(line0,line1,line2);
    Zeptomoby::OrbitTools::cOrbit orbit(tl);

    //Epoch of Kepler parameters
    int epochYear = (int)tl.GetField(Zeptomoby::OrbitTools::cTle::FLD_EPOCHYEAR);
    int days= (int)tl.GetField(Zeptomoby::OrbitTools::cTle::FLD_EPOCHDAY );
    double fraction  = (tl.GetField(Zeptomoby::OrbitTools::cTle::FLD_EPOCHDAY)-days);

    double date[6];
    tleEpochToDate(epochYear,days,fraction,date);
    struct tm time;
    time.tm_year = date[0] -1900;
    time.tm_mon = date[1]- 1;
    time.tm_mday = date[2];
    time.tm_hour = date[3];
    time.tm_min  = date[4];
    time.tm_sec  = date[5];
    time.tm_isdst = -1;
    time_t rawtime = mktime (&time);

    //AOS time
    struct tm t_AOS;
    t_AOS.tm_year = date[0] -1900;
    t_AOS.tm_mon = date[1]- 1;
    t_AOS.tm_mday = date[2];
    t_AOS.tm_hour = date[3];
    t_AOS.tm_min  = date[4];
    t_AOS.tm_sec  = date[5];
    time.tm_isdst = -1;
    time_t tt_AOS = mktime(&t_AOS);

    //LOS time
    struct tm t_LOS;
    t_LOS.tm_year = date[0] -1900;
    t_LOS.tm_mon = date[1]- 1;
    t_LOS.tm_mday = date[2];
    t_LOS.tm_hour = date[3];
    t_LOS.tm_min  = date[4];
    t_LOS.tm_sec  = date[5];
    time.tm_isdst = -1;
    time_t tt_LOS = mktime(&t_LOS);

    //Current time
    time_t timer;
    struct tm * ptm;
    std::time(&timer);
    double UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));
    timer=timer-UTC;

    /********************************************************************************/
    /************************** FREQUENCY CORRECTION ********************************/
    /********************************************************************************/
    int child_frequency = fork();
    switch(child_frequency){
        case -1:
            std::cout << "Error\n";
        break;
        //Child process
        case 0:
            sleep(1.3);
            int ttt; //PIPE
            char *pipe = "/tmp/satObPIPE";

            mkfifo(pipe,0666);
            ttt = open(pipe,O_WRONLY);
            struct sembuf op;
            while(true){
                /****** Current doppler and frequency *****/
                std::time(&timer); //get current time; same as: timer = time(NULL)
                timer=timer-UTC; //Current local time to current UTC time
                double m = std::difftime(timer,rawtime); //Time elapsed from ephemerids
                m=m/60; //Seconds to minutes*/
                timer=timer+UTC; //Current UTC time to current local time

                Zeptomoby::OrbitTools::cEciTime eci2 = orbit.GetPosition(m);
                Zeptomoby::OrbitTools::cJulian j(timer);
                Zeptomoby::OrbitTools::cSite siteEquator(std::atof(pt.get<std::string>("POSITION.Lat").c_str()),std::atof(pt.get<std::string>("POSITION.Long").c_str()),std::atof(pt.get<std::string>("POSITION.Hight").c_str()));

                double velocity_vector[3]={eci2.Velocity().m_x-siteEquator.GetPosition(j).Velocity().m_x,eci2.Velocity().m_y-siteEquator.GetPosition(j).Velocity().m_y,eci2.Velocity().m_z-siteEquator.GetPosition(j).Velocity().m_z};

                //Vector satelllite-ground
                double station_vector[3]={siteEquator.GetPosition(j).Position().m_x - eci2.Position().m_x,
                    siteEquator.GetPosition(j).Position().m_y - eci2.Position().m_y,
                        siteEquator.GetPosition(j).Position().m_z - eci2.Position().m_z};

                //Unitary vector
                double mod_station = sqrt(station_vector[0]*station_vector[0] + station_vector[1]*station_vector[1] + station_vector[2]*station_vector[2]);
                station_vector[0]=station_vector[0]/mod_station;
                station_vector[1]=station_vector[1]/mod_station;
                station_vector[2]=station_vector[2]/mod_station;

                double v = station_vector[0]*velocity_vector[0] + station_vector[1]*velocity_vector[1] + station_vector[2]*velocity_vector[2];

                double doppler=(frequency*1000000.0)*((1000*v)/299792458.0);
                double f=frequency*1000000.0 + doppler;
                f=f/1000000.0;

                std::cout << setprecision(9);
                //cout << "Doppler: " << doppler << "Hz  Current frequency: " << f<< " Hz\r" << flush;

                /****** Doppler and frequency in the receiver *****/
                std::time(&timer); //get current time; same as: timer = time(NULL)
                timer=timer-UTC; //Current local time to current UTC time
                m = std::difftime(timer,rawtime); //Time elapsed from ephemerids
                m=m+1.47;
                m=m/60; //Seconds to minutes*/
                timer=timer+UTC+1.47; //Current UTC time to current local time

                eci2 = orbit.GetPosition(m);
                Zeptomoby::OrbitTools::cJulian j2(timer);

                double velocity_vector2[3]={eci2.Velocity().m_x-siteEquator.GetPosition(j).Velocity().m_x,eci2.Velocity().m_y-siteEquator.GetPosition(j).Velocity().m_y,eci2.Velocity().m_z-siteEquator.GetPosition(j).Velocity().m_z};

                //Vector satelllite-ground
                double station_vector2[3]={siteEquator.GetPosition(j).Position().m_x - eci2.Position().m_x,
                    siteEquator.GetPosition(j).Position().m_y - eci2.Position().m_y,
                        siteEquator.GetPosition(j).Position().m_z - eci2.Position().m_z};

                //Unitary vector
                double mod_station2 = sqrt(station_vector2[0]*station_vector2[0] + station_vector2[1]*station_vector2[1] + station_vector2[2]*station_vector2[2]);
                station_vector2[0]=station_vector2[0]/mod_station2;
                station_vector2[1]=station_vector2[1]/mod_station2;
                station_vector2[2]=station_vector2[2]/mod_station2;

                v = station_vector2[0]*velocity_vector2[0] + station_vector2[1]*velocity_vector2[1] + station_vector2[2]*velocity_vector2[2];

                doppler=(frequency*1000000.0)*((1000*v)/299792458.0);
                f=frequency*1000000.0 + doppler;
                f=f/1000000.0;
                f=(f-0.01)*2;

                std::string ff=std::to_string(f);


                int sizeff = 14-ff.size();
                if(*memory==intsat){
                    op.sem_num = 0;
                    op.sem_op = -1;
                    op.sem_flg = 0;
                    semop (sem_idf,&op, 1);

                    for(int iii=0;iii<14;++iii){
                        if( (14-(iii+1)) < sizeff ){
                            const char number='0';
                            write(ttt,&number,sizeof(char));
                        }
                        else{
                            const char number=(const char)ff[iii];
                            write(ttt,&number,sizeof(char));
                        }
                    }
                    semctl(sem_idf,0,SETVAL,1);
                }
                usleep(500000);
            }

    }

    /*********************************************************************************/
    /************************** AOS AND LOS TIME ************************************/
    /********************************************************************************/

    for(int captures=0;;++captures){
        std::time(&timer); //Get current time
        timer=timer-UTC; //Current time to UTC time
        double d = std::difftime(timer,rawtime); //Elapsed time since epoch (seconds)
        d=d/60; //Elapsed time since epoch (minutes)
        time_t UTC_timer=timer+UTC;

        int seconds=0;
        Zeptomoby::OrbitTools::cSite siteEquator(std::atof(pt.get<std::string>("POSITION.Lat").c_str()),std::atof(pt.get<std::string>("POSITION.Long").c_str()),std::atof(pt.get<std::string>("POSITION.Hight").c_str()));
        // Calculate the next AOS and LOS
        for (double mpe = d; (mpe <=d+9999999999999999) && (aos!=2); mpe=mpe+0.0166666, ++seconds){
            // Get the position of the satellite at time "mpe"
            Zeptomoby::OrbitTools::cEciTime eci = orbit.GetPosition(mpe);
            Zeptomoby::OrbitTools::cTopo topoLook = siteEquator.GetLookAngle(eci);

            elevation0=elevation1;
            elevation1=topoLook.ElevationDeg();
            azimuth1=topoLook.AzimuthDeg();

            if(elevation0==9999){
                elevation0=elevation1;
                azimuth0=azimuth1;
            }

            if((elevation0<0 && elevation1>0) || (elevation0>0 && elevation1<0)){
                if(elevation0<0 && elevation1>0 && aos==0){
                    //std::cout << "\n\n**************  NEXT CAPTURE ************************\n";

                    t_AOS.tm_year = date[0] -1900;
                    t_AOS.tm_mon = date[1]- 1;
                    t_AOS.tm_mday = date[2];

                    t_AOS.tm_hour = date[3];
                    t_AOS.tm_min  = date[4];
                    t_AOS.tm_sec  = date[5];
                    time.tm_isdst = -1;
                    tt_AOS = mktime(&t_AOS);

                    tt_AOS=tt_AOS+(mpe)*60;
                    tt_AOS=tt_AOS+std::atof(pt.get<std::string>("CAPTURE.Start").c_str());

                    //std::cout << "+++ AOS (UTC): " << ctime (&tt_AOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                    ++aos;
                    //std::cout << "-----------------------------------------------------\n";
                }
                else if(elevation0>0 && elevation1<0){
                    t_LOS.tm_year = date[0] -1900;
                    t_LOS.tm_mon = date[1]- 1;
                    t_LOS.tm_mday = date[2];
                    t_LOS.tm_hour = date[3];
                    t_LOS.tm_min  = date[4];
                    t_LOS.tm_sec  = date[5];
                    time.tm_isdst = -1;
                    tt_LOS = mktime(&t_LOS);
                    tt_LOS=tt_LOS+std::atof(pt.get<std::string>("CAPTURE.End").c_str());

                    tt_LOS=tt_LOS+(mpe)*60;
                    //std::cout << "+++ LOS (UTC): " << ctime (&tt_LOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                    //std::cout << "*****************************************************\n";
                    ++aos;
                }
            }
            //Next capture: NOW
            else if(elevation0>0 && aos==0){
                    t_AOS.tm_year = date[0] -1900;
                    t_AOS.tm_mon = date[1]- 1;
                    t_AOS.tm_mday = date[2];

                    t_AOS.tm_hour = date[3];
                    t_AOS.tm_min  = date[4];
                    t_AOS.tm_sec  = date[5];
                    time.tm_isdst = -1;
                    tt_AOS = mktime(&t_AOS);

                    tt_AOS=tt_AOS+(mpe)*60;
                    //std::cout << "\n\n**************  NEXT CAPTURE: NOW  ******************\n";
                    //std::cout << "+++ AOS (UTC): " << ctime (&tt_AOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                    ++aos;
                    //std::cout << "----------------------------------------------------\n";
            }
        }

        elevation0=9999;
        aos=0;


        /************ Capture the next pass ************/
        //Create a process to capture the signal
        bool endd=false;
        bool first=false;
        struct sembuf op;
        int pid = fork();
        while(endd==false){
            switch(pid){
                case -1:
                    std::cout << "Error\n";
                break;
                //Child process
                case 0:{
                    //Take control of shared memory.
                    op.sem_num = 0;
                    op.sem_op = -1;
                    op.sem_flg = 0;
                    semop (sem_id,&op, 1);

                    //Current time
                    std::time(&timer);
                    //Current time to UTC time
                    UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));
                    timer=timer-UTC;
                    int d1 = std::difftime(tt_AOS,timer);
                    int d2 = std::difftime(tt_LOS,timer);


                    if( d1<0 && d2>0 && first==false && *memory==0){
                        *memory=intsat;

                        timer=timer+UTC;
                        std::cout << "Catching satellite: " << intsat << " --> " << ctime(&timer) << "\n";
                        timer=timer-UTC;

                        std::string record=" nc localhost ";
                        record.insert(record.size(),pt.get<std::string>("CAPTURE.Port"));
                        record.insert(record.size(),"| sox -t f32 -c 2 -r 96000 - ");
                        record.insert(record.size(),pt.get<std::string>("CAPTURE.Directory"));
                        record.insert(record.size(),"/");
                        record.insert(record.size(),sat_number);
                        record.insert(record.size(),"_");
                        std::stringstream cap;
                        cap << captures;
                        record.insert(record.size(),cap.str());
                        record.insert(record.size(),".wav");
                        system(record.c_str());

                        first=true;
                    }
                    if(d2<0){
                        endd=true;
                    }
                semctl(sem_id,0,SETVAL,1);
                break;}
                //Father process
                default:
                    //Current time
                    std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
                    //Current time to UTC time
                    UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));
                    timer=timer-UTC;
                    //End capture
                    int d3 = std::difftime(tt_LOS,timer);
                    if(d3<0 && *memory==intsat){
                        timer=timer+UTC;
                        std::cout << "End: " << intsat << "-->" << ctime(&timer) << "\n";
                        timer=timer-UTC;

						system("ps ax | grep sox | grep -v grep | awk '{print $1}' | xargs kill");
						usleep(500000);
                        /*********************************************************************************/
                        /************************** DECODE NOAA SIGNAL **********************************/
                        /********************************************************************************/
						*memory=0;
                        //NOAA19
                        if(intsat==33591){
							int pidd = fork();
                            switch(pidd){
                                case -1:
                                    std::cout << "Error\n";
                                break;
                                //Child process
                                case 0:
                                    char result[PATH_MAX];
                                    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
                                    std::string cm( result, (count > 0) ? count : 0 );
                                    cm=cm.substr(0,cm.length()-12);
                                    cm.insert(0,"python ");

                                    cm.insert(cm.size(),"decoder.py ");


                                    cm.insert(cm.size(),pt.get<std::string>("CAPTURE.Directory"));
                                    cm.insert(cm.size(),"/");
                                    cm.insert(cm.size(),sat_number);
                                    cm.insert(cm.size(),"_");
                                    std::stringstream cap;
                                    cap << captures;
                                    cm.insert(cm.size(),cap.str());
                                    cm.insert(cm.size(),".wav");

                                    cm.insert(cm.size()," ");

                                    cm.insert(cm.size(),pt.get<std::string>("CAPTURE.Directory"));
                                    cm.insert(cm.size(),"/");
                                    cm.insert(cm.size(),sat_number);
                                    cm.insert(cm.size(),"_");
                                    cm.insert(cm.size(),cap.str());
                                    cm.insert(cm.size(),"dec.wav");

                                    system(cm.c_str());
                                break;
                            }
                        }
                        kill (pid, SIGTERM);
                        endd=true;
                    }
                    break;
            }
        }
    }

    return 0;
}



inline bool exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}
