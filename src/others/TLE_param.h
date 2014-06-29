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
