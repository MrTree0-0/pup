//
// Created by 金乐盛 on 2018/6/2.
//

#ifndef TRAINANDTICKET_STATIONINFO_H
#define TRAINANDTICKET_STATIONINFO_H
#include "bppair.h"
#include "money.h"
#include "hour_minute.h"
#include "traininfo.h"
using namespace tool;
namespace station_system{
class station_key{
  public:
    mychar station_name;
    mychar train_id;

    station_key(const station_key &s){
        station_name.init(s.station_name.ch);
        train_id.init(s.train_id.ch);
    }
    station_key(const char *name = "", const char *id = ""){
        station_name.init(name);
        train_id.init(id);
    }
    station_key &operator=(const station_key &s){
        station_name.init(s.station_name.ch);
        train_id.init(s.train_id.ch);
        return *this;
    }
};


bool operator==(const station_key &s1, const station_key &s2){
    return (s1.station_name == s2.station_name) && (s1.train_id == s2.train_id);
}

bool operator>(const station_key &s1, const station_key &s2){
    if(s1.station_name > s2.station_name) return true;
    if(s1.station_name < s2.station_name) return false;
    if(s1.train_id > s2.train_id) return true;
    if(s1.train_id < s2.train_id) return false;
    return false;
}

bool operator<(const station_key &s1, const station_key &s2){
    return s2 > s1;
}

bool operator<=(const station_key &s1, const station_key &s2){
    if(s1.station_name < s2.station_name) return 1;
    else return 0;
}

bool operator>=(const station_key &s1, const station_key &s2){
    if(s1.station_name > s2.station_name) return 1;
    else return 0;
}




class station_info{
  public:
    mychar station_name;
    tool::hour_minute arrival_time;
    tool::hour_minute leave_time;
    tool::hour_minute gap_time;
    tool::money money[MAX_TICKET_KIND];

    station_info(const char* name = ""){station_name.init(name);}
    station_info(const mychar &name){station_name.init(name.ch);}
    station_info(const station_info &info){
        station_name.init(info.station_name.ch);
        arrival_time = info.arrival_time;
        leave_time = info.leave_time;
        gap_time = info.gap_time;
        for(int i = 0; i < MAX_TICKET_KIND; i++){
            money[i].price = info.money[i].price;
        }
    }

    station_info &operator=(const station_info &info){
        station_name.init(info.station_name.ch);
        arrival_time = info.arrival_time;
        leave_time = info.leave_time;
        gap_time = info.gap_time;
        for(int i = 0; i < MAX_TICKET_KIND; i++){
            money[i].price = info.money[i].price;
        }
        return *this;
    }
};







};


#endif //TRAINANDTICKET_STATIONINFO_H
