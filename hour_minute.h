//
// Created by 金乐盛 on 2018/6/1.
//

#ifndef TRAINANDTICKET_TIME_H
#define TRAINANDTICKET_TIME_H
#include "bppair.h"
using namespace bpt;
//using namespace std;
namespace tool{
class hour_minute{
    friend std::ostream &operator<< (std::ostream &out, hour_minute &time0);
    friend std::istream &operator>> (std::istream &in, hour_minute &time0);
    friend bool operator>(const hour_minute &h1, const hour_minute &h2);
    friend bool operator==(const hour_minute &h1, const hour_minute &h2);
    friend bool operator<(const hour_minute &h1, const hour_minute &h2);
  private:
    pair<int ,int> clock;

  public:
    explicit hour_minute(int h = -1, int m = -1):clock(pair<int, int>(h, m)){};
    hour_minute(pair<int, int> &p):clock(p){}
    hour_minute & operator=(const hour_minute &other){
        clock.first = other.clock.first;
        clock.second = other.clock.second;
        return *this;
    }
    hour_minute(hour_minute &t){clock.first = t.clock.first; clock.second = t.clock.second;}


};

bool operator>(const hour_minute &h1, const hour_minute &h2){
    if(h1.clock.first > h2.clock.first) return true;
    if(h1.clock.first < h2.clock.first) return false;
    if(h1.clock.second > h2.clock.second) return true;
    else return false;
}

bool operator==(const hour_minute &h1, const hour_minute &h2){
    return (h1.clock.first == h2.clock.first && h1.clock.second == h2.clock.second);
}

bool operator<(const hour_minute &h1, const hour_minute &h2){
    return (h2 > h1);
}

std::ostream &operator<< (std::ostream &out, hour_minute &hour_minute1){
    if(hour_minute1.clock.first == -1){
        out << "xx:xx";
    }
    else {
        std::cout.fill('0');//设置填充字符
        std::cout.width(2);//设置域宽
        out << hour_minute1.clock.first;
        out << ":";
        std::cout.fill('0');//设置填充字符
        std::cout.width(2);//设置域宽
        out << hour_minute1.clock.second;
    }
    return out;
}
std::istream &operator>> (std::istream &in, hour_minute &hour_minute1){
    mychar t;
    in >> t;
    if(t.ch[0] == 'x') hour_minute1.clock.first = -1, hour_minute1.clock.second = -1;
    else {
        hour_minute1.clock.first = (t.ch[0] - '0') * 10 + (t.ch[1] - '0');
        hour_minute1.clock.second = (t.ch[3] - '0') * 10 + (t.ch[4] - '0');
    }
    return in;
}

}
#endif //TRAINANDTICKET_TIME_H
