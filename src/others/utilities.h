#include <iostream>
#include <vector>
#include <math.h>
#include <linux/limits.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>



void tleEpochToDate(int epochYear,int days,double s,double* date);
void doppler(float satx_0,float saty_0,float satz_0,float satx_1,float saty_1,float satz_1,float satv_0,float satv_1,float sx,float sy,float sz,float f);
void config(std::string name,boost::property_tree::ptree* conf,int binary_size);


class Sat{
    public:
        int proccess_id;

        int satellite_number;
        float frequency;
        time_t time;
};
