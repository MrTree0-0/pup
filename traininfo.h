//
// Created by 金乐盛 on 2018/6/1.
//

#ifndef TRAINANDTICKET_TRAININFO_H
#define TRAINANDTICKET_TRAININFO_H
#define MAX_STATION_NUM 60
#define MAX_TICKET_KIND 5
#include <utility>
#include "bppair.h"
#include "hour_minute.h"
#include "money.h"
#include "stationinfo.h"
using namespace bpt;
namespace train_system{
/*class train_station{
  public:
    int num_of_ticket[MAX_TICKET_KIND];
    train_station()
    {
        for(int j = 0; j < MAX_TICKET_KIND; j++){
            num_of_ticket[j] = 2000;
        }
    }
};*/

class train_info{
  public:
    mychar train_id;
    mychar train_name;
    mychar train_catalog;
    bool sell_or_not;
    int num_station;
    int num_ticket;
    mychar ticket_name[MAX_TICKET_KIND];
    int ticket_num[MAX_STATION_NUM][MAX_TICKET_KIND];
    long offset_stationinfo;


    explicit train_info(const char *id = "", const char *name = "", const char* catalog = "", int station = 0, int ticket = 0, long offset = 0)
        :train_id(id), train_name(name), train_catalog(catalog), num_station(station), num_ticket(ticket), offset_stationinfo(offset){
        sell_or_not = false;
        for(int i = 0; i < MAX_STATION_NUM; i++){
            for(int j = 0; j < MAX_TICKET_KIND; j++){
                ticket_num[i][j] = 2000;
            }
        }
    }

    train_info(const train_info &info0){
        train_id.init(info0.train_id.ch);
        train_name.init(info0.train_name.ch);
        train_catalog.init(info0.train_catalog.ch);
        sell_or_not = info0.sell_or_not;
        num_station = info0.num_station;
        num_ticket = info0.num_ticket;
        offset_stationinfo = info0.offset_stationinfo;
        for(int i = 0; i < info0.num_ticket; i++){
            ticket_name[i].init(info0.ticket_name[i].ch);
        }
        for(int i = 0; i < info0.num_station; i++){
            /*trainStation[i].station_name.init(info0.trainStation[i].station_name.ch);
            trainStation[i].arrival_time = info0.trainStation[i].arrival_time;
            trainStation[i].leave_time = info0.trainStation[i].leave_time;
            trainStation[i].gap_time = info0.trainStation[i].gap_time;*/
            for(int j = 0; j < info0.num_ticket; j++){
                ticket_num[i][j] = info0.ticket_num[i][j];
            }
        }
    }

    train_info &operator=(const train_info &info0){
        train_id.init(info0.train_id.ch);
        train_name.init(info0.train_name.ch);
        train_catalog.init(info0.train_catalog.ch);
        sell_or_not = info0.sell_or_not;
        num_station = info0.num_station;
        num_ticket = info0.num_ticket;
        offset_stationinfo = info0.offset_stationinfo;
        for(int i = 0; i < info0.num_ticket; i++){
            ticket_name[i].init(info0.ticket_name[i].ch);
        }
        for(int i = 0; i < info0.num_station; i++){
            /*trainStation[i].station_name.init(info0.trainStation[i].station_name.ch);
            trainStation[i].arrival_time = info0.trainStation[i].arrival_time;
            trainStation[i].leave_time = info0.trainStation[i].leave_time;
            trainStation[i].gap_time = info0.trainStation[i].gap_time;*/
            for(int j = 0; j < info0.num_ticket; j++){
                ticket_num[i][j] = info0.ticket_num[i][j];
            }
        }
        return *this;
    }
};


/*std::ostream &operator<< (std::ostream &out, train_info &info1){
    out << info1.train_id << " " << info1.train_name << " " <<  info1.train_catalog << " " << info1.num_station << " " << info1.num_ticket;
    for(int i = 0; i < info1.num_ticket; i++){
        out << " " << info1.ticket_name[i];
    }
    out << std::endl;
    for(int i = 0; i < info1.num_station; i++){
        out << inf
        for(int j = 0; j < info1.num_ticket; j++)
        out << std::endl;
    }
    return out;
}
std::istream &operator>> (std::istream &in, train_info &info1){
    in >> info1.train_id >> info1.train_name >> info1.train_catalog >> info1.num_station >> info1.num_ticket;
    for(int i = 0; i < info1.num_ticket; i++){
        in >> info1.ticket_name[i];
    }
    for(int i = 0; i < info1.num_station; i++){
        char ch;
        for(int j = 0; j < info1.num_ticket; j++) in >> ch >> ch >> ch >> info1.ticket_num[i][j];// cout << info1.trainStation[i].money[j];
    }
    return in;
}*/

class train_key{
  public:
    friend bool operator>(const train_key &key1, const train_key &key2);
    friend bool operator==(const train_key &key1, const train_key &key2);
    friend  bool operator<(const train_key &key1, const train_key &key2);
    mychar train_id;
    int date;

    explicit train_key(const char* id = "", int date0 = 1){
        train_id.init(id);
        date = date0;
    }

    train_key(const train_key &key){
        train_id.init(key.train_id.ch);
        date = key.date;
    }

    train_key &operator=(const train_key &key){
        train_id.init(key.train_id.ch);
        date = key.date;
        return *this;
    }


};
bool operator>(const train_key &key1, const train_key &key2){
    if(key1.train_id > key2.train_id) return true;
    if(key1.train_id < key2.train_id) return false;
    if(key1.date > key2.date) return true;
    else return false;
}

bool operator==(const train_key &key1, const train_key &key2){
    return (key1.train_id == key2.train_id && key1.date == key2.date);
}

bool operator<(const train_key &key1, const train_key &key2){
    return key2 > key1;
}
std::ostream &operator<< (std::ostream &out, train_key &key1){
    out << key1.train_id << " " << key1.date << std::endl;
    return out;
}
}


#endif //TRAINANDTICKET_TRAININFO_H
