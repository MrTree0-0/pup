//
// Created by 金乐盛 on 2018/6/4.
//

#ifndef TRAIN_BPLUSTREEFORTRAIN_H
#define TRAIN_BPLUSTREEFORTRAIN_H
//
// Created by 金乐盛 on 2018/5/12.
//
#include <iostream>
#include <fstream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "Bnodefortrain.h"
#include "bppair.h"


#define NO_OPEN
#define OFFSET_META 0
//#define debug "jadkjf"
//using namespace std;
using namespace tool;
namespace train_system{
//template <class Key, class Value, class Compare =  std::less<Key>>
template <class Key, class Value>
class bplustree{

  public:
    bpt_meta meta{};
    FILE *fp;
    mychar path;//the path to the file


    bplustree(const char *filename = ""){
        path.add(filename);
        fp = nullptr;
    }

    void init(const char* filename = ""){
        path.clear();
        path.add(filename);
        fp = fopen(path.ch, "rb+");
        if(fp != NULL){
            if(readfile(&meta, OFFSET_META, 1, sizeof(bpt_meta))){
                fclose(fp);
            }
            else{
                build_empty_tree();
                readfile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
                fclose(fp);
            }
        }
        else{
            /*fp = fopen(path.ch, "w");
            fclose(fp);*/
            //cout << "here" << endl;
            fp = fopen(path.ch, "w");
            fclose(fp);
            fp = fopen(path.ch, "rb+");
            build_empty_tree();
            readfile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
            fclose(fp);
        }
    }


    void clear(){
        fclose(fp);
        fp = fopen(path.ch, "w");
        fclose(fp);
        //cout << path << endl;
        fp = fopen(path.ch, "rb+");
        //cout << readfile(&meta, OFFSET_META, 1, sizeof(bpt_meta)) << endl;
        build_empty_tree();
        readfile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
        //fclose(fp);
    }

    void open_file(){
        //if the file is close, open the file
        if(fp_level == 0){
            fp = fopen(path.ch, "rb+");
            if(fp == NULL) {fp = fopen(path.ch, "wb+"); fclose(fp); fp = fopen(path.ch, "rb+");}
        }
        fp_level = 1;
    }

    void close_file(){
        //if the file is open, close the file
        if(fp_level == 1){
            fclose(fp);
        }
        fp_level = 0;
    }

    void printsize(){
        std::cout << "the size of leaf " << sizeof(leaf_node<Key, Value>) << " so there are " <<  (4 * 1024) / sizeof(leaf_node<Key, Value>) << std::endl;
        std::cout << "the size of node " << sizeof(internal_node<Key, Value>)  << " so there are " << (4 * 1024) / sizeof(internal_node<Key, Value>) << std::endl;
    }

    void print_the_leaf(long offset_leaf, int num){
        leaf_node<Key, Value> leaf;
        readfile(&leaf, offset_leaf, 1, sizeof(leaf_node<Key, Value>));
        std::cout << std::endl;
        std::cout << "the " << num << " leaf:" << std::endl;
        for(int i = 0; i < leaf.numOfpair; i++){
            std::cout << i << "\t" << " the key is " << leaf.children[i].first << " the value is " << leaf.children[i].second << std::endl;
        }
    }

    void print_the_node(long offset_node){
        internal_node<Key, Value> node;
        readfile(&node, offset_node, 1, sizeof(internal_node<Key, Value>));
        std::cout << std::endl;
        std::cout << " the node is here" << std::endl;
        //cout << node.index[0].first << node.index[1].first << endl;
        //cout << "the " << num << "leaf:" << endl;
        if(node.type == 1){
            for(int i = 0; i < node.numOfkey; i++){
                print_the_leaf(node.index[i].second, i);
            }
        }
        else{
            for(int i = 0; i < node.numOfkey; i++){
                std::cout << i << " the key is " << "\t" << node.index[i].first << '\t';
            }
            for(int i = 0; i < node.numOfkey; i++){
                print_the_node(node.index[i].second);
            }
        }

    }

    //something operator about Bplustree

    //insert key and value
    bool insert(const Key &key, const Value &value){
        leaf_node<Key, Value> leaf;
        //to find the key should in which leaf
        //long offset_leaf = 0;
        long offset_leaf = find_leaf(key, meta.root);
        //cout << offset_leaf << endl;
        //the key is the smallest
        if(offset_leaf == 0 || meta.size == 0){//only when the key is the smallest
            //cout << "更新最开始父亲节点" << endl;
            readfile(&leaf, meta.firstleaf, 1, sizeof(leaf_node<Key, Value>));//先读如第一片叶子
            bool judge = insert_in_leaf(leaf, key, value);//将东西插入其中
            if(!judge) return false;
            //writefile(&leaf, meta.firstleaf, 1, sizeof(leaf_node<Key, Value>));
            long offset_father = leaf.father;
            while(offset_father != 0){//开始更新父亲的key值
                internal_node<Key, Value> father;
                readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
                father.index[0].first = key;
                writefile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
                offset_father = father.father;
            }
            meta.size++;
            writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
        }
        else{
            //read in the leaf
            readfile(&leaf, offset_leaf, 1, sizeof(leaf_node<Key, Value>));

            //insert in the leaf
            bool judge = insert_in_leaf(leaf, key, value);
            if(!judge) return false;
            else {
                meta.size++;
                writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
            }
        }
        return true;
    }

    bool change_value(const Key &key, const Value &value){
        long offset = 0;
        offset = find_leaf(key, meta.root);
        if(offset == 0) {
            return false;
        }//there is no such range in the root
        else{
            return change_in_leaf(key, value, offset);
        }
    }

    bool erase(const Key &key){
        leaf_node<Key, Value> leaf;
        pair<Value, bool> judge;
        //if(key == debug) cout << "fuck0" << endl;
        judge = find(key);
        if(judge.second == false) return false;
        //if(key == debug) cout << "fuck0" << endl;
        long offset_leaf = find_leaf(key, meta.root);
        readfile(&leaf, offset_leaf, 1, sizeof(leaf_node<Key, Value>));
        //if(key == debug) cout << "fuck0" << endl;
        earse_in_leaf(leaf, key);//只是在叶子里面删掉这个东西，然后如果是第一个就向上传递而已
        //if(key == debug) cout << "fuck0" << endl;
        if(leaf.numOfpair < LEAST_PAIR_IN_LEAF_TRAIN){
            //if(key == debug) cout << "fuck0" << endl;
            if(borrow_from_rightleaf(leaf)){//向同父右兄弟借

            }
            else{
                //if(key == debug) cout << "fuck1" << endl;
                if(borrow_from_leftleaf(leaf)){//向同父做兄弟借

                }
                else{//if(key == debug) cout << "fuck2" << endl;

                    if(merge_rightleaf(leaf)){//融合右兄弟，融合成功后看看是否需要调整node

                    }
                    else{//if(key == debug) cout << "fuck3" << endl;
                        if(merge_leftleaf(leaf)){//融合左兄弟，融合成功后看看是否需要调整nod
                        }
                        else{//一个节点没有兄弟，这种情况是不可能存在的
                            //throw "what a stupid situation";
                            //cout << "here" << endl;
                            writefile(&leaf, offset_leaf, 1, sizeof(leaf_node<Key, Value>));
                        }
                    }
                }
            }
        }
        else writefile(&leaf, offset_leaf, 1, sizeof(leaf_node<Key, Value>));//把叶子结点写进去
        meta.size--;//改变meta的大小
        writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
        return true;
    }


    pair<Value, bool> find(const Key &key){//if find return <value, 1>; not find return <Value(), 0>
        long offset = 0;
        //if(key == debug) cout << "fuckfind" << endl;
        offset = find_leaf(key, meta.root);
        if(offset == 0) {
            Value v;
            pair<Value, bool> p(v, false);
            return p;
        }//there is no such range in the root
        else{
            pair<Value, bool> p = find_pos_in_leaf(key, offset);
            return p;
        }
    }

    void writeleaf(){
        leaf_node<Key, Value> leaf;
        writefile(&leaf, 0, 1, sizeof(leaf_node<Key, Value>));
    }
    void writenode(){
        internal_node<Key, Value> node;
        writefile(&node, 0, 1, sizeof(internal_node<Key, Value>));
    }



    void build_empty_tree(){
        meta.root = OFFSET_META + sizeof(bpt_meta);
        meta.firstleaf = OFFSET_META + sizeof(bpt_meta) + sizeof(internal_node<Key, Value>);
        meta.size = 0;
        internal_node<Key, Value> root;//create a root
        leaf_node<Key, Value> leaf;
        root.father = 0;//no father
        root.offset = meta.root;//its offset
        root.numOfkey = 1;//no key
        root.type = 1;
        root.index[0].second = meta.firstleaf;

        leaf.offset = meta.firstleaf;//its offset
        leaf.father = meta.root;//its father
        leaf.numOfpair = 0;//no son
        leaf.next = 0;
        //leaf.pre = 0;
        meta.solt = meta.firstleaf + sizeof(leaf_node<Key, Value>);
        writefile(&root, root.offset, 1, sizeof(internal_node<Key, Value>));
        writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));
        writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
    }









  private:

    int cmp(Key a, Key b){
        if(a < b){//a < b
            return 1;
        }
        if(a > b){//a > b
            return -1;
        }
        else return 0;//==
    }


    long find_leaf(const Key &key, long offset){//find until to the leaf and return the offset of the leaf
        internal_node<Key, Value> node;
        readfile(&node, offset, 1, sizeof(internal_node<Key, Value>));
        //if(key == debug)cout << "find begin" << endl;
        if(node.type == 1){//means it is going to the leaf
            int r = node.numOfkey - 1;
            int l = 0;
            //if(key == debug) cout << "find111 for 222" << endl;
            int x = node_binary_search(node.index, l, r, key);
            //cout << "leaf " << x << endl;
            if(x < 0){
                return 0;
            }
            //if(key == debug) cout << "find111 for" << node.index[x].first << endl;
            return node.index[x].second;//it finds the leaf
        }
        else{//means there maybe go to next node
            int r = node.numOfkey - 1;
            int l = 0;
            int x = node_binary_search(node.index, l, r, key);
            //cout << " node " << x << endl;
            if(x < 0){
                return 0;
            }
            //if(key == debug) cout << "find for" << node.index[x].first << "\t" << node.index[x].second << endl;
            return find_leaf(key, node.index[x].second);
        }
    }

    pair<Value, bool> find_pos_in_leaf(const Key &key, long offset){//find in the leaf of offset and return the value and bool pair
        leaf_node<Key, Value> leaf;
        readfile(&leaf, offset, 1, sizeof(leaf_node<Key, Value>));
        int r = leaf.numOfpair - 1;
        int l = 0;
        int x = leaf_binary_search(leaf.children, l, r, key);
        if(leaf.children[x].first == key) {pair<Value, bool> p(leaf.children[x].second, true); return p;}
        else {pair<Value, bool> p(leaf.children[x].second, false); return p;}
    }

    bool change_in_leaf(const Key &key, const Value &value, long offset){
        leaf_node<Key, Value> leaf;
        readfile(&leaf, offset, 1, sizeof(leaf_node<Key, Value>));
        int r = leaf.numOfpair - 1;
        int l = 0;
        int x = leaf_binary_search(leaf.children, l, r, key);
        if(leaf.children[x].first == key) {
            leaf.children[x].second = value;
            writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));
            return true;
        }
        else {
            return false;
        }
    }


    //search in the leaf for which pair, if there is not such pair then goto the first pair which key is smaller than it
    int leaf_binary_search(const pair<Key, Value> p[], int l, int r, Key key){
        int ans = 0;
        if(cmp(p[l].first, p[r].first) == -1){
            return -1;
        }
        if(cmp(p[r].first , key) != -1){
            return r;
        }
        while(l <= r){
            int mid = (l + r) / 2;
            if(cmp(p[mid].first , key) == -1){
                ans = mid;
                r = mid - 1;
            }
            else{
                l = mid + 1;
            }
        }
        return ans - 1;
    }



    //search in the internal key for which pair, if there is not such pair then goto the first pair which key is smaller than it
    int node_binary_search(const pair<Key, long> p[], int l, int r, Key key){
        int ans = 0;
        if(cmp(p[l].first, p[r].first) == -1){
            return 0;
        }
        if(cmp(p[r].first , key) != -1){
            return r;
        }
        while(l <= r){
            int mid = (l + r) / 2;
            //if(key == debug) cout << "find111 for ssss" << mid << l << r << endl;
            if(cmp(p[mid].first , key) == -1){
                ans = mid;
                r = mid - 1;
            }
            else{
                l = mid + 1;
            }
        }
        return ans - 1;
    }


    bool borrow_from_rightleaf(leaf_node<Key, Value> &leaf){//向右边叶子借东西
        long offset_right = leaf.next;
        if(offset_right == 0) return false;
        leaf_node<Key, Value> rightleaf;
        readfile(&rightleaf, offset_right, 1, sizeof(leaf_node<Key, Value>));
        if(rightleaf.father != leaf.father){//右兄弟是别人的
            return false;
        }
        if(rightleaf.numOfpair == 0){
            return false;
        }
        if(rightleaf.numOfpair <= LEAST_PAIR_IN_LEAF_TRAIN){//如果右边叶子贫穷，那么就媒介成功
            return false;
        } else{
            //borrow from right
            leaf.children[leaf.numOfpair] = rightleaf.children[0];
            leaf.numOfpair++;
            for(int i = 0; i <= rightleaf.numOfpair - 2; i++){//每个都向前移动一个
                rightleaf.children[i] = rightleaf.children[i + 1];
            }
            rightleaf.numOfpair--;

            internal_node<Key, Value> father;//改变父亲里面的索引
            long offset_father = rightleaf.father;
            readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
            int x = node_binary_search(father.index, 0, father.numOfkey - 1, leaf.children[leaf.numOfpair - 1].first);
            father.index[x].first = rightleaf.children[0].first;
            writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));

            writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));
            writefile(&rightleaf, offset_right, 1, sizeof(leaf_node<Key, Value>));
            return true;
        }
    }

    bool borrow_from_leftleaf(leaf_node<Key, Value> &leaf){//向左兄弟借东西
        internal_node<Key, Value> father;
        readfile(&father, leaf.father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, leaf.children[0].first);
        //if(leaf.children[0].first == 999) cout << "shit" << x << endl;
        if(x <= 0) return false;//没有左边的兄弟
        leaf_node<Key, Value> leftleaf;
        readfile(&leftleaf, father.index[x - 1].second, 1, sizeof(leaf_node<Key, Value>));
        if(leftleaf.numOfpair <= LEAST_PAIR_IN_LEAF_TRAIN){ return false;}//左兄弟很贫穷
        for(int i = leaf.numOfpair; i > 0; i--){
            leaf.children[i] = leaf.children[i - 1];
        }
        leaf.children[0] = leftleaf.children[leftleaf.numOfpair - 1];
        leaf.numOfpair++;
        leftleaf.numOfpair--;
        father.index[x].first = leaf.children[0].first;//更新一下父亲的索引，既然有左兄弟，肯定不可能是第一个，不用向上更新
        writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));

        writefile(&leaf, leaf.offset,1 ,sizeof(leaf_node<Key, Value>));
        writefile(&leftleaf, leftleaf.offset, 1, sizeof(leaf_node<Key, Value>));
        return true;
    }

    //merge the right leaf
    bool merge_rightleaf(leaf_node<Key, Value> &leaf){//融合右节点
        long offset_right = leaf.next;
        if(offset_right == 0){//右节点不存在
            /*writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));
            if(leaf.numOfpair == 0){//开始向上传递
                long offset_father = leaf.father;
                while(true){
                    internal_node<Key, Value> node;
                    readfile(&node, offset_father, 1, sizeof(internal_node<Key, Value>));
                    node.numOfkey--;
                    writefile(&node, offset_father, 1, sizeof(internal_node<Key, Value>));
                    if(node.numOfkey == 0){
                        offset_father = node.father;
                        if(offset_father == 0) break;
                    }
                    else break;
                }
            }
            return;*/
            return false;
        }
        leaf_node<Key, Value> rightleaf;
        readfile(&rightleaf, offset_right, 1, sizeof(leaf_node<Key, Value>));
        if(rightleaf.father != leaf.father){//右兄弟是别人的
            return false;
        }
        Key ori = rightleaf.children[0].first;
        for(int i = 0; i < rightleaf.numOfpair; i++){//将右兄弟的东西搬到leaf里面去
            leaf.children[leaf.numOfpair + i] = rightleaf.children[i];
        }
        leaf.numOfpair += rightleaf.numOfpair;
        leaf.next = rightleaf.next;
        internal_node<Key, Value> father;
        long offset_father = rightleaf.father;
        readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, ori);//找到右兄弟的位置
        for(int i = x; i < father.numOfkey - 1; i++){
            father.index[i] =  father.index[i + 1];
        }
        father.numOfkey--;//改变父亲节点

        writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));//转移结束

        if(keep_node(father)){//让父亲节点保持清爽
            //cout << "kepp" << endl;
        }
        else{
            writefile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
        }
        return true;
    }

    bool merge_leftleaf(leaf_node<Key, Value> leaf){//融合到左边的叶子里面去
        internal_node<Key, Value> father;//在father中找到位置，目的是找到前一个
        readfile(&father, leaf.father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, leaf.children[0].first);
        if(x <= 0) return false;

        leaf_node<Key, Value> leftleaf;//找到前一片叶子，然后开始进行搬迁，将node移动到leafnode那里去
        readfile(&leftleaf, father.index[x - 1].second, 1, sizeof(leaf_node<Key, Value>));
        for(int i = 0; i < leaf.numOfpair; i++){
            leftleaf.children[i + leftleaf.numOfpair] = leaf.children[i];
        }
        leftleaf.numOfpair += leaf.numOfpair;
        leftleaf.next = leaf.next;

        for(int i = x; i < father.numOfkey - 1; i++){
            father.index[i] = father.index[i + 1];
        }
        father.numOfkey--;
        writefile(&leftleaf, leftleaf.offset, 1, sizeof(leaf_node<Key, Value>));
        if(keep_node(father)){

        }
        else{
            writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));
        }
        return true;
    }



    bool keep_node(internal_node<Key, Value> &node){
        if(node.father == 0){//是根节点就什么都不用干
            return false;
        }
        if(node.numOfkey < LEAST_KEY_IN_INTER_TRAIN){
            if(borrow_from_rightnode(node)){
                //cout << "1" << endl;
                return true;
            }
            else{
                if(borrow_from_leftnode(node)){
                    //cout << "2" << endl;
                    return true;
                }
                else {
                    if(merge_right_node(node)){
                        //cout << "3" << endl;
                        return true;
                    }
                    else{
                        if(merge_left_node(node)){
                            //cout << "4" << endl;
                            return true;
                        }
                        else{//此时父亲节点只有一个了，说明可以越过父亲节点了，将节点与祖父节点连起来
                            internal_node<Key, Value> father;
                            readfile(&father, node.father, 1, sizeof(internal_node<Key, Value>));
                            if(father.father == 0) {//如果father是根的话，说明就只有一个在根里面了，可以改变根
                                //cout << "king" << endl;
                                meta.root = node.offset;
                                writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
                                node.father = 0;
                                writefile(&node, node.offset, 1, sizeof(internal_node<Key, Value>));
                            }
                            else{
                                internal_node<Key, Value> grandfather;
                                readfile(&grandfather, father.father, 1, sizeof(internal_node<Key, Value>));
                                int x = node_binary_search(grandfather.index, 0, grandfather.numOfkey - 1, father.index[0].first);
                                grandfather.index[x].second = node.offset;
                                node.father = grandfather.offset;
                                writefile(&grandfather, grandfather.offset, 1, sizeof(internal_node<Key, Value>));
                                writefile(&node, node.offset, 1, sizeof(internal_node<Key, Value>));
                            }
                            return true;
                        }
                    }
                }
            }
        }
        else return false;
    }


    bool borrow_from_rightnode(internal_node<Key, Value> &node){//从右侧节点借东西
        internal_node<Key, Value> father;
        long offset_father = node.father;
        if(offset_father == 0) return false;//发现是根节点
        readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, node.index[0].first);
        if(x == father.numOfkey - 1) return false;//发现已经是最右边了
        long offset_right = father.index[x + 1].second;//找到右侧节点
        internal_node<Key, Value> rightnode;
        readfile(&rightnode, offset_right, 1, sizeof(internal_node<Key, Value>));

        if(rightnode.numOfkey <= LEAST_KEY_IN_INTER_TRAIN){//如果右边的兄弟贫穷了，就算了
            return false;
        }
        else{
            node.index[node.numOfkey] = rightnode.index[0];//把东西接过来
            node.numOfkey++;
            for(int i = 0; i <= rightnode.numOfkey - 2; i++){//把右边的节点调整一下
                rightnode.index[i] = rightnode.index[i + 1];
            }
            rightnode.numOfkey--;

            if(rightnode.type == 0){
                internal_node<Key, Value> son;
                readfile(&son, node.index[node.numOfkey - 1].second, 1, sizeof(internal_node<Key, Value>));
                son.father = node.offset;
                writefile(&son, node.index[node.numOfkey - 1].second, 1, sizeof(internal_node<Key, Value>));
            }
            else{
                leaf_node<Key, Value> son;
                readfile(&son, node.index[node.numOfkey - 1].second, 1, sizeof(leaf_node<Key, Value>));
                son.father = node.offset;
                writefile(&son, node.index[node.numOfkey - 1].second, 1, sizeof(leaf_node<Key, Value>));
            }

            /*offset_father = rightnode.father;
            readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
            int xx = node_binary_search(father.index, 0, father.numOfkey - 1, node.index[node.numOfkey - 1].first);
            father.index[xx].first = rightnode.index[0].first;
            writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));*/
            father.index[x + 1].first = rightnode.index[0].first;//更新父亲的索引
            writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));

            writefile(&node, node.offset, 1, sizeof(leaf_node<Key, Value>));
            writefile(&rightnode, rightnode.offset, 1, sizeof(leaf_node<Key, Value>));
            return true;
        }
    }

    bool borrow_from_leftnode(internal_node<Key, Value> &node){
        //cout << "begin" << endl;
        internal_node<Key, Value> father;
        long offset_father = node.father;
        if(offset_father == 0) return false;//是根节点所以爆炸了
        readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, node.index[0].first);
        if(x == 0) return false;
        long offset_left = father.index[x - 1].second;//找到了左边的
        internal_node<Key, Value> leftnode;
        readfile(&leftnode, offset_left, 1, sizeof(internal_node<Key, Value>));

        if(leftnode.numOfkey <= LEAST_PAIR_IN_LEAF_TRAIN){//左边的很贫穷就算了
            return false;
        } else{
            for(int i = node.numOfkey; i > 0; i--){//将左边最后一个接过来
                node.index[i] = node.index[i - 1];
            }
            node.index[0] = leftnode.index[leftnode.numOfkey - 1];
            node.numOfkey++;
            leftnode.numOfkey--;

            if(node.type == 0){
                internal_node<Key, Value> son;
                readfile(&son, node.index[0].second, 1, sizeof(internal_node<Key, Value>));
                son.father = node.offset;
                writefile(&son, node.index[0].second, 1, sizeof(internal_node<Key, Value>));
            }
            else{
                leaf_node<Key, Value> son;
                readfile(&son, node.index[0].second, 1, sizeof(leaf_node<Key, Value>));
                son.father = node.offset;
                writefile(&son, node.index[0].second, 1, sizeof(leaf_node<Key, Value>));
            }

            //改变索引值
            readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
            father.index[x].first = node.index[0].first;
            writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));

            writefile(&node, node.offset, 1, sizeof(leaf_node<Key, Value>));
            writefile(&leftnode, leftnode.offset, 1, sizeof(leaf_node<Key, Value>));
            return true;
        }
    }

    bool merge_left_node(internal_node<Key, Value> &node){//融合到左兄弟
        if(node.father == 0) return false;//跟节点就没什么好说的了
        internal_node<Key, Value> father;
        readfile(&father, node.father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, node.index[0].first);
        if(x == 0) return false;
        internal_node<Key, Value> leftnode;
        readfile(&leftnode, father.index[x - 1].second, 1, sizeof(internal_node<Key, Value>));
        for(int i = 0; i < node.numOfkey; i++){
            leftnode.index[i + leftnode.numOfkey] = node.index[i];
            if(leftnode.type == 0){
                internal_node<Key, Value> son;
                readfile(&son, node.index[i].second, 1, sizeof(internal_node<Key, Value>));
                son.father = leftnode.offset;
                writefile(&son, node.index[i].second, 1, sizeof(internal_node<Key, Value>));
            }
            else{
                leaf_node<Key, Value> son;
                readfile(&son, node.index[i].second, 1, sizeof(leaf_node<Key, Value>));
                son.father = leftnode.offset;
                writefile(&son, node.index[i].second, 1, sizeof(leaf_node<Key, Value>));
            }
        }
        leftnode.numOfkey += node.numOfkey;
        for(int i = x; i <= father.numOfkey - 2; i++){
            father.index[i] = father.index[i + 1];
        }
        father.numOfkey--;
        writefile(&leftnode, leftnode.offset, 1, sizeof(internal_node<Key, Value>));
        if(keep_node(father)){
        }
        else writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));
        return true;
    }

    bool merge_right_node(internal_node<Key, Value> &node){
        if(node.father == 0) return false;
        internal_node<Key, Value> father;
        readfile(&father, node.father, 1, sizeof(internal_node<Key, Value>));
        int x = node_binary_search(father.index, 0, father.numOfkey - 1, node.index[0].first);
        if(x == father.numOfkey - 1) return false;
        internal_node<Key, Value> rightnode;
        readfile(&rightnode, father.index[x + 1].second, 1, sizeof(internal_node<Key, Value>));
        for(int i = 0; i < rightnode.numOfkey; i++){//把右边节点东西搬过来
            node.index[i + node.numOfkey] = rightnode.index[i];
            if(rightnode.type == 0){
                internal_node<Key, Value> son;
                readfile(&son, rightnode.index[i].second, 1, sizeof(internal_node<Key, Value>));
                son.father = node.offset;
                writefile(&son, rightnode.index[i].second, 1, sizeof(internal_node<Key, Value>));
            }
            else{
                leaf_node<Key, Value> son;
                readfile(&son, rightnode.index[i].second, 1, sizeof(leaf_node<Key, Value>));
                son.father = node.offset;
                writefile(&son, rightnode.index[i].second, 1, sizeof(leaf_node<Key, Value>));
            }
        }
        node.numOfkey += rightnode.numOfkey;
        for(int i = x + 1; i <= father.numOfkey - 2; i++){//调整父亲
            father.index[i] = father.index[i + 1];
        }
        father.numOfkey--;
        writefile(&node, node.offset, 1, sizeof(internal_node<Key, Value>));
        if(keep_node(father)){
        }
        else writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));
        return true;
    }

    void earse_in_leaf(leaf_node<Key, Value> &leaf, const Key &key){
        int x = leaf_binary_search(leaf.children, 0, leaf.numOfpair - 1, key);
        for(int i = x; i < leaf.numOfpair - 1; i++){
            leaf.children[i] = leaf.children[i + 1];
        }
        leaf.numOfpair--;
        internal_node<Key, Value> father;
        long offset_father = leaf.father;
        //Key k = key;
        while(true){
            if(offset_father == 0) break;
            if(x != 0) break;
            readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
            x = node_binary_search(father.index, 0, father.numOfkey - 1, key);
            father.index[x].first = leaf.children[0].first;
            writefile(&father, father.offset, 1, sizeof(internal_node<Key, Value>));
            offset_father = father.father;
        }
    }



    bool insert_in_leaf(leaf_node<Key, Value> &leaf, const Key &key, const Value &value){
        int x = leaf_binary_search(leaf.children, 0, leaf.numOfpair - 1, key);//to find the last thing smaller than it
        if(leaf.children[x].first == key) return false;
        for(int i = leaf.numOfpair; i >= x + 2; i--){
            leaf.children[i] = leaf.children[i - 1];
        }
        leaf.children[x + 1].first = key;
        leaf.children[x + 1].second = value;
        leaf.numOfpair++;//把东西插入leaf
        if(leaf.numOfpair == PAIR_IN_LEAF_TRAIN){
            spilt_leaf(leaf);//分裂leaf
        }
        else{
            writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));//写入leaf
        }
        return true;
    }

    void insert_in_node(internal_node<Key, Value> &node, const Key &key, const long &offset){
        int x = node_binary_search(node.index, 0, node.numOfkey - 1, key);//to find the last key smaller than it
        for(int i = node.numOfkey; i >= x + 2; i--){
            node.index[i] = node.index[i - 1];
        }
        node.index[x + 1].first = key;
        node.index[x + 1].second = offset;
        node.numOfkey++;//把东西插入node
        if(node.numOfkey == KEY_IN_INTER_TRAIN){
            spilt_inter(node);//分裂node
        }
        else{
            writefile(&node, node.offset, 1, sizeof(internal_node<Key, Value>));//写入node
        }
    }



    void spilt_leaf(leaf_node<Key, Value> &leaf){//将一个饱和的叶子分成两个,然后改变放入他的父亲中，如果父亲满了就向上分裂
        //newleaf.pre = leaf.offset;
        leaf_node<Key, Value> newleaf;//create a new leaf
        newleaf.numOfpair = (int)(PAIR_IN_LEAF_TRAIN / 2 + 0.999);
        leaf.numOfpair -= newleaf.numOfpair;
        newleaf.father = leaf.father;//享有共同的父亲
        newleaf.offset = meta.solt;//记录新叶子的地址
        meta.solt += sizeof(leaf_node<Key, Value>);//改变meta的solt
        newleaf.next = leaf.next;
        leaf.next = newleaf.offset;//完成链式传递
        for(int i = 0; i < newleaf.numOfpair; i++){
            newleaf.children[i] = leaf.children[i + leaf.numOfpair];
        }//将节点里面的东西搬过去

        //写入
        writefile(&leaf, leaf.offset, 1, sizeof(leaf_node<Key, Value>));
        writefile(&newleaf, newleaf.offset, 1, sizeof(leaf_node<Key, Value>));
        writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));

        //check its father
        internal_node<Key, Value> father;
        long offset_father = leaf.father;
        readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
        insert_in_node(father, newleaf.children[0].first, newleaf.offset);//将新叶子插入到父亲节点里面去，插入的是新节点的key值和地址
        /*for(int i = 0; i < KEY_IN_INTER; i++){
            cout << father.index[i].first << ' ' << father.index[i].second << endl;
        }*/
    }


    //spilt internal node
    void spilt_inter(internal_node<Key, Value> &node){//分裂节点
        //cout << "spilt the father" << endl;
        if(node.offset == meta.root){//如果它是跟节点的话
            //cout << "change root" << endl;
            internal_node<Key, Value> newroot;//there must need a new root

            internal_node<Key, Value> newnode;//there must need a new node

            //spilt the node
            newnode.numOfkey = (int)(KEY_IN_INTER_TRAIN / 2 + 0.999);
            node.numOfkey -= newnode.numOfkey;
            newnode.father = node.father;
            newnode.type = node.type;
            newnode.offset = meta.solt;//confirm the node's offset
            //change the solt
            meta.solt += sizeof(internal_node<Key, Value>);
            for(int i = 0; i < newnode.numOfkey; i++){
                newnode.index[i] = node.index[node.numOfkey + i];
                if(newnode.type == 1){//它下面的是叶子节点
                    leaf_node<Key, Value> son;
                    readfile(&son, newnode.index[i].second, 1, sizeof(leaf_node<Key, Value>));
                    //cout << son.offset << " leaf change father " << newnode.offset << endl;
                    son.father = newnode.offset;
                    writefile(&son, newnode.index[i].second, 1, sizeof(leaf_node<Key, Value>));
                }
                else{
                    internal_node<Key, Value> son;
                    readfile(&son, newnode.index[i].second, 1, sizeof(internal_node<Key, Value>));
                    //cout << son.offset << " node change father " << newnode.offset << endl;
                    son.father = newnode.offset;
                    writefile(&son, newnode.index[i].second, 1, sizeof(internal_node<Key, Value>));
                }
            }



            //make a new root
            newroot.index[0].first = node.index[0].first;
            newroot.index[0].second = node.offset;
            newroot.index[1].first = newnode.index[0].first;
            newroot.index[1].second = newnode.offset;
            newroot.offset = meta.solt;
            //change the solt
            meta.solt += sizeof(internal_node<Key, Value>);
            newnode.father = newroot.offset;
            node.father = newroot.offset;
            newroot.type = 0;
            newroot.father = 0;
            newroot.numOfkey = 2;
            meta.root = newroot.offset;



            //write to the file
            writefile(&node, node.offset,  1, sizeof(internal_node<Key, Value>));
            writefile(&newnode, newnode.offset, 1, sizeof(internal_node<Key, Value>));
            writefile(&newroot, newroot.offset, 1, sizeof(internal_node<Key, Value>));
            writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));
        }
        else{
            internal_node<Key, Value> newnode;//create a new node
            newnode.numOfkey = (int)(KEY_IN_INTER_TRAIN / 2 + 0.999);
            node.numOfkey -= newnode.numOfkey;
            newnode.father = node.father;
            newnode.type = node.type;
            newnode.offset = meta.solt;//confirm the node's offset
            //change the solt
            meta.solt += sizeof(internal_node<Key, Value>);
            for(int i = 0; i < newnode.numOfkey; i++){
                newnode.index[i] = node.index[node.numOfkey + i];
                if(newnode.type == 1){
                    leaf_node<Key, Value> son;
                    readfile(&son, newnode.index[i].second, 1, sizeof(leaf_node<Key, Value>));
                    //cout << son.offset << " leaf change his father to " << newnode.offset << endl;
                    son.father = newnode.offset;
                    writefile(&son, son.offset, 1, sizeof(leaf_node<Key, Value>));
                }
                else{
                    internal_node<Key, Value> son;
                    readfile(&son, newnode.index[i].second, 1, sizeof(internal_node<Key, Value>));
                    //cout << son.offset << " node change his father to " << newnode.offset << endl;
                    son.father = newnode.offset;
                    writefile(&son, son.offset, 1, sizeof(internal_node<Key, Value>));
                }
            }


            //write to the file
            writefile(&node, node.offset,  1, sizeof(internal_node<Key, Value>));
            writefile(&newnode, newnode.offset, 1, sizeof(internal_node<Key, Value>));
            writefile(&meta, OFFSET_META, 1, sizeof(bpt_meta));

            long offset_father = node.father;
            internal_node<Key, Value> father;
            readfile(&father, offset_father, 1, sizeof(internal_node<Key, Value>));
            insert_in_node(father, newnode.index[0].first, newnode.offset);
        }

    }



    bool fp_level = 0;//fp_level = 1 means the file is open; else it's close

    size_t readfile(void *place, long offset, size_t numOfblock, size_t sizeOfblock){
        //place is where to keep the data, offset is the offset, numOfblock is read how much such block, sizeOfblock is to read how much big
#ifndef NO_OPEN
        open_file();
#endif
        int judge;
        judge = fseek(fp, offset, 0);//find the correct pos
        if(judge != 0)throw "fuck";//if don't find, then throw
        size_t numOfcorrect;
        numOfcorrect = fread(place, numOfblock, sizeOfblock,fp);//read from the file
#ifndef NO_OPEN
        close_file();
#endif
        return numOfcorrect;
    }
    size_t writefile(void *place, long offset, size_t numOfblock, size_t sizeOfblock){
        //place is where has the data, offset is the offset, numOfblock is write how much such block, sizeOfblock is to write how much big
#ifndef NO_OPEN
        open_file();
#endif
        int judge;
        judge = fseek(fp, offset, 0);//find the right pos
        if(judge != 0)throw "fuck";//if don't find, then throw
        size_t numOfcorrect;
        numOfcorrect = fwrite(place, numOfblock, sizeOfblock, fp);//write into the file
#ifndef NO_OPEN
        close_file();
#endif
        return numOfcorrect;
    }

};
}

#endif //TRAIN_BPLUSTREEFORTRAIN_H
