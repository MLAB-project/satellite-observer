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

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"

#include "utilities.h"


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
    Satellite* sat;
    sat=satellites[0];

    //Extract TLE parameters
    const Zeptomoby::OrbitTools::cTle tl(sat->line0,sat->line1,sat->line2);

    Zeptomoby::OrbitTools::cOrbit orbit(tl);
    std::vector<Zeptomoby::OrbitTools::cEci> Pos;

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


    time_t timer;
    struct tm * ptm;
    std::time(&timer);  /* get current time; same as: timer = time(NULL)  */
    double UTC=3600*(std::atof(pt.get<std::string>("TIME.Hour").c_str()));

    cout << "Current time: " << ctime (&timer) << "\n";
    timer=timer-UTC;
    cout << "Current UTC time: " << ctime (&timer) << "\n";
    double d = std::difftime(timer,rawtime);
    d=d/60;



    struct tm t_AOS;
    t_AOS.tm_year = date[0] -1900;
    t_AOS.tm_mon = date[1]- 1;
    t_AOS.tm_mday = date[2];
    t_AOS.tm_hour = date[3];
    t_AOS.tm_min  = date[4];
    t_AOS.tm_sec  = date[5];
    time.tm_isdst = -1;
    time_t tt_AOS = mktime(&t_AOS);


    struct tm t_LOS;
    t_LOS.tm_year = date[0] -1900;
    t_LOS.tm_mon = date[1]- 1;
    t_LOS.tm_mday = date[2];
    t_LOS.tm_hour = date[3];
    t_LOS.tm_min  = date[4];
    t_LOS.tm_sec  = date[5];
    time.tm_isdst = -1;
    time_t tt_LOS = mktime(&t_LOS);


    // Calculate position, velocity
    for (double mpe = d; (mpe <=d+9999999999999999) && (aos!=8); mpe=mpe+0.0016){
        // Get the position of the satellite at time "mpe"
        Zeptomoby::OrbitTools::cEciTime eci = orbit.GetPosition(mpe);
        Zeptomoby::OrbitTools::cSite siteEquator(std::atof(pt.get<std::string>("POSITION.Lat").c_str()),std::atof(pt.get<std::string>("POSITION.Long").c_str()),std::atof(pt.get<std::string>("POSITION.Hight").c_str())); // 0.00 N, 100.00 W, 0 km altitude
        Zeptomoby::OrbitTools::cTopo topoLook = siteEquator.GetLookAngle(eci);

        elevation0=elevation1;
        elevation1=topoLook.ElevationDeg();
        azimuth1=topoLook.AzimuthDeg();
        //std::cout << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
        //std::cout << "Mpe " << mpe-d <<"\n";

        if(elevation0==9999){
            elevation0=elevation1;
            azimuth0=azimuth1;
        }

        if((elevation0<0 && elevation1>0) || (elevation0>0 && elevation1<0)){
            if(elevation0<0 && elevation1>0){
                std::cout << "\n\n*****************************************************\n";
                t_AOS.tm_year = date[0] -1900;
                t_AOS.tm_mon = date[1]- 1;
                t_AOS.tm_mday = date[2];
                t_AOS.tm_hour = date[3];
                t_AOS.tm_min  = date[4];
                t_AOS.tm_sec  = date[5];
                time.tm_isdst = -1;
                tt_AOS = mktime(&t_AOS);

                tt_AOS=tt_AOS+(mpe-20)*60+UTC;
                std::cout << "+++ AOS: " << ctime (&tt_AOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                ++aos;
                std::cout << "-----------------------------------------------------\n";
            }
            else{
                t_LOS.tm_year = date[0] -1900;
                t_LOS.tm_mon = date[1]- 1;
                t_LOS.tm_mday = date[2];
                t_LOS.tm_hour = date[3];
                t_LOS.tm_min  = date[4];
                t_LOS.tm_sec  = date[5];
                time.tm_isdst = -1;
                tt_LOS = mktime(&t_LOS);

                tt_LOS=tt_LOS+(mpe-20)*60+UTC;
                std::cout << "+++ LOS: " << ctime (&tt_LOS) << "||||" << "Elevation: " << topoLook.ElevationDeg() << " " << "Azimuth: "<< topoLook.AzimuthDeg() <<"\n";
                std::cout << "*****************************************************\n";
                ++aos;
            }
        }
   }


    return 0;
}



inline bool exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}
