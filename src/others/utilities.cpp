#include "utilities.h"


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
