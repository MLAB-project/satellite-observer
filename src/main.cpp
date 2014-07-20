#include "stdafx.h"

#include <stdio.h>
#include "cTLE.h"
#include "cEci.h"
#include "cOrbit.h"
#include "cSite.h"

#include "time.h"


#include "TLE_param.h"
#include "Satellite.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"

#include "utilities.h"

#include <signal.h>


inline bool exists(const std::string&);


int main(int argc, char *argv[]){

    std::string file;
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

    float frequency;





    //Config file path.
    if(argc>=2){
        file = argv[1];

        if(exists(file) == true){
            std::cout << "Configuration file: " << file <<"\n";
        }
        else{
            std::cout << "Configuration file does not exist.\n";
            return 1;
        }
    }
    //Default conf.
    else{
        file = "/home/carlos/Escritorio/mlab/Resources/conf/default.ini";
        char result[PATH_MAX];
        ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
        std::string path( result, (count > 0) ? count : 0 );
        path=path.substr(0,path.length()-13);
        std::cout << "Fichero:" << path << "\n";
        path.insert(path.length(),"/default.ini");
        file=path;
    }






    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(file, pt);

    if(pt.get<std::string>("TLE.FileSource")=="local"){
        tle_file = pt.get<std::string>("TLE.FileSource");
    }
    if(pt.get<std::string>("TLE.FileSource")=="server"){
        if(pt.get<std::string>("TLE.Server")=="spacetrack"){
            std::string query = "wget --post-data='identity=";

            query.insert(query.length(),pt.get<std::string>("SPACETRACK.User"));
            query.insert(query.length(),"&password=");
            query.insert(query.length(),pt.get<std::string>("SPACETRACK.Pass"));
            query.insert(query.length(),"&query=");

            if(pt.get<std::string>("SPACETRACK.List")=="amateur"){
                query.insert(query.length(),"https://www.space-track.org/basicspacedata/query/class/tle_latest/ORDINAL/1/EPOCH/%3Enow-30/orderby/NORAD_CAT_ID/format/3le/favorites/Amateur'");
            }
            else{
                query.insert(query.length(),"https://www.space-track.org/basicspacedata/query/class/tle_latest/ORDINAL/1/EPOCH/%3Enow-30/orderby/NORAD_CAT_ID/format/3le'");
            }
            query.insert(query.length()," --cookies=on --keep-session-cookies --save-cookies=cookies.txt 'https://www.space-track.org/ajaxauth/login' ");
            query.insert(query.length(),"-O ");
            query.insert(query.length(),pt.get<std::string>("TLE.Local_path"));


            system(query.c_str());
        }
    }


    TLE* tle = new TLE(pt.get<std::string>("TLE.Local_path"));
    //if(pt.get<std::string>("SPACETRACK.List")!="amateur" && pt.get<std::string>("SPACETRACK.List")!="all"){
        satellites=tle->extract(pt.get<std::string>("SPACETRACK.Number"));
    //}
    //else{
        //satellites=tle->extract("z");
    //}

    for(int i=0;i<satellites.size();++i){
        Satellite* s;
        s = satellites[i];

        std::cout << "*****************************************************\n";
        std::cout << "Line0: " << s->line0 << "\n";
        std::cout << "Line1: " << s->line1 << "\n";
        std::cout << "Line2: " << s->line2 << "\n";
        std::cout << "*****************************************************\n";
    }
    frequency = std::stof(pt.get<std::string>("CAPTURE.Frequency"));
    std::cout << "Frequency: " << frequency << "\n";
    std::cout << "*****************************************************\n";


    Satellite* sat;
    sat=satellites[0];

    //Extract TLE parameters
    const Zeptomoby::OrbitTools::cTle tl(sat->line0,sat->line1,sat->line2);

    Zeptomoby::OrbitTools::cOrbit orbit(tl);

   int epochYear = (int)tl.GetField(Zeptomoby::OrbitTools::cTle::FLD_EPOCHYEAR);
   int days= (int)tl.GetField(Zeptomoby::OrbitTools::cTle::FLD_EPOCHDAY );
   double fraction  = (tl.GetField(Zeptomoby::OrbitTools::cTle::FLD_EPOCHDAY)-days);

    //Epoch of Kepler parameters
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
    cout << "Epoch (UTC): " << ctime (&rawtime) << "\n";


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
    std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
    double UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));
    timer=timer-UTC;



    char result2[PATH_MAX];
    ssize_t count2 = readlink( "/proc/self/exe", result2, PATH_MAX );
    std::string path( result2, (count2 > 0) ? count2 : 0 );
    path=path.substr(0,path.length()-13);
    path.insert(path.length(),"/frequency.py 1 111");
    system(path.c_str());

    //system("");
    //PIPE
    int ttt;
    char *pipe = "/tmp/satObPIPE";
    mkfifo(pipe,0666);
    ttt = open(pipe,O_WRONLY);


    int child_frequency = fork();
    switch(child_frequency){
        case -1:
            std::cout << "Error\n";
        break;
        //Child process
        case 0:
            while(true){
                //std::cout << "bucle\n";

                std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
                timer=timer-UTC;

                double m = std::difftime(timer,rawtime);
                m=m/60;

                Zeptomoby::OrbitTools::cEciTime eci2 = orbit.GetPosition(m);

                Zeptomoby::OrbitTools::cJulian j(timer);
                Zeptomoby::OrbitTools::cSite siteEquator(std::atof(pt.get<std::string>("POSITION.Lat").c_str()),std::atof(pt.get<std::string>("POSITION.Long").c_str()),std::atof(pt.get<std::string>("POSITION.Hight").c_str()));
                float velocity_vector[3]={eci2.Velocity().m_x,eci2.Velocity().m_y,eci2.Velocity().m_z};
                float station_vector[3]={siteEquator.GetPosition(j).Position().m_x - eci2.Position().m_x,
                siteEquator.GetPosition(j).Position().m_y - eci2.Position().m_y,
                siteEquator.GetPosition(j).Position().m_z - eci2.Position().m_z};

                Zeptomoby::OrbitTools::cTopo topoLook2 = siteEquator.GetLookAngle(eci2);

                float velocity=sqrt(velocity_vector[0]*velocity_vector[0]+velocity_vector[1]*velocity_vector[1]+velocity_vector[2]*velocity_vector[2]);
                float cos_angle = velocity_vector[0]*station_vector[0]+velocity_vector[1]*station_vector[1]+velocity_vector[2]*station_vector[2];
                float cos_angle1 = sqrt(velocity_vector[0]*velocity_vector[0]+velocity_vector[1]*velocity_vector[1]+velocity_vector[2]*velocity_vector[2]);
                float cos_angle2 = sqrt(station_vector[0]*station_vector[0]+station_vector[1]*station_vector[1]+station_vector[2]*station_vector[2]);
                cos_angle=cos_angle/(cos_angle1*cos_angle2);
                float doppler=(frequency*1000000)*((velocity*1000*cos_angle)/299792458);
                double f=frequency*1000000 + (double)doppler;
                f=f/1000000;
                f=(f-0.01)*2;
                std::string ff=std::to_string(f);


                for(int iii=0;iii<10;++iii){
                    const char number=(const char)ff[iii];
                    write(ttt,&number,sizeof(char));
                }


                //sleep(0.3);


                        //std::cout << setprecision(12);
                        //cout << "Current frequency (with doppler): " << f << "Hz\r" << flush;
            }
            break;
    }






    for(int captures=0;;++captures){
        //Current time
        std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
        //cout << "Current time: " << ctime (&timer) << "\n";

        //Current time to UTC time
        timer=timer-UTC;
        //cout << "Current UTC time: " << ctime (&timer) << "\n";

        //Elapsed time since epoch (seconds)
        double d = std::difftime(timer,rawtime);
        //Elapsed time since epoch (minutes)
        d=d/60;
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
                    std::cout << "\n\n**************  NEXT CAPTURE ************************\n";

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

                    std::cout << "+++ AOS (UTC): " << ctime (&tt_AOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                    ++aos;
                    std::cout << "-----------------------------------------------------\n";
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
                    std::cout << "+++ LOS (UTC): " << ctime (&tt_LOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                    std::cout << "*****************************************************\n";
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
                    std::cout << "\n\n**************  NEXT CAPTURE: NOW  ******************\n";
                    std::cout << "+++ AOS (UTC): " << ctime (&tt_AOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                    ++aos;
                    std::cout << "-----------------------------------------------------\n";
            }
        }
        elevation0=9999;
        aos=0;

        /************ Capture the next pass ************/
        //Create a process to capture the signal
        bool endd=false;
        bool first=false;
        int pid = fork();



        //write(t,"Mensaje entre Procesos",sizeof("Mensaje entre Procesos"));
        //cerramos la tuberia
        //close(t);

        //borramos
        //unlink(tuberia);



        while(endd==false){
            //Current time
            std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
            //Current time to UTC time
            UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));
            timer=timer-UTC;

            switch(pid){
                case -1:
                    std::cout << "Error\n";
                break;
                //Child process
                case 0:
                    //Start capture
                    if(std::difftime(tt_AOS,timer)<=0 && first==false){
                        std::cout << "Catching...\n";
                        std::string record="arecord -D ";

                        record.insert(record.size(),pt.get<std::string>("CAPTURE.Hardware"));
                        record.insert(record.size()," -f dat -t wav -c2 ");
                        record.insert(record.size(),pt.get<std::string>("CAPTURE.Directory"));
                        record.insert(record.size(),"/");
                        record.insert(record.size(),pt.get<std::string>("SPACETRACK.Number"));
                        record.insert(record.size(),"_");
                        std::stringstream cap;
                        cap << captures;
                        record.insert(record.size(),cap.str());
                        record.insert(record.size(),".wav");

                        std::cout << record << "\n";

                        system(record.c_str());

                        first=true;
                    }
                    /*if(std::difftime(tt_AOS,timer)<=0){
                        double m = std::difftime(timer,rawtime);
                        m=m/60;

                        Zeptomoby::OrbitTools::cEciTime eci2 = orbit.GetPosition(m);

                        Zeptomoby::OrbitTools::cJulian j(UTC_timer);
                        float velocity_vector[3]={eci2.Velocity().m_x,eci2.Velocity().m_y,eci2.Velocity().m_z};
                        float station_vector[3]={siteEquator.GetPosition(j).Position().m_x - eci2.Position().m_x,
                        siteEquator.GetPosition(j).Position().m_y - eci2.Position().m_y,
                        siteEquator.GetPosition(j).Position().m_z - eci2.Position().m_z};

                        Zeptomoby::OrbitTools::cTopo topoLook2 = siteEquator.GetLookAngle(eci2);

                        float velocity=sqrt(velocity_vector[0]*velocity_vector[0]+velocity_vector[1]*velocity_vector[1]+velocity_vector[2]*velocity_vector[2]);
                        float cos_angle = velocity_vector[0]*station_vector[0]+velocity_vector[1]*station_vector[1]+velocity_vector[2]*station_vector[2];
                        float cos_angle1 = sqrt(velocity_vector[0]*velocity_vector[0]+velocity_vector[1]*velocity_vector[1]+velocity_vector[2]*velocity_vector[2]);
                        float cos_angle2 = sqrt(station_vector[0]*station_vector[0]+station_vector[1]*station_vector[1]+station_vector[2]*station_vector[2]);
                        cos_angle=cos_angle/(cos_angle1*cos_angle2);
                        float doppler=((frequency/2)+1)*((velocity*1000*cos_angle)/299792458);
                        double f=frequency + (double)doppler;

                        write(t,&f,sizeof(f));


                        std::cout << setprecision(12);
                        cout << "Current frequency (with doppler): " << f << "Hz\r" << flush;

                    }*/
                break;
                //Father process
                default:
                    //End capture
                    if(std::difftime(tt_LOS,timer)<=0){
                        system("ps ax | grep arecord | grep -v grep | awk '{print $1}' | xargs kill");
                        kill (pid, SIGTERM);
                        std::cout << "End of the capture.\n\n\n";
                        endd=true;
                    }
            }
        }
    }

























/*




    std::cout << "\n\n";

    std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
  /*  UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));
    timer=timer-UTC;
    d = std::difftime(timer,rawtime);
    d=d/60;
    double m=d;

    while(true){
        std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
  /*      UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));

        timer=timer-UTC;
        d = std::difftime(timer,rawtime);
        d=d/60;
        m=d;

        Zeptomoby::OrbitTools::cEciTime eci2 = orbit.GetPosition(m);

        Zeptomoby::OrbitTools::cJulian j(UTC_timer);
        float velocity_vector[3]={eci2.Velocity().m_x,eci2.Velocity().m_y,eci2.Velocity().m_z};
        float station_vector[3]={siteEquator.GetPosition(j).Position().m_x - eci2.Position().m_x,
        siteEquator.GetPosition(j).Position().m_y - eci2.Position().m_y,
        siteEquator.GetPosition(j).Position().m_z - eci2.Position().m_z};

        Zeptomoby::OrbitTools::cTopo topoLook2 = siteEquator.GetLookAngle(eci2);


        float velocity=sqrt(velocity_vector[0]*velocity_vector[0]+velocity_vector[1]*velocity_vector[1]+velocity_vector[2]*velocity_vector[2]);

        float cos_angle = velocity_vector[0]*station_vector[0]+velocity_vector[1]*station_vector[1]+velocity_vector[2]*station_vector[2];
        float cos_angle1 = sqrt(velocity_vector[0]*velocity_vector[0]+velocity_vector[1]*velocity_vector[1]+velocity_vector[2]*velocity_vector[2]);
        float cos_angle2 = sqrt(station_vector[0]*station_vector[0]+station_vector[1]*station_vector[1]+station_vector[2]*station_vector[2]);
        cos_angle=cos_angle/(cos_angle1*cos_angle2);
        float doppler=100000000*((velocity*1000*cos_angle)/299792458);

        //std::cout << "Velocity: " << velocity << "\n";


        //std::cout << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";

         std::cout << setprecision(12);
         double f=100000000.0000000000 + (double)doppler;
         cout << "Elevation: " << topoLook2.ElevationDeg() << " " << "Azimuth: "<< topoLook2.AzimuthDeg() << " " << "Current frequency (with doppler) to 100Mhz: " << f << "Hz\r" << flush;
    }
*/


    return 0;
}



inline bool exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}
