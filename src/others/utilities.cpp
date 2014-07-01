#include "utilities.h"


void doppler(float satx_0,float saty_0,float satz_0,float satx_1,float saty_1,float satz_1,float satv_0,float satv_1,float sx,float sy,float sz,float f){
//(x_0,y_0,z_0) Satellite position in t=t_0
//(x_1,y_1,z_1) Satellite position in t=t_1
//doppler=f*(v/c)
//v=satellite relative velocity
//v=

float psat[3]={satx_1-satx_0,saty_1-saty_0,satx_1-satx_0}; //Unitary vector P0->P1
float p[3]={sx-satx_0,sy-saty_0,sz-satz_0}; //Unitary vector P0->station
float angle = psat[1]*p[1]+psat[2]*p[2]+psat[3]*p[3];
float angle1 = sqrt(psat[1]*psat[1]+psat[2]*psat[2]+psat[3]*psat[3]);
float angle2 = sqrt(p[1]*p[1]+p[2]*p[2]+p[3]*p[3]);

angle=angle/(angle1*angle2);

float doppler=f*((satv_0*angle)/299792458);

std::cout << "Doppler: " << doppler << "\n";
}







void tleEpochToDate(int epochYear,int days,double s,double* date){
    double secondsofday=86400*s;

    int epochMonth=0;
    int epochDay=0;

    int addDays;

    int epochHour=0;
    int epochMinutes=0;
    int epochSeconds=0;


    if (epochYear<57){
        epochYear+=2000;
    }
    else{
        epochYear+=1900;
    }


    if(days>0){
        days=days-31;
        addDays=31;
        ++epochMonth;
    }
    if(days>0){
        if((epochYear % 4 == 0 && epochYear % 100 != 0) || epochYear % 400 == 0){
            days=days-29;
            addDays=29;
        }
        else{
            days=days-28;
            addDays=28;
        }
        ++epochMonth;
    }
    if(days>0){
        days=days-31;
        addDays=31;
        ++epochMonth;
    }
    if(days>0){
        days=days-30;
        addDays=30;
        ++epochMonth;
    }
    if(days>0){
        days=days-31;
        addDays=31;
        ++epochMonth;
    }
    if(days>0){
        days=days-31;
        addDays=31;
        ++epochMonth;
    }
    if(days>0){
        days=days-30;
        addDays=30;
        ++epochMonth;
    }
    if(days>0){
        days=days-31;
        addDays=31;
        ++epochMonth;
    }
    if(days>0){
        days=days-30;
        addDays=30;
        ++epochMonth;
    }
    if(days>0){
        days=days-31;
        addDays=31;
        ++epochMonth;
    }
    epochDay=days+addDays;

   for(;secondsofday>0;secondsofday=secondsofday-3600){
        ++epochHour;
   }
   --epochHour;

   secondsofday=secondsofday+3600;
   for(;secondsofday>0;secondsofday=secondsofday-60){
        ++epochMinutes;
   }
   --epochMinutes;
   secondsofday=secondsofday+60;

   for(;secondsofday>0;secondsofday=secondsofday-1){
        ++epochSeconds;
   }
   --epochSeconds;

    date[0]=epochYear;
    date[1]=epochMonth;
    date[2]=epochDay;
    date[3]=epochHour;
    date[4]=epochMinutes;
    date[5]=epochSeconds;
}
