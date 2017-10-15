#include <string>
#include <iostream>
#include <ctype.h>
#include <fstream>
#include "DG_util.h"
using namespace std;


std::string FileProcessor::getkey(int argc, char *argv[], 
                                  std::string key, bool isflag, std::string def)
{
    for(int i=0; i<argc; ++i){
        if(key!=argv[i]) continue;
        if(isflag) return "present";
        else if(i<argc-1) return argv[i+1];
        else break;
    }
    if(!isflag){
        if(def.empty()) throw key + " not found on command line";
        else return def;
    }
    return string();
}



FileProcessor::FileProcessor(int argc, char *argv[]):nseqs(0)
{
    string inputfile = getkey(argc, argv, "-input");
    bool bothstrands = getkey(argc, argv, "-r", true)=="present"? true: false;

    ifstream f(inputfile.c_str());
    if(!f.is_open()) throw "cannot open input file";
    string s;          
    while(!f.eof()){
        getline(f, s);
        if(s.length()==0) continue;
        size_t offset = s.find_last_not_of("\r\n\t ");                          
        if(offset==string::npos) continue;
        if(offset!=s.length()-1) s.erase(offset+1);

        if(s[0]=='>'){
            ++nseqs;
            headers.push_back(s.substr(1));
            boundaries.push_back(int(seq.length()));
            continue;
        } else if(nseqs==0){
            ++nseqs;
            headers.push_back("(empty header)");
            boundaries.push_back(0);
        }
        GetUpper(s);
        seq.append(s);
    }

    if(bothstrands){
        size_t osize=boundaries.size(), seqlen=seq.length();
        for(int n=int(osize)-1; n>=0; --n)
            boundaries.push_back(2*int(seqlen)-boundaries[n]-length(n));
        seq+=reversecomplement(seq);
        nseqs*=2;
        vector<string> h2;
        for(int n=int(headers.size())-1; n>=0; --n)
            h2.push_back(string("-")+headers[n]);
        headers.insert(headers.end(), h2.begin(), h2.end());
    }
}


const std::string FileProcessor::operator [](int n) const
{ 
    if(n>nseqs || n < 0) throw "operator [](): index out of bound";
    return seq.substr(boundaries[n], length(n));
}


int FileProcessor::length(int n) const
{
    if(n>nseqs || n < 0) throw "length(): index out of bound";
    return (n==nseqs-1?int(seq.length()):boundaries[n+1]) - boundaries[n];
}


void FileProcessor::print() const
{
    for(int i=0; i<size(); ++i)
        cout << "[" << i << "] " << length(i) << " " << get_header(i) << endl
        << (*this)[i] << endl << endl;
}


std::string FileProcessor::reversecomplement(const std::string &s) const
{
    string rc;
    for(string::const_reverse_iterator p=s.rbegin(); p!=s.rend();++p){
        switch(*p){
            case 'A': rc+='T'; break;
            case 'C': rc+='G'; break;
            case 'G': rc+='C'; break;
            case 'T': rc+='A'; break;
            case 'a': rc+='t'; break;
            case 'c': rc+='g'; break;
            case 'g': rc+='c'; break;
            case 't': rc+='a'; break;
            default: rc+=*p;
        };
    }
    return rc;
}

void FileProcessor::GetUpper(string& str) const
{
   for(int i = 0; i < str.length(); i++)
   {
      str[i] = toupper(str[i]);
   }
   return;
}
