#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstdlib> 
#include <vector>

using namespace std;

int main(int argc, char ** argv)
{
    string dataName = argv[1];
    int nProc = atoi(argv[2]);
    vector<ofstream*> fout;
    for (int i = 0; i < nProc; ++i){
        stringstream tmp;
        tmp << dataName << "_" << i;
        ofstream* ft = new ofstream(tmp.str().c_str());
        fout.push_back(ft);
    }
    fstream fin(dataName.c_str());
    string tmp;
    int count = 0;
    while (getline(fin, tmp)){
        (*(fout[(count++) % nProc])) << tmp << endl;
    }
    for (int i = 0; i < nProc; ++i){
        fout[i]->close();
        delete fout[i];
    }
    fin.close();
    return 0;
}