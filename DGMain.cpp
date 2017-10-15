/******************************************************************************

    WordSearch/Teiresias
    Dazhang Gu
    $Log: DGMain.cpp,v $
    Revision 1.4  2007/09/10 14:40:39  gu
    *** empty log message ***

    Revision 1.3  2007/08/13 17:55:10  gu
    *** empty log message ***

    Revision 1.2  2007/08/13 17:46:39  gu
    *** empty log message ***

*******************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include "DGTeiresias.h"
#include "DG_util.h"
using namespace std;

int main(int argc, char *argv[])
{
    try {
        int w = atoi(FileProcessor::getkey(argc,argv,"-w").c_str());
        int l = atoi(FileProcessor::getkey(argc,argv,"-l").c_str());
        int k = atoi(FileProcessor::getkey(argc,argv,"-k").c_str());
        int L = atoi(FileProcessor::getkey(argc,argv,"-L",false,"0").c_str());
        string outfile = FileProcessor::getkey(argc,argv,"-output");
        ofstream g(outfile.c_str());
        if(!g.is_open()) throw "cannot open output file.";

        Teiresias ter(argc, argv, w, l, k, L, g);
        ter.run();
        cerr << "back in main!" << endl;
    } 
    catch(const string &msg) {
        cerr << endl << msg << endl;
        cerr << endl << "Command line arguments:" << endl << endl
            << "search_t -input <infile> -output <outfile> "
            << "-w <min wordlen> -l <literals> -k <min repeats> -L <min # seqs> "
            << "-r (both strands)"
            << endl;
        return 1;
    }
    return 0;
}
