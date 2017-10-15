/******************************************************************************

    WordSearch/Teiresias
    Dazhang Gu
    $Log: DGStack.h,v $
    Revision 1.3  2007/08/13 17:55:10  gu
    *** empty log message ***

    Revision 1.2  2007/08/13 17:46:39  gu
    *** empty log message ***

*******************************************************************************/
#ifndef TSTACK
#define TSTACK
class TStack;

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include "DG_util.h"


struct E
{
    std::string str;
    std::list<int> *loc;
    int p, n;
    bool operator ==(const E &rhs) const;
};


class LS
{
    const TStack *stack;
public:
    LS(const TStack *s):stack(s){}
    bool operator()(int nl, int nr);
};


class TStack
{
    friend class Teiresias;
    friend class LS;
    int w,l,k,L,tl;
    std::vector<E> stack;
    void sort();                                          
    int sr;
    int pr;
    typedef std::map<std::string,std::list<int> *,Lessthan> M;
    M *m;
    E pe;
    std::ostream &out;
    std::vector<int> b;

    E *icon(const E &el, const E &er, bool is);
    bool in(int start, size_t len);
    void con0();
    void con1();

public:
    TStack(std::ostream &g):out(g){}
    void build(M *ht, const M &ep, int w, int l, int k, int L, const std::string &seq);
    void begin();
};


#endif
