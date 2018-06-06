//
// Created by 金乐盛 on 2018/6/1.
//

#ifndef TRAINANDTICKET_MONEY_H
#define TRAINANDTICKET_MONEY_H
#include <iostream>
namespace tool{
class money{
    friend std::ostream &operator<< (std::ostream &out, money &m);
    friend std::istream &operator>> (std::istream &in, money &m);
  private:

  public:
    double price;
    money(double x = 0):price(x){}

    money & operator=(const money &other){
        price = other.price;
        return (*this);
    }
};
std::ostream &operator<< (std::ostream &out, money &m){
    std::cout.precision(6);
    std::cout.setf(std::ios::fixed);
    if(m.price == 0) out << "0.0";
    else out << m.price;
    return out;
}
std::istream &operator>> (std::istream &in, money &m){
    in >> m.price;
    return in;
}
}
#endif //TRAINANDTICKET_MONEY_H
