#include    "TLE_param.h"


/************************** TLE *****************************/
TLE::TLE(std::string file){
    this->file=file;
}

void TLE::second_line(std::string line){
        std::string param;
        int position1=0;
        int position2;
        int position3;
        bool lock=true;

        position2=line.find(" ",position1);
        param=line.substr(position1,position2-position1);
        this->line2[0]=param;

        for(int i=1;i<8;++i){
            position1=position2+1;
            position2=line.find(" ",position1);
            while(lock==true){
                position3=line.find(" ",position2+1);
                if(position2+1==position3){
                    position2=position3;
                }
                else{
                    lock=false;
                }
            }
            lock=true;
            param=line.substr(position1,position2-position1);
            this->line2[i]=param;
        }
}


std::vector<Satellite*> TLE::extract(std::string number){
    std::ifstream f(file.c_str(),std::ios::in);
    std::ifstream input;
    std::string line0;
    std::string line1;
    std::string line2;

    std::vector<Satellite*> satellites;
    int cnt=0;

    //Zero line
    std::getline(f,line0);
    while(!f.eof() && line0!="" && line0!=" " && line0!="\n"){
        //First line
        std::getline(f,line1);

        //Second line
        std::getline(f,line2);
        second_line(line2);

        if(this->line2[1]==number || number=="z"){
            Satellite* sat = new Satellite();
            sat->line0=line0;
            sat->line1=line1;
            sat->line2=line2;

            satellites.push_back(sat);
        }
        std::getline(f,line0);
    }
    return satellites;
}



