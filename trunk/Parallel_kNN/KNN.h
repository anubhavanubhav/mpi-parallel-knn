#ifndef _KNN_H
#define _KNN_H

#include "Similar.h"
#include "SimilarEuclid.h"
#include "SimilarCos.h"
#include "SimilarSet.h"
#include "DataStruct.h"

#include <queue>
#include <fstream>


using namespace std;

class KNN
{

public:

    typedef vector<Document> Doc;
    typedef priorityQueue<NN> NN_Queue;
    typedef vector<NN_Queue> TestNN;
    Similar* m_Sim;
    Doc m_Doc;
    int nnK;

public:

    KNN(int _k)
    {
        nnK = _k;
        m_Sim = new Similar();
    }

    KNN(int _k, string _simName)
    {
        nnK = _k;
        if (_simName == "Euclid")
        {
            m_Sim = new SimilarEuclid();
        }
        else if (_simName == "Cos")
        {
            m_Sim = new SimilarCos();
        }
        else if (_simName == "Set")
        {
            m_Sim = new SimilarSet();
        } 
        else
        {
            m_Sim = new Similar();
        }
    }

public:

    ~KNN(void)
    {
        delete m_Sim;
    }

public:

    void LoadTrainData(string _fileName)
    {
        ifstream fin(_fileName.c_str());
        string tmp;
        while (getline(fin, tmp))
        {
            Document doc;
            doc.parse(tmp);
            m_Doc.push_back(doc);
        }
        fin.close();
    }

    NN_Queue Test(Document doc)
    {
        NN_Queue ret(nnK);
        for (int i = 0; i < m_Doc.size(); ++i)
        {
            NN nn;
            nn.label = m_Doc[i].label;
            nn.similar = m_Sim->sim(doc, m_Doc[i]);
            ret.Push(nn);
        }
        return ret;
    }

    TestNN Test(string _testFile)
    {
        ifstream fin(_testFile.c_str());
        string tmp;
        TestNN ret;
        while (getline(fin, tmp))
        {
            Document doc;
            doc.parse(tmp);
            NN_Queue tmpq = Test(doc);
            ret.push_back(tmpq);
        }
        fin.close();
        return ret;
    }

};

#endif
