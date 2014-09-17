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
