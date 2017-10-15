#include <ctype.h>
#include <math.h>
#include <map>
#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "DGStack.h"
#include "DGTeiresias.h"
#include "DG_util.h"
using namespace std;

void TStack::build(M *m, const M &ep, int w, int l, int k, int L, const std::string &seq)
{
    this->w = w; this->l = l; this->k = k; this->L = L; this->m = m; 
    this->tl = int(seq.length());

    stack.reserve(ep.size());
    for(M::const_reverse_iterator p=ep.rbegin(); p!=ep.rend(); ++p)
    {
        E e;
        e.str = p->first;
        size_t id = e.str.find_last_not_of(DOT);
        if(id!=string::npos && id<e.str.length()-1) e.str.erase(id+1);
        for(list<int>::iterator q=p->second->begin(); q!=p->second->end();)
            if(in(*q,w)) ++q;
            else q=p->second->erase(q);

            if(p->second->size()>=size_t(k)){
                e.loc = p->second;
                stack.push_back(e);
            }
            else delete p->second;
    }
    sort();
}


void TStack::sort()
{
    if(stack.empty()) return;
    vector<int> id(stack.size());
    for(size_t i=0; i<id.size(); ++i) id[i]=(int)i;
    std::sort(id.begin(),id.end(),LS(this));
    sr = id[0];
    stack[id[0]].n = id.size()>1?id[1]:-1;
    stack[id[0]].p = -1;
    for(size_t i=1; i<id.size()-1; ++i){
        stack[id[i]].n = id[i+1];
        stack[id[i]].p = id[i-1];
    }
    stack[id[id.size()-1]].n = -1;
    stack[id[id.size()-1]].p = id.size()>1?id[id.size()-2]:-1;
}


bool TStack::in(int start, size_t len)
{
    size_t i;      
    for(i=0; i<b.size(); ++i) if(b[i]>start) break;
    if(i!=b.size() && start+len>size_t(b[i])) return false;
    return true;
}


E *TStack::icon(const E &el, const E &er, bool is=false)
{
    int overlap = l-1;
    int idx_l=int(el.str.length()), idx_r=-1;
    for(int i=0; i<overlap; ++i){
        idx_l = int(el.str.find_last_not_of(DOT,idx_l-1));
        idx_r = int(er.str.find_first_not_of(DOT,idx_r+1));
    }
    if(el.str.substr(idx_l)!=er.str.substr(0,idx_r+1)) return NULL;

    pe.loc = new list<int>(el.loc->size());
    list<int> right(*er.loc);
    for(list<int>::iterator p=right.begin();p!=right.end();++p) *p -= idx_l;
    list<int>::iterator end = set_intersection( el.loc->begin(), el.loc->end(), 
        right.begin(), right.end(), pe.loc->begin());
    pe.loc->erase(end, pe.loc->end());

    for(list<int>::iterator p=pe.loc->begin(); p!=pe.loc->end();)
        if(in(*p,idx_l+er.str.length())) ++p;
        else p=pe.loc->erase(p);

    if(pe.loc->size()<size_t(k)){
        delete pe.loc;
        return NULL;
    }
    pe.str = el.str + er.str.substr(idx_r+1);
    if(el==er || is){
        E copye=pe;
        E *r = icon(copye, er, true);
        if(r==NULL) pe=copye;
        else if(r->loc->size()<copye.loc->size()-1 && er.str[0]!='N'){
            delete pe.loc;
            pe=copye;
        }
    }
    return &pe;
}


void TStack::begin()
{
    while(!stack.empty()){
        pr = int(stack.size())-1;
        cerr << pr << endl;
        con0();
        E e=*stack.rbegin();
        if(e.p==-1) sr=e.n;
        else stack[e.p].n =e.n;
        if(e.n!=-1) stack[e.n].p=e.p;
        delete e.loc;
        stack.pop_back();
    }
}


void TStack::con0()
{
    E e=*stack.rbegin();
    for(int i=pr; i>=0; --i){
        E *combo = icon(e, stack[i]);
        if(combo==NULL) continue;
        stack.push_back(*combo);
        list<int> *p=combo->loc;
        con0();
        delete p;
        stack.pop_back();        
    }
    con1();
}


void TStack::con1()
{
    E e=*stack.rbegin();
    for(int i=sr; i!=-1; i=stack[i].n){
        if(i==int(stack.size())-1) continue;
        E *combo = icon(stack[i], e);
        if(combo==NULL) continue;
        stack.push_back(*combo);
        list<int> *p=combo->loc;
        con1();
        delete p;
        stack.pop_back();
    }
    //if(e.str.length()<size_t(L)) return;

    int val = Teiresias::key(e.str);
    M::iterator p = m[val].find(e.str); 
    if(p==m[val].end()){
        size_t seqs=0, lc=0;
        for(list<int>::iterator p=e.loc->begin(); p!=e.loc->end(); ++p){
            size_t c = lc;
            while(c<b.size() && b[c]<=*p) ++c;
            if(c!=lc){ ++seqs; lc=c; }
        }
        if(seqs<L) return;
        out << e.str << ',' << seqs << ',' << e.loc->size() << endl;
        m[val][e.str]=NULL;
    }
}


bool LS::operator()(int nl, int nr)
{
    const std::string &l = stack->stack[nl].str;
    const std::string &r = stack->stack[nr].str;
    size_t n=l.rfind(DOT);
    n = n==std::string::npos? l.length(): l.length()-1-n;
    size_t m=r.rfind(DOT);
    m = m==std::string::npos? r.length(): r.length()-1-m;
    if(n==m) return l < r;
    return n>m;
}


bool E::operator ==(const E &rhs) const
{
    return str==rhs.str && loc==rhs.loc;
}
