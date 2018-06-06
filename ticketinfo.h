//
// Created by 金乐盛 on 2018/6/1.
//

#ifndef TRAINANDTICKET_TICKETINFO_H
#define TRAINANDTICKET_TICKETINFO_H
#include "bppair.h"
#include "traininfo.h"
using namespace tool;
namespace ticket_system{
class ticket_buyer{
  public:
    mychar id_consumer;
    int date;
    mychar catalog;
    mychar train_id;
    mychar loc1;
    mychar loc2;


    ticket_buyer(const char* id = "", const int date0 = 1, const char* catalog0 = "" ,const char* trainid = "" , const char* loc11 = "" ,const char* loc22 = ""){
        id_consumer.init(id);
        date = date0;
        catalog.init(catalog0);
        train_id.init(trainid);
        loc1.init(loc11);
        loc2.init(loc22);
    }
    ticket_buyer(const ticket_buyer &t){
        id_consumer.init(t.id_consumer.ch);
        train_id.init(t.train_id.ch);
        date = t.date;
        catalog.init(t.catalog.ch);
        loc1.init(t.loc1.ch);
        loc2.init(t.loc2.ch);
        //ticket_kind.init(t.ticket_kind.ch);
    }
};

bool operator==(const ticket_buyer &t1, const ticket_buyer &t2){
    return (t1.id_consumer == t2.id_consumer && t1.date == t2.date && t1.catalog == t2.catalog
        && t1.loc1 == t2.loc1 && t1.loc2 == t2.loc2 && t1.train_id == t2.train_id);
}

bool operator<(const ticket_buyer &t1, const ticket_buyer &t2){
    if(t1.id_consumer < t2.id_consumer) return true;
    if(t1.id_consumer > t2.id_consumer) return false;
    if(t1.date < t2.date) return true;
    if(t1.date > t2.date) return false;
    if(t1.catalog < t2.catalog) return true;
    if(t1.catalog > t2.catalog) return false;
    if(t1.train_id < t2.train_id) return true;
    if(t1.train_id > t2.train_id) return false;
    if(t1.loc1 < t2.loc1) return true;
    if(t1.loc1 > t2.loc1) return false;
    if(t1.loc2 < t2.loc2) return true;
    if(t1.loc2 > t2.loc2) return false;
    //if(t1.ticket_kind < t2.ticket_kind) return true;
    //if(t1.ticket_kind > t2.ticket_kind) return false;
    return false;
}

bool operator>(const ticket_buyer &t1, const ticket_buyer &t2){
    return !(t1 < t2 || t1 == t2);
}

bool operator<=(const ticket_buyer &t1, const ticket_buyer &t2){
    if(t1.id_consumer < t2.id_consumer) return true;
    if(t1.id_consumer > t2.id_consumer) return false;
    if(t1.date < t2.date) return true;
    if(t1.date > t2.date) return false;
    //if(t1.catalog < t2.catalog) return true;
    //if(t1.catalog > t2.catalog) return false;
    return false;
}

bool operator>=(const ticket_buyer &t1, const ticket_buyer &t2){
    /*if(t1.id_consumer > t2.id_consumer) return true;
    if(t1.id_consumer < t2.id_consumer) return false;
    if(t1.date > t2.date) return true;
    if(t1.date < t2.date) return false;
    if(t1.catalog > t2.catalog) return true;
    if(t1.catalog < t2.catalog) return false;
    return false;*/
    return (t2 <= t1);
}
class ticket_info{
  public:
    tool::hour_minute leavetime;
    tool::hour_minute arrivaltime;
    mychar ticket_name[MAX_TICKET_KIND];
    money ticket_money[MAX_TICKET_KIND];
    int ticket_num[MAX_TICKET_KIND];

    ticket_info(){
        for(int i = 0; i < MAX_TICKET_KIND; i++){
            ticket_name[i].init("");
            ticket_money[i].price = 0;
            ticket_num[i] = 0;
        }
    }
    ticket_info(const ticket_info &t){
        leavetime = t.leavetime;
        arrivaltime = t.arrivaltime;
        for(int i = 0 ; i < MAX_TICKET_KIND; i++){
            ticket_name[i].init(t.ticket_name[i].ch);
            ticket_money[i].price = t.ticket_money[i].price;
            ticket_num[i] = t.ticket_num[i];
        }
    }
};

}
#endif //TRAINANDTICKET_TICKETINFO_H
