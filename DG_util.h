#ifndef UTIL
#define UTIL
#include <vector>
#include <string>

struct Lessthan
{
    bool operator ()(const std::string &l, const std::string &r) const
    {
        size_t n=l.find('.');
        if(n==std::string::npos) n=l.length();
        size_t m=r.find('.'); 
        if(m==std::string::npos) m=r.length();            
        if(n==m) return l < r;
        return n>m;
    }
};

class FileProcessor
{
    int nseqs;
    std::vector<std::string> headers;
    std::vector<int> boundaries;
    std::string seq;
    std::string reversecomplement(const std::string &s) const;
    void GetUpper(std::string& str) const;
    
public:
    FileProcessor(int argc, char *argv[]);
    FileProcessor(){}

    static std::string getkey(int argc, char *argv[], std::string key, 
        bool isflag=false, std::string def=std::string());

    int size() const {return nseqs;}
    const std::vector<std::string> &get_headers() const { return headers; }
    const std::string &get_header(int n) const { return headers[n]; }
    const std::vector<int> &get_boundaries() const { return boundaries; }
    const std::string operator [](int n) const;
    int length(int n) const;
    size_t length() const {return seq.length();}
    const std::string &get_seq() const {return seq;}
    void print() const;
};
#endif
