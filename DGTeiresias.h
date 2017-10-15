#ifndef TEIRESIAS
#define TEIRESIAS
class Teiresias;

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include "DGStack.h"
#include "DG_util.h"

#define SIZE 30000
// #define DOT char(0xAA)
#define DOT char('_')

class Teiresias
{   
    std::map<std::string,std::list<int> *,Lessthan> *ht;
    TStack stack;
    std::string seq;
    int w,l,k,d,L;
    std::ostream &out;
    FileProcessor input;

    void nk(const std::string &ep, int start, int pr, int i);

public:
    Teiresias(int argc, char *argv[], int w, int l, int k, int L, std::ostream &g);
    ~Teiresias();
    static int key(const std::string &s);
    void run();
};

#endif

