//
// Created by 金乐盛 on 2018/6/7.
//

#ifndef TRAIN_LINEAR_LIST_H
#define TRAIN_LINEAR_LIST_H
#include <iostream>
#include <fstream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "bppair.h"
//#define OFFSET_META 0
namespace Database{
struct linear_meta{
    long solt;
};

template <class Value>
class linear_list{
  public:
    //bpt_meta meta{};
    FILE *fp;
    tool::mychar path;

    linear_list(const char* filename = ""){
        path.init(filename);
        fp = nullptr;
    }


    void clear(){
        fclose(fp);
        fp = fopen(path.ch, "w");
        fclose(fp);
        fp = fopen(path.ch, "rb+");
        build();
    }


    void build(){
        meta.solt = 0 + sizeof(linear_meta);
        writefile(&meta, 0, 1, sizeof(linear_meta));
    }

    void openfile(){
        fp = fopen(path.ch, "rb+");
        if(fp == NULL) {fp = fopen(path.ch, "wb+"); fclose(fp); fp = fopen(path.ch, "rb+");}
    }
    void closefile(){
        fclose(fp);
    }

    void init(const char* filename = ""){
        path.clear();
        path.add(filename);
        fp = fopen(path.ch, "rb+");
        if(fp != NULL){
            if(readfile(&meta, 0, 1, sizeof(meta))){
                fclose(fp);
            }
            else{
                readfile(&meta, 0, 1, sizeof(meta));
                fclose(fp);
            }
        }
        else{
            fp = fopen(path.ch, "w");
            fclose(fp);
            fp = fopen(path.ch, "rb+");
            meta.solt = sizeof(meta);
            fclose(fp);
        }
    }

    long writevalue(Value v[], int num){
        writefile(v, meta.solt, 1, sizeof(Value) * num);
        long ans = meta.solt;
        meta.solt += sizeof(Value) * num;
        return ans;
    }

    void readvalue(long offset, Value v[], int num){
        readfile(v, offset, 1, sizeof(Value) * num);
    }



  private:
    linear_meta meta;
    size_t readfile(void *value, long offset, size_t numofblock, size_t sizeofblock){
        int judge;
        judge = fseek(fp, offset, 0);
        if(judge != 0) throw "fuck";
        size_t numOfcorrect;
        numOfcorrect = fread(value, numofblock, sizeofblock, fp);
        return numOfcorrect;
    }

    size_t writefile(void *value, long offset, size_t numofblock, size_t sizeofblock){
        int judge;
        judge = fseek(fp, offset, 0);
        if(judge != 0) throw "fuck";
        size_t numofcorrect;
        numofcorrect = fwrite(value, numofblock, sizeofblock, fp);
        return numofcorrect;
    }


};
}
#endif //TRAIN_LINEAR_LIST_H
