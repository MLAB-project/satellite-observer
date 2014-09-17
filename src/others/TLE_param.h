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

#ifndef  TLE_PARAM_INC
#define  TLE_PARAM_INC

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include "Satellite.h"



class TLE{
    public:
        std::string file;
        std::string line0;
        std::string line1[9];
        std::string line2[8];

        TLE(std::string file);
        void second_line(std::string line);

        double string_to_long_double(std::string str,int dec);

        std::vector<Satellite*> extract(std::string number);
};




#endif
