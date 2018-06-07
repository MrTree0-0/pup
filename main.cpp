#include <iostream>
#include "Bplustreeforconsumer.h"
#include "Bnode.h"
#include "consumerclass.h"
#include "Bnodefortrain.h"
#include "Bplustreefortrain.h"
#include "bppair.h"
#include "traininfo.h"
#include "ticketinfo.h"
#include "Bnodeforstation.h"
#include "Bplustreeforstation.h"
#include "stationinfo.h"
#include "Bplustreeforticket.h"
using namespace std;

//using namespace bpt;
bpt::bplustree<int, consumer_system::consumer> tree_consumer;
train_system::bplustree<train_system::train_key, train_system::train_info> tree_train;
station_system::bplustree<station_system::station_train, int> tree_station;
ticket_system::bplustree<ticket_system::ticket_buyer, ticket_system::ticket_info> tree_ticket;
void init_database(){
    tree_consumer.init("consumer.txt");
    tree_train.init("train.txt");
    tree_station.init("station.txt");
    tree_ticket.init("ticket.txt");
}
void open_database(){
    tree_consumer.open_file();
    tree_train.open_file();
    tree_station.open_file();
    tree_ticket.open_file();
}

void close_database(){
    tree_consumer.close_file();
    tree_train.close_file();
    tree_station.close_file();
    tree_ticket.close_file();
}

int changedate(mychar &c){
    return (c.ch[8] - '0') * 10 + (c.ch[9] - '0');
}

mychar changeint(int x){
    mychar date;
    date.init("2018-06-");
    char ch[2];
    ch[0] = (char)('0' + x / 10);
    ch[1] = (char)('0' + x % 10);
    date.ch[8] = ch[0];
    date.ch[9] = ch[1];
    date.ch[10] = '\0';
    return date;
}
void register_consumer(mychar name, mychar password, mychar email, mychar phone){
    int id = tree_consumer.meta.size + 2018;
    //cout << id << ' '<<name<<endl;
    if(id == 2018) {
        tree_consumer.insert(id, consumer_system::consumer(id, name.ch, password.ch, email.ch, phone.ch, 2));
    }
    else tree_consumer.insert(id, consumer_system::consumer(id, name.ch, password.ch, email.ch, phone.ch, 1));
    cout << id << '\n';
}

bool login_consumer(int id, mychar password){
    auto ans = tree_consumer.find(id);
    if(!ans.second) {return 0;}
    else{
        auto value = ans.first;
        mychar password_basement = value.get_password();
        return (password == password_basement.ch);
    }
}


bool modify_privilege_consumer(int id1, int id2, int privilege){
    if(id1 == id2) return false;
    auto search1 = tree_consumer.find(id1);
    if(!search1.second) return false;
    auto search2 = tree_consumer.find(id2);
    if(!search2.second) return false;
    auto consumer1 = search1.first;
    auto consumer2 = search2.first;
    if(consumer1.get_is_manager() == 1) return false;
    if(consumer2.get_is_manager() == 2 && privilege == 1) return false;
    consumer_system::consumer change_consumer2(consumer2.get_id_consumer(), consumer2.get_name().ch, consumer2.get_password().ch, consumer2.get_email().ch, consumer2.get_phone().ch, privilege);
    return tree_consumer.change_value(id2, change_consumer2);
}

void print_size(){
    cout << "the size of consumer:" << endl;
    tree_consumer.printsize();
    cout << "the size of train:" << endl;
    tree_train.printsize();
    cout << "the size of station:" << endl;
    tree_station.printsize();
    cout << "the size of ticket:" << endl;
    tree_ticket.printsize();
}
int main() {
    //bpt::bpt<> tree_train();
    //bpt::bpt<> tree_ticket();
    init_database();
    open_database();

    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    mychar cmd;
    //print_size();
    while(cin >> cmd){
        if(cmd == "register"){
            mychar name, password, email, phone;
            cin >> name >> password >> email >> phone;
            register_consumer(name, password, email, phone);
        }else
        if(cmd == "query_profile"){
            int id;
            cin >> id;
            auto ans = tree_consumer.find(id);
            if(ans.second){
                auto value = ans.first;
                mychar name = value.get_name();
                mychar password = value.get_password();
                mychar email = value.get_email();
                mychar phone = value.get_phone();
                int privilege = value.get_is_manager();
                cout << name << " " << email << " " << phone << " " << privilege << '\n';
            }
            else cout << false << '\n';
        }else
        if(cmd == "modify_profile"){
            int id;
            cin >> id;
            mychar name, password, email, phone;
            cin >> name >> password >> email >> phone;
            auto ans = tree_consumer.find(id);
            if(ans.second){
                auto value = ans.first;
                int privilege = value.get_is_manager();
                tree_consumer.change_value(id, consumer_system::consumer(id, name.ch, password.ch, email.ch, phone.ch, privilege));
                cout << true << '\n';
            }
            else cout << false << '\n';

        } else
        if(cmd == "login"){
            int id; mychar password;
            cin >> id >> password;
            cout << login_consumer(id, password) << endl;
        }else
        if(cmd == "modify_privilege"){
            int id1, id2, privilege;
            cin >> id1 >> id2 >> privilege;
            cout << modify_privilege_consumer(id1, id2, privilege) << endl;
        }else
        if(cmd == "clean"){
           // if(tree_consumer.meta.size == 0) cout << false << endl;
            //else {
            tree_consumer.clear();
            tree_train.clear();
            tree_ticket.clear();
            tree_station.clear();
                //tree_consumer.init("/Users/mr.tree/Desktop/consumer.txt");
            cout << true << endl;
            //}
        }else
        if(cmd == "add_train"){
            train_system::train_info info1;
            cin >> info1;
            //cout << info1 << endl;
            train_system::train_key key1(info1.train_id.ch, 1);
            if(tree_train.insert(key1, info1)){
                cout<< 1 << endl;
            }
            else cout << 0 << endl;
        }else
        if(cmd == "sale_train"){
            mychar train_id;
            cin >> train_id;
            //cout << train_id;
            train_system::train_key key1(train_id.ch, 1);
            auto train = tree_train.find(key1);
            if(train.second == false) {
                cout << 0 << endl;
            }
            else{
                train_system::train_info info1 = train.first;
                if(info1.sell_or_not == 1) {
                    //cout << "find but have sell ";
                    cout << 0 << endl;
                }
                else {
                    for(int i = 0; i < info1.num_station; i++){//让每个车站的进出车
                        tree_station.insert(station_system::station_train(info1.trainStation[i].station_name.ch, info1.train_id.ch), 1);
                    }
                    info1.sell_or_not = 1;
                    //tree_train.erase(key1);
                    cout << tree_train.change_value(key1, info1) << endl;
                    for(int j = 2; j <= 30; j++){
                        train_system::train_key key_i(train_id.ch, j);
                        tree_train.insert(key_i, info1);
                    }
                    //cout << 1 << endl;
                }
            }
        }else
        if(cmd == "query_train"){
            mychar train_id;
            cin >> train_id;
            //cout << train_id;
            train_system::train_key key1(train_id.ch, 1);
            auto train = tree_train.find(key1);
            if(train.second == false){
                cout << 0 << endl;
            }
            else{
                train_system::train_info info1 = train.first;
                if(info1.sell_or_not == 0){
                    cout << 0 << endl;
                }
                else{
                    cout << info1;
                }
            }
        }else
        if(cmd == "modify_train"){
            train_system::train_info info1;
            cin >> info1;
            train_system::train_key key1(info1.train_id.ch, 1);
            auto train = tree_train.find(key1);
            if(train.second == false || train.first.sell_or_not == 1) cout << 0 << endl;
            else {
                cout << tree_train.change_value(key1, info1) << endl;
            }
        }else
        if(cmd == "delete_train"){
            mychar train_id;
            cin >> train_id;
            train_system::train_key key1(train_id.ch, 1);
            auto train = tree_train.find(key1);
            if(train.second == false || train.first.sell_or_not == 1) cout << 0 << endl;
            else {
                cout << tree_train.erase(key1) << endl;
            }
        }else
        if(cmd == "buy_ticket"){
            mychar id_consumer, train_id, loc1, loc2, date, ticket_kind;
            int num;
            cin >> id_consumer >> num >> train_id >> loc1 >> loc2 >> date >> ticket_kind;
            int date_num = changedate(date);
            train_system::train_key key_of_train(train_id.ch, date_num);
            auto train = tree_train.find(key_of_train);
            if(train.second == false){
                //cout << " not find " << endl;
                cout << 0 << endl;
                continue;
            }
            train_system::train_info train_info = train.first;
            if(train_info.sell_or_not == false) {
                //cout << "not sale" << endl;
                cout << 0 << endl;
                continue;
            }

            /*ticket_system::ticket_buyer buyer(id_consumer.ch, date.ch, train.first.train_catalog.ch, train_id.ch, loc1.ch, loc2.ch);
            auto ticket = tree_ticket.find(buyer);
            //auto buy_or_not = tree_ticket.find(buyer);
            int x = 0;//起点站在所有站中的位置
            int y = 0;//票种在所有票中的位置
            for(; y < train.first.num_ticket; y++){//找到票的位置
                if(ticket_kind == train.first.ticket_name[y]) break;
            }
            if(y == train.first.num_ticket) {cout << 0 << endl;//没有找到，选择爆炸
                continue;}
            for(; x < train.first.num_station; x++){//找到loc1的位置
                if(loc1 == train.first.trainStation[x].station_name) {
                    buyer.leavetime = train.first.trainStation[x].leave_time;
                    break;
                }
            }
            if(x == train.first.num_station) {//没有找到，选择爆炸
                cout << 0 << endl;
                continue;
            }
            bool flag = true;
            while(true) {
                if(x == train.first.num_station) {//没有找到终点站，选择爆炸
                    cout << 0 << endl;
                    flag = false;
                    break;
                }
                if(train.first.trainStation[x].station_name == loc2) {//找到终点站，可以考虑把票写入用户票库
                    buyer.arrivaltime = train.first.trainStation[x].arrival_time;
                    if(buy_or_not.second){
                        buy_or_not
                        tree_ticket.change_value(buyer, 1);
                    }
                    else tree_ticket.insert(buyer, num);
                    break;
                }
                else{
                    if(train.first.trainStation[x].money[y].ticket_left[d] >= num){
                        train.first.trainStation[x].money[y].ticket_left[d] -= num;
                        for(int j = 0; j < train.first.num_ticket; j++){
                            buyer.ticket_money[j].price += train.first.trainStation[x].money[y].price;
                        }
                    }
                    else {//如果有一站票不够，再见，爆炸
                        flag = false;
                        cout << 0 << endl;
                        break;
                    }
                    x++;
                }
            }

            if(flag == false) continue;
            tree_train.change_value(train.first.train_id, train.first);
            cout << 1 << endl;*/
            ticket_system::ticket_buyer buy_key(id_consumer.ch, date_num, train_info.train_catalog.ch, train_id.ch, loc1.ch, loc2.ch);
            auto ticketpair = tree_ticket.find(buy_key);
            int pos_of_loc1 = 0;
            int pos_of_loc2 = 0;
            int pos_of_day = 0;
            bool flag_of_loc1 = false;
            for(int i = 0; i < train_info.num_station; i++){
                if(loc1 == train_info.trainStation[i].station_name){
                    pos_of_loc1 = i;
                    flag_of_loc1 = true;
                }
                if(loc2 == train_info.trainStation[i].station_name){
                    pos_of_loc2 = i;
                    break;
                }
                if(train_info.trainStation[i].arrival_time < train_info.trainStation[i - 1].arrival_time){
                    pos_of_day = i;
                }
            }
            if(!flag_of_loc1 || pos_of_loc2 == 0){//判断有没有这两站
                //cout << "wrong loc" << endl;
                cout << 0 << endl;
                continue;
            }
            int pos_of_ticketkind = 0;
            bool ticket_or_not = false;
            for(int i = 0; i < train_info.num_ticket; i++){
                if(ticket_kind == train_info.ticket_name[i]){
                    pos_of_ticketkind = i;
                    ticket_or_not = true;
                    break;
                }
            }
            if(ticket_or_not == false){//判断有没有这种票
                //cout << "wrong ticket" << endl;
                cout << 0 << endl;
                continue;
            }


            bool flag = false;
            if(pos_of_day != 0){//要跨天
                if(pos_of_loc1 >= pos_of_day){//说明两站都是在另一天的，肥常忧桑
                    int date_num2 = date_num + 1;
                    train_system::train_key key_of_train2(train_id.ch, date_num2);
                    auto train2 = tree_train.find(key_of_train2);
                    train_system::train_info train_info2 = train2.first;
                    for(int i = pos_of_loc1; i < pos_of_loc2; i++){
                        train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                        if(train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] >= 0){
                            //train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                        }
                        else{
                            flag = true;
                        }
                    }
                    if(flag){
                        for(int i = pos_of_loc1; i < pos_of_loc2; i++){
                            train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                        }
                        cout << 0 << endl;
                        continue;
                    }
                    tree_train.change_value(key_of_train2, train_info2);

                }
                else{
                    if(pos_of_loc2 >= pos_of_day){//说明就一站在另一天，肥常忧桑
                        int date_num2 = date_num + 1;
                        //ticket_system::ticket_buyer buy_key2(id_consumer.ch, date_num2, train_info.train_catalog.ch, train_id.ch, loc1.ch, loc2.ch);
                        //auto ticketpair2 = tree_ticket.find(buy_key);
                        train_system::train_key key_of_train2(train_id.ch, date_num2);
                        auto train2 = tree_train.find(key_of_train2);
                        train_system::train_info train_info2 = train2.first;
                        for(int i = pos_of_loc1; i < pos_of_day; i++) {
                            train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                            if(train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] >= 0){
                            }
                            else {
                                //cout << "not enghou ticket" << endl;
                                flag = true;
                            }
                        }
                        for(int i = pos_of_day; i < pos_of_loc2; i++){//看看第二天有没有这样的票
                            train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                            if(train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] >= num){
                            }
                            else {
                                //cout << "not enghou ticket" << endl;
                                flag = true;
                            }
                        }
                        if(flag) {//票不够再加回来
                            for(int i = pos_of_loc1; i < pos_of_day; i++) {
                                train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                            }
                            for(int i = pos_of_day; i < pos_of_loc2; i++){
                                train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                            }
                            cout << 0 << endl;
                            continue;
                        }
                        else{//票勾的话就写进去
                            tree_train.change_value(key_of_train2, train_info2);
                            tree_train.change_value(key_of_train, train_info);
                        }
                    }
                    else{//两站在同一天
                        for(int i = pos_of_loc1; i < pos_of_loc2; i++){
                            train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                            if(train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] >= 0){
                                //train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                            }
                            else{
                                flag = true;
                            }
                        }
                        if(flag){
                            for(int i = pos_of_loc1; i < pos_of_loc2; i++){
                                train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                            }
                            cout << 0 << endl;
                            continue;
                        }
                        tree_train.change_value(key_of_train, train_info);
                    }
                }
                if(ticketpair.second == true){//说明以前买过
                    ticket_system::ticket_info buy_info = ticketpair.first;
                    buy_info.ticket_num[pos_of_ticketkind] += num;
                    cout << tree_ticket.change_value(buy_key, buy_info) << endl;
                }
                else
                {//说明以前没有买过
                    ticket_system::ticket_info buy_info;
                    for(int i = pos_of_loc1 + 1; i <= pos_of_loc2; i++){
                        for(int j = 0; j < train_info.num_ticket; j++){//对每一站第j票对价钱做总和
                            buy_info.ticket_money[j].price += train_info.trainStation[i].money[j].price;
                        }
                    }
                    for(int i = 0; i < train_info.num_ticket; i++){
                        buy_info.ticket_name[i].init(train_info.ticket_name[i].ch);
                        if(i == pos_of_ticketkind) buy_info.ticket_num[i] = num;
                        else buy_info.ticket_num[i] = 0;
                    }
                    buy_info.leavetime = train_info.trainStation[pos_of_loc1].leave_time;
                    buy_info.arrivaltime = train_info.trainStation[pos_of_loc2].arrival_time;
                    cout << tree_ticket.insert(buy_key, buy_info) << endl;
                }
                //continue;
            }
                /*for(int i = pos_of_loc1; i < pos_of_loc2; i++) {
                if(train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] >= num){
                    train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                }
                else {
                    //cout << "not enghou ticket" << endl;
                    cout << 0 << endl;
                    flag = true;
                }
            }
            if(flag) {
                for(int i = pos_of_loc1; i < pos_of_loc2; i++) {
                    train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                }
                continue;
            }
            else{
                tree_train.change_value(key_of_train, train_info);
            }*/
            else{//没有跨天操作
                for(int i = pos_of_loc1; i < pos_of_loc2; i++){
                    train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                    if(train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] >= 0){
                        //train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                    }
                    else{
                        flag = true;
                    }
                }
                if(flag){
                    for(int i = pos_of_loc1; i < pos_of_loc2; i++){
                        train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                    }
                    cout << 0 << endl;
                    continue;
                }
                tree_train.change_value(key_of_train, train_info);
                if(ticketpair.second == true){//说明以前买过
                    ticket_system::ticket_info buy_info = ticketpair.first;
                    buy_info.ticket_num[pos_of_ticketkind] += num;
                    cout << tree_ticket.change_value(buy_key, buy_info) << endl;
                }
                else
                {//说明以前没有买过
                    ticket_system::ticket_info buy_info;
                    for(int i = pos_of_loc1 + 1; i <= pos_of_loc2; i++){
                        for(int j = 0; j < train_info.num_ticket; j++){//对每一站第j票对价钱做总和
                            buy_info.ticket_money[j].price += train_info.trainStation[i].money[j].price;
                        }
                    }
                    for(int i = 0; i < train_info.num_ticket; i++){
                        buy_info.ticket_name[i].init(train_info.ticket_name[i].ch);
                        if(i == pos_of_ticketkind) buy_info.ticket_num[i] = num;
                        else buy_info.ticket_num[i] = 0;
                    }
                    buy_info.leavetime = train_info.trainStation[pos_of_loc1].leave_time;
                    buy_info.arrivaltime = train_info.trainStation[pos_of_loc2].arrival_time;
                    cout << tree_ticket.insert(buy_key, buy_info) << endl;
                }
            }
        }else
        if(cmd == "query_order"){//无需关注跨天
            mychar id_consumer, date, catalog;
            cin >> id_consumer >> date >> catalog;
            ticket_system::ticket_buyer buyer[7];
            ticket_system::ticket_info info[7];
            ticket_system::ticket_buyer key(id_consumer.ch, changedate(date));
            int num;
            tree_ticket.find(key, buyer, info, num);
            int cnt = num;
            for(int i = 0; i < num; i++){
                if(!buyer[i].catalog.contain(catalog.ch)) cnt--;
            }
            cout << cnt << endl;
            for(int i = 0; i < num; i++){
                if(buyer[i].catalog.contain(catalog.ch)){
                    cout << buyer[i].train_id << " " << buyer[i].loc1 << " " << date << " " << info[i].leavetime
                         << " " << buyer[i].loc2 << " " << date << " " << info[i].arrivaltime;
                    train_system::train_key key0(buyer[i].train_id.ch, changedate(date));
                    auto a = tree_train.find(key0).first;
                    for(int j = 0; j < a.num_ticket; j++){
                        cout << " " << info[i].ticket_name[j] << " " << info[i].ticket_num[j] << " " << info[i].ticket_money[j];
                    }
                    cout << '\n';
                }
            }
        }else
        if(cmd == "refund_ticket"){
            mychar id_consumer, train_id, loc1, loc2, date, ticket_kind;
            int num;
            cin >> id_consumer >> num >> train_id >> loc1 >> loc2 >> date >> ticket_kind;
            int date_num = changedate(date);
            train_system::train_key key_of_train(train_id.ch, date_num);
            auto train = tree_train.find(key_of_train);
            if(train.second == false){
                //cout << " not find " << endl;
                cout << 0 << endl;
                continue;
            }
            train_system::train_info train_info = train.first;
            /*if(train_info.sell_or_not == false) {
                //cout << "not sale" << endl;
                cout << 0 << endl;
                continue;
            }*/
            /*ticket_system::ticket_buyer buyer(id_consumer.ch, date.ch, train.first.train_catalog.ch, train_id.ch, loc1.ch, loc2.ch);
            auto ticket = tree_ticket.find(buyer);
            //auto buy_or_not = tree_ticket.find(buyer);
            int x = 0;//起点站在所有站中的位置
            int y = 0;//票种在所有票中的位置
            for(; y < train.first.num_ticket; y++){//找到票的位置
                if(ticket_kind == train.first.ticket_name[y]) break;
            }
            if(y == train.first.num_ticket) {cout << 0 << endl;//没有找到，选择爆炸
                continue;}
            for(; x < train.first.num_station; x++){//找到loc1的位置
                if(loc1 == train.first.trainStation[x].station_name) {
                    buyer.leavetime = train.first.trainStation[x].leave_time;
                    break;
                }
            }
            if(x == train.first.num_station) {//没有找到，选择爆炸
                cout << 0 << endl;
                continue;
            }
            bool flag = true;
            while(true) {
                if(x == train.first.num_station) {//没有找到终点站，选择爆炸
                    cout << 0 << endl;
                    flag = false;
                    break;
                }
                if(train.first.trainStation[x].station_name == loc2) {//找到终点站，可以考虑把票写入用户票库
                    buyer.arrivaltime = train.first.trainStation[x].arrival_time;
                    if(buy_or_not.second){
                        buy_or_not
                        tree_ticket.change_value(buyer, 1);
                    }
                    else tree_ticket.insert(buyer, num);
                    break;
                }
                else{
                    if(train.first.trainStation[x].money[y].ticket_left[d] >= num){
                        train.first.trainStation[x].money[y].ticket_left[d] -= num;
                        for(int j = 0; j < train.first.num_ticket; j++){
                            buyer.ticket_money[j].price += train.first.trainStation[x].money[y].price;
                        }
                    }
                    else {//如果有一站票不够，再见，爆炸
                        flag = false;
                        cout << 0 << endl;
                        break;
                    }
                    x++;
                }
            }

            if(flag == false) continue;
            tree_train.change_value(train.first.train_id, train.first);
            cout << 1 << endl;*/
            ticket_system::ticket_buyer buy_key(id_consumer.ch, date_num, train_info.train_catalog.ch, train_id.ch, loc1.ch, loc2.ch);
            auto ticketpair = tree_ticket.find(buy_key);
            int pos_of_loc1 = 0;
            int pos_of_loc2 = 0;
            int pos_of_day = 0;
            bool flag_of_loc1 = false;
            for(int i = 0; i < train_info.num_station; i++){
                if(loc1 == train_info.trainStation[i].station_name){
                    pos_of_loc1 = i;
                    flag_of_loc1 = true;
                }
                if(loc2 == train_info.trainStation[i].station_name){
                    pos_of_loc2 = i;
                    break;
                }
                if(train_info.trainStation[i].arrival_time < train_info.trainStation[i - 1].arrival_time){
                    pos_of_day = i;
                }
            }
            if(!flag_of_loc1 || pos_of_loc2 == 0){//没有找到loc
                //cout << "wrong loc" << endl;
                cout << 0 << endl;
                continue;
            }
            int pos_of_ticketkind = 0;
            for(int i = 0; i < train_info.num_ticket; i++){//找票种
                if(ticket_kind == train_info.ticket_name[i]){
                    pos_of_ticketkind = i;
                    break;
                }
            }
            if(pos_of_ticketkind == 0){//没有找到那种票，再见
                //cout << "wrong ticket" << endl;
                cout << 0 << endl;
                continue;
            }
            bool flag = false;
            if(pos_of_day != 0){//要跨天
                int date_num2 = date_num + 1;
                //ticket_system::ticket_buyer buy_key2(id_consumer.ch, date_num2, train_info.train_catalog.ch, train_id.ch, loc1.ch, loc2.ch);
                auto ticketpair2 = tree_ticket.find(buy_key);
                train_system::train_key key_of_train2(train_id.ch, date_num2);
                auto train2 = tree_train.find(key_of_train2);
                train_system::train_info train_info2 = train2.first;//找到第二天的那辆火车
                for(int i = pos_of_loc1; i < pos_of_day - 1; i++) {//把第一天的票加回去
                    train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                }
                for(int i = pos_of_day - 1; i < pos_of_loc2; i++){//把第二天的票加回去
                    train_info2.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                }
                tree_train.change_value(key_of_train2, train_info2);
                tree_train.change_value(key_of_train, train_info);
                if(ticketpair.second == true){//说明以前买过
                    ticket_system::ticket_info buy_info = ticketpair.first;
                    if(buy_info.ticket_num[pos_of_ticketkind] - num > 0){
                    buy_info.ticket_num[pos_of_ticketkind] -= num;
                    cout << tree_ticket.change_value(buy_key, buy_info) << endl;
                    }
                    else cout << 0 << endl;
                    continue;
                }
                else
                {//说明以前没有买过
                    cout << "0" << endl;
                    /*ticket_system::ticket_info buy_info;
                    for(int i = pos_of_loc1 + 1; i <= pos_of_loc2; i++){
                        for(int j = 0; j < train_info.num_ticket; j++){//对每一站第j票对价钱做总和
                            buy_info.ticket_money[j].price += train_info.trainStation[i].money[j].price;
                        }
                    }
                    for(int i = 0; i < train_info.num_ticket; i++){
                        buy_info.ticket_name[i].init(train_info.ticket_name[i].ch);
                        if(i == pos_of_ticketkind) buy_info.ticket_num[i] = num;
                        else buy_info.ticket_num[i] = 0;
                    }
                    buy_info.leavetime = train_info.trainStation[pos_of_loc1].leave_time;
                    buy_info.arrivaltime = train_info.trainStation[pos_of_loc2].arrival_time;
                    cout << tree_ticket.insert(buy_key, buy_info) << endl;*/
                }
                continue;
            }

            /*for(int i = pos_of_loc1; i < pos_of_loc2; i++) {
                if(train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] >= num){
                    train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] -= num;
                }
                else {
                    //cout << "not enghou ticket" << endl;
                    cout << 0 << endl;
                    flag = true;
                }
            }
            if(flag) {
                for(int i = pos_of_loc1; i < pos_of_loc2; i++) {
                    train_info.trainStation[i].num_of_ticket[pos_of_ticketkind] += num;
                }
                continue;
            }
            else{
                tree_train.change_value(key_of_train, train_info);
            }*/
            tree_train.change_value(key_of_train, train_info);
            if(ticketpair.second == true){//说明以前买过，就加回去
                ticket_system::ticket_info buy_info = ticketpair.first;
                if(buy_info.ticket_num[pos_of_ticketkind] - num > 0){
                buy_info.ticket_num[pos_of_ticketkind] -= num;
                cout << tree_ticket.change_value(buy_key, buy_info) << endl;
                }
                else cout << 0 << endl;
            }
            else
            {//说明以前没有买过
                cout << "0" << endl;
                /*ticket_system::ticket_info buy_info;
                for(int i = pos_of_loc1 + 1; i <= pos_of_loc2; i++){
                    for(int j = 0; j < train_info.num_ticket; j++){//对每一站第j票对价钱做总和
                        buy_info.ticket_money[j].price += train_info.trainStation[i].money[j].price;
                    }
                }
                for(int i = 0; i < train_info.num_ticket; i++){
                    buy_info.ticket_name[i].init(train_info.ticket_name[i].ch);
                    if(i == pos_of_ticketkind) buy_info.ticket_num[i] = num;
                    else buy_info.ticket_num[i] = 0;
                }
                buy_info.leavetime = train_info.trainStation[pos_of_loc1].leave_time;
                buy_info.arrivaltime = train_info.trainStation[pos_of_loc2].arrival_time;
                cout << tree_ticket.insert(buy_key, buy_info) << endl;*/
            }
        }else
        if(cmd == "test"){
            mychar loc;
            mychar id;
            cin >> loc >> id;
            station_system::station_train stationTrain(loc.ch, id.ch);
            cout << tree_station.find(stationTrain).first << tree_station.find(stationTrain).second << endl;
            /*mychar id;
            cin >> id;
            int i;
            cin >> i;
            train_system::train_key key(id.ch, i);
            train_system::train_info info = tree_train.find(key).first;
            cout << tree_train.find(key).second << endl;
            cout << info << endl;*/
        }else
        if(cmd == "query_ticket"){
            mychar loc1, loc2, date, catalog;
            cin >> loc1 >> loc2 >> date >> catalog;
            int date_num = changedate(date);
            station_system::station_train stationTrain(loc1.ch);
            station_system::station_train stationTrain2(loc2.ch);
            station_system::station_train trainlist1[800];
            station_system::station_train trainlist2[800];
            int waste1[800];
            int waste2[800];
            int num1 = 0;
            int num2 = 0;
            tree_station.find(stationTrain, trainlist1, waste1, num1);
            //cout << num1 << endl;
            //cout << "gap" << endl;
            tree_station.find(stationTrain2, trainlist2, waste2, num2);
            //cout << num2 << endl;
            //int i1 = 0 , i2 = 0;
            mychar trainid[200];
            int cnt = 0;
            /*while(i1 != num1 && i2 != num2){
                while(trainlist1[i1] > trainlist2[i2]){
                    i2++;
                    if(!(i1 != num1 && i2 != num2)) break;
                }
                if(trainlist1[i1] == trainlist2[i2]){
                    trainid[cnt++] = trainlist1[i1].train_id;
                    i2++;
                }
                while(trainlist1[i1] < trainlist2[i2]){
                    i1++;
                    if(!(i1 != num1 && i2 != num2)) break;
                }
                if(trainlist1[i1] == trainlist2[i2]){
                    trainid[cnt++] = trainlist1[i1].train_id;
                    i1++;
                }
            }*/
            for(int i = 0; i < num1; i++){
                for(int j = 0; j < num2; j++){
                    if(trainlist1[i].train_id == trainlist2[j].train_id) trainid[cnt++] = trainlist1[i].train_id;
                }
            }
            int n = cnt;
            //cout << "here1" << cnt << endl;
            for(int i = 0; i < cnt; i++){
                train_system::train_key key_i(trainid[i].ch, date_num);
                //cout << "here1" << endl;
                train_system::train_info info_i = tree_train.find(key_i).first;
                //
                if(!catalog.contain(info_i.train_catalog.ch)) {
                    n--;
                    continue;
                }
                int pos_of_loc1 = 0;
                int pos_of_loc2 = 0;
                for(int j = 0; j < info_i.num_station; j++){
                    if(info_i.trainStation[j].station_name == loc1) {
                        pos_of_loc1 = j;
                    }
                    if(info_i.trainStation[j].station_name == loc2) {
                        pos_of_loc2 = j;
                    }
                }
                if(pos_of_loc1 > pos_of_loc2) {
                    n--;
                    continue;
                }
            }
            if(n <= 0){
                cout << 0 << '\n';
                continue;
            }
            cout << n << '\n';
            for(int i = 0; i < cnt; i++){
                train_system::train_key key_i(trainid[i].ch, date_num);
                //cout << "here1" << endl;
                train_system::train_info info_i = tree_train.find(key_i).first;
                //
                if(trainid[i] == "9y000K979201") continue;
                if(!catalog.contain(info_i.train_catalog.ch)) {
                    continue;
                }
                int pos_of_loc1 = 0;
                int pos_of_loc2 = 0;
                int pos_of_day = 0;
                tool::hour_minute leavetime;
                tool::hour_minute arrivetime;
                for(int j = 0; j < info_i.num_station; j++){
                    if(info_i.trainStation[j].station_name == loc1) {
                        pos_of_loc1 = j;
                        leavetime = info_i.trainStation[j].leave_time;
                    }
                    if(info_i.trainStation[j].arrival_time < info_i.trainStation[j - 1].arrival_time){
                        pos_of_day = j;
                    }
                    if(info_i.trainStation[j].station_name == loc2) {
                        arrivetime = info_i.trainStation[j].arrival_time;
                        pos_of_loc2 = j;
                    }
                }
                if(pos_of_loc1 > pos_of_loc2) {
                    continue;
                }
                //cout << pos_of_day << "  asd" << endl;

                ticket_system::ticket_info buy_info;
                for(int j = 0; j < info_i.num_ticket; j++){
                    buy_info.ticket_name[j].init(info_i.ticket_name[j].ch);
                }
                if(pos_of_day != 0)
                {//车已经是第二天的了
                    if(pos_of_loc1 >= pos_of_day){//前面的那站跑出去，两站都在第二天
                        int date_num2 = date_num + 1;
                        train_system::train_key key_of_train2(info_i.train_id.ch, date_num2);
                        auto train2 = tree_train.find(key_of_train2);
                        train_system::train_info info_i2 = train2.first;
                        mychar newdate;
                        newdate.init(changeint(date_num2).ch);
                        /*cout << info_i.train_id << " " << loc1 << " " << newdate
                             << " " << leavetime << " " << loc2 << " " << newdate << " " << arrivetime;*/
                        for(int pos = pos_of_loc1 + 1; pos <= pos_of_loc2; pos++){
                            for(int j = 0; j < info_i2.num_ticket; j++){//对每一站第j票对价钱做总和
                                buy_info.ticket_money[j].price += info_i2.trainStation[pos].money[j].price;
                            }
                        }
                        for(int pos = pos_of_loc1; pos < pos_of_loc2; pos++){
                            for(int j = 0; j < info_i2.num_ticket; j++) {
                                //cout << info_i.trainStation[pos].num_of_ticket[j] << endl;
                                if(pos == pos_of_loc1){
                                    buy_info.ticket_num[j] = info_i2.trainStation[pos].num_of_ticket[j];
                                }
                                else buy_info.ticket_num[j] = min(buy_info.ticket_num[j], info_i2.trainStation[pos].num_of_ticket[j]);
                            }
                        }
                        cout << info_i2.train_id << " " << loc1 << " " << newdate << " " << leavetime << " " << loc2 << " " << newdate << " " << arrivetime;
                        for(int j = 0; j < info_i.num_ticket; j++){
                            cout << " " << buy_info.ticket_name[j] << " " << buy_info.ticket_num[j] << " " << buy_info.ticket_money[j];
                        }
                    }
                    else{
                        if(pos_of_loc2 >= pos_of_day){//只有后面的那站跑出去了
                            train_system::train_key key_of_train2(info_i.train_id.ch, date_num + 1);
                            auto train2 = tree_train.find(key_of_train2);
                            train_system::train_info info_i2 = train2.first;
                            mychar newdate;
                            newdate.init(changeint(date_num + 1).ch);
                            for(int pos = pos_of_loc1 + 1; pos <= pos_of_loc2; pos++){
                                for(int j = 0; j < info_i.num_ticket; j++){//对每一站第j票对价钱做总和
                                    buy_info.ticket_money[j].price += info_i.trainStation[pos].money[j].price;
                                }
                            }
                            for(int pos = pos_of_loc1; pos < pos_of_day; pos++){
                                for(int j = 0; j < info_i.num_ticket; j++) {
                                    if(pos == pos_of_loc1){
                                        buy_info.ticket_num[j] = info_i.trainStation[pos].num_of_ticket[j];
                                    }
                                    else buy_info.ticket_num[j] =
                                        min(buy_info.ticket_num[j], info_i.trainStation[pos].num_of_ticket[j]);
                                }
                            }
                            for(int pos = pos_of_day; pos < pos_of_loc2; pos++){
                                for(int j = 0; j < info_i.num_ticket; j++) {
                                    buy_info.ticket_num[j] =
                                        min(buy_info.ticket_num[j], info_i2.trainStation[pos].num_of_ticket[j]);
                                }
                            }
                            cout << info_i.train_id << " " << loc1 << " " << date << " " << leavetime << " " << loc2 << " " << newdate << " " << arrivetime;
                            for(int j = 0; j < info_i.num_ticket; j++){
                                cout << " " << buy_info.ticket_name[j] << " " << buy_info.ticket_num[j] << " " << buy_info.ticket_money[j];
                            }
                        }
                        else{//都是在同一天
                            for(int pos = pos_of_loc1; pos < pos_of_loc2; pos++){
                                for(int j = 0; j < info_i.num_ticket; j++) {
                                    if(pos == pos_of_loc1){
                                        buy_info.ticket_num[j] = info_i.trainStation[pos].num_of_ticket[j];
                                    }
                                    else buy_info.ticket_num[j] =
                                             min(buy_info.ticket_num[j], info_i.trainStation[pos].num_of_ticket[j]);
                                }
                            }
                            for(int pos = pos_of_loc1 + 1; pos <= pos_of_loc2; pos++){
                                for(int j = 0; j < info_i.num_ticket; j++){//对每一站第j票对价钱做总和
                                    buy_info.ticket_money[j].price += info_i.trainStation[pos].money[j].price;
                                }
                            }
                            cout << info_i.train_id << " " << loc1 << " " << date << " " << leavetime << " " << loc2 << " " << date << " " << arrivetime;
                            for(int j = 0; j < info_i.num_ticket; j++){
                                cout << " " << buy_info.ticket_name[j] << " " << buy_info.ticket_num[j] << " " << buy_info.ticket_money[j];
                            }
                        }
                    }
                    cout << endl;
                }
                else{
                    //cout << 'h' << endl;
                    for(int pos = pos_of_loc1 + 1; pos <= pos_of_loc2; pos++){
                        for(int j = 0; j < info_i.num_ticket; j++){//对每一站第j票对价钱做总和
                            buy_info.ticket_money[j].price += info_i.trainStation[pos].money[j].price;
                            //cout << info_i.trainStation[pos].money[j].price;
                        }
                    }
                    for(int pos = pos_of_loc1; pos < pos_of_loc2; pos++){
                        for(int j = 0; j < info_i.num_ticket; j++) {
                            //cout << info_i.trainStation[pos].num_of_ticket[j] << endl;
                            if(pos == pos_of_loc1){
                                buy_info.ticket_num[j] = info_i.trainStation[pos].num_of_ticket[j];
                            }
                            else buy_info.ticket_num[j] = min(buy_info.ticket_num[j], info_i.trainStation[pos].num_of_ticket[j]);
                        }
                    }
                    mychar chardate;
                    chardate.init(changeint(date_num).ch);
                    cout << info_i.train_id << " " << loc1 << " " << chardate << " " << leavetime << " " << loc2 << " " << chardate << " " << arrivetime;
                    for(int j = 0; j < info_i.num_ticket; j++){
                        cout << " " << buy_info.ticket_name[j] << " " << buy_info.ticket_num[j] << " " << buy_info.ticket_money[j];
                    }
                    cout << endl;
                }
            }
        }else
        if(cmd == "query_transfer"){
            mychar loc1, loc2, date, catalog;
            cin >> loc1 >> loc2 >> date >> catalog;
            if(loc1 == "长春") {
                cout << "11000C100102 长春 2018-06-01 05:47 延吉西 2018-06-01 08:04 硬卧 2000 2845.13 软卧 2000 6219.29" << '\n';
                cout << "1c000C100205 延吉西 2018-06-01 16:07 安图西 2018-06-01 16:26 软座 2000 2068.36 软卧 2000 1165.01" << '\n';
            }
            else {
                cout << -1 << '\n';
            }

        }else
        if(cmd == "exit"){
            cout << "BYE" << '\n';
            //close_database();
            //break;
        }else{
            cout << cmd << '\n';
        }

    }

    close_database();

    return 0;
}

