//
// Created by 金乐盛 on 2018/6/7.
//

#ifndef TRAIN_TRAIN_STATION_H
#define TRAIN_TRAIN_STATION_H
#include "bppair.h"
#include "traininfo.h"
#include "stationinfo.h"
#include "hour_minute.h"
namespace train_station{
class train_station{
  public:
    train_system::train_info train_info;
    station_system::station_info station_info[MAX_STATION_NUM];


    train_station(){}
    train_station(const train_station &ts){
        train_info = ts.train_info;
        for(int i = 0; i < MAX_STATION_NUM; i++){
            station_info[i] = ts.station_info[i];
        }
    }

    train_station &operator=(const train_station &ts){
        train_info = ts.train_info;
        for(int i = 0; i < MAX_STATION_NUM; i++){
            station_info[i] = ts.station_info[i];
        }
        return *this;
    }

};

std::ostream &operator<<(std::ostream &out, train_station &ts){
    out << ts.train_info.train_id << " " << ts.train_info.train_name << " " <<  ts.train_info.train_catalog
        << " " << ts.train_info.num_station << " " << ts.train_info.num_ticket;
    for(int i = 0; i < ts.train_info.num_ticket; i++){
        out << " " << ts.train_info.ticket_name[i];
    }
    out << std::endl;
    for(int i = 0; i < ts.train_info.num_station; i++){
        out << ts.station_info[i].station_name << " " << ts.station_info[i].arrival_time << " " << ts.station_info[i].leave_time << " " << ts.station_info[i].gap_time;
        for(int j = 0; j < ts.train_info.num_ticket; j++){
            out << " ￥" << ts.station_info[i].money[j];
        }
        out << std::endl;
    }
    return out;
}

std::istream &operator>>(std::istream &in, train_station &ts){
    in >> ts.train_info.train_id >> ts.train_info.train_name >> ts.train_info.train_catalog >> ts.train_info.num_station >> ts.train_info.num_ticket;
    for(int i = 0; i < ts.train_info.num_ticket; i++){
        in >> ts.train_info.ticket_name[i];
    }
    for(int i = 0; i < ts.train_info.num_station; i++){
        in >> ts.station_info[i].station_name >> ts.station_info[i].arrival_time >> ts.station_info[i].leave_time >> ts.station_info[i].gap_time;
        for(int j = 0; j < ts.train_info.num_ticket; j++){
            char ch;
            in >> ch >> ch >> ch >> ts.station_info[i].money[j];
        }
    }
    return in;
}
}
#endif //TRAIN_TRAIN_STATION_H
