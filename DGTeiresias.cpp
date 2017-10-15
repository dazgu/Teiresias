
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include "DGTeiresias.h"
#include "DG_util.h"

using namespace std;


Teiresias::Teiresias(int argc, char *argv[], int w, int l, int k, int L, std::ostream &g)
:input(argc,argv),stack(g),out(g)
{
    this->w = w;
    this->l = l;
    this->k = k;
    this->L = L;
    d = w-l;
    ht = new map<string,list<int> *,Lessthan>[SIZE];
    stack.b = input.get_boundaries();
}


Teiresias::~Teiresias()
{
    delete[] ht;
}


int Teiresias::key(const std::string &s)
{
    int total=0;
    for(size_t i=0; i<10; ++i){
        if(i>=s.length()) break;
        int d;
        switch(s[i]){
            case 'A': d=0; break;
            case 'C': d=1; break;
            case 'G': d=2; break;
            case 'T': d=3; break;
            default: d=4;
        }
        total = total*5 + d;
    }
    return total%SIZE;
}


void Teiresias::nk(const string &ep, int start, int pr=0, int i=0)
{
    if(i < d){
        for(int j=pr+1; j<l+i+1; ++j){
            string s = ep;
            s[j]=DOT;
            nk(s, start, j,i+1);
        }
        return;
    }
    int val = key(ep);
    map<string,list<int> *,Lessthan>::iterator p = ht[val].find(ep);
    if(p!=ht[val].end()) p->second->push_back(start);
    else ht[val][ep]=new list<int>(1,start);
}


void Teiresias::run()
{
    int pp=0, percent;
    size_t len = input.length();
    for(size_t start=0; start< len-w+1; ++start){
        percent = int(start*100/len);
        if(percent==pp+5){
            cerr << percent << "%\t" << int(start) << "/" << len << endl;
            pp=percent;
        }
        nk(input.get_seq().substr(start, w), int(start));
    }

    map<string,list<int> *,Lessthan> ep;
    for(int i=0; i<SIZE; ++i){
        if(ht[i].empty()) continue;
        for(map<string,list<int> *,Lessthan>::iterator p=ht[i].begin();
            p!=ht[i].end();++p)
        {
            if(p->second->size()<size_t(k)){
                delete p->second;
                continue;
            }
            ep.insert(*p);
        }
        ht[i].clear();
    }

    stack.build(ht,ep,w,l,k,L,input.get_seq());
    ep.clear();
    stack.begin();
}
