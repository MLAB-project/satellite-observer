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
#ifndef  SATELLITE_INC
#define  SATELLITE_INC

#include <string>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <vector>


class Satellite {
    public:
    std::string number;
    std::string line0;
    std::string line1;
    std::string line2;
};


#endif
