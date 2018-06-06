//
// Created by 金乐盛 on 2018/6/2.
//

#ifndef TRAINANDTICKET_STATIONINFO_H
#define TRAINANDTICKET_STATIONINFO_H
#include "bppair.h"
using namespace tool;
namespace station_system{
    class station_train{
      public:
        mychar station_name;
        mychar train_id;

        station_train(const station_train &s):station_name(s.station_name.ch), train_id(s.train_id.ch){}
        station_train(const char *name = "", const char *id = ""):station_name(name), train_id(id){}
    };

bool operator==(const station_train &s1, const station_train &s2){
    return (s1.station_name == s2.station_name) && (s1.train_id == s2.train_id);
}

bool operator>(const station_train &s1, const station_train &s2){
    if(s1.station_name > s2.station_name) return true;
    if(s1.station_name < s2.station_name) return false;
    if(s1.train_id > s2.train_id) return true;
    if(s1.train_id < s2.train_id) return false;
    return false;
}

bool operator<(const station_train &s1, const station_train &s2){
    return s2 > s1;
}

bool operator<=(const station_train &s1, const station_train &s2){
    if(s1.station_name < s2.station_name) return 1;
    else return 0;
}

bool operator>=(const station_train &s1, const station_train &s2){
    if(s1.station_name > s2.station_name) return 1;
    else return 0;
}
};


#endif //TRAINANDTICKET_STATIONINFO_H
