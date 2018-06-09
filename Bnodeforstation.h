//
// Created by 金乐盛 on 2018/6/2.
//

#ifndef TRAINANDTICKET_BNODEFORSTATION_H
#define TRAINANDTICKET_BNODEFORSTATION_H
//
// Created by 金乐盛 on 2018/5/14.
//

#include <iostream>
#include <fstream>
#include "bppair.h"
#define PAIR_IN_LEAF_STATION 35
#define KEY_IN_INTER_STATION 49
#define LEAST_PAIR_IN_LEAF_STATION (PAIR_IN_LEAF_STATION / 2 + 0.999)
#define LEAST_KEY_IN_INTER_STATION (KEY_IN_INTER_STATION / 2 + 0.999)
using namespace tool;
namespace station_system{

template <class Key, class Value>
struct internal_node{
    long offset;//the offset of this
    long father;
    int numOfkey;//the number of keys
    int type;//if it's 0, it's a normal node which its children are nodes; else its children are leaf_nodes
    pair<Key, long> index[KEY_IN_INTER_STATION];//there are indexes point to the next level.
};

template <class Key, class Value>
struct leaf_node{//which contains some pair(key and value)
    long offset;//the offset of this
    long next;//the next leaf_node
    long father;//its father_node
    int numOfpair;//how much pair in the node
    pair<Key, Value> children[PAIR_IN_LEAF_STATION];//how much pair    !!!there may be an error

    //long pre;//the pre leaf_node
};


struct bpt_meta{
    //int numOftree;
    long firstleaf;//where is the first leaf
    long root;//where is the root
    long solt;//where is the fp*
    int size;
};






}

#endif //TRAINANDTICKET_BNODEFORSTATION_H
