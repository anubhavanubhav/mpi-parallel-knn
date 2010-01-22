#include "Similar.h"
#include "KNN.h"

#include "mpi.h"

#include <iostream>
#include <fstream>

using namespace std;

string trainData;
string testData;
string simName;
int k;
int myRank;

void LoadConfig()
{
    ifstream fin("parallel_knn.ini");
    getline(fin, trainData);
    getline(fin, testData);
    getline(fin, simName);
    fin >> k;
    fin.close();
}

void OutputKNN(KNN::TestNN& _knn, string _out)
{
    ofstream fout(_out.c_str());
    for (int i = 0; i < _knn.size(); ++i)
    {
        for (int j = 0; j < k; ++j)
        {
            fout << _knn[i][j].label << " " << _knn[i][j].similar << " ";
        }
        fout << endl;
    }
    fout.close();
}

int main(int argc, char ** argv)
{

    MPI::Init(argc, argv);

    LoadConfig();
    KNN knn(k, simName);

    myRank = MPI::COMM_WORLD.Get_rank();


    stringstream trainFile;
    trainFile << trainData << "_" << myRank;
    knn.LoadTrainData(trainFile.str().c_str());

    KNN::TestNN tnn = knn.Test(testData);

    if (myRank != 0)
    {
        //非0进程发送结果 
        int totalLen = 0;
        for (int i = 0; i < tnn.size(); ++i)
        {
            totalLen += tnn[i].Length();
        }
        unsigned char* buffer = new unsigned char[totalLen];
        int offset = 0;
        for (int i = 0; i < tnn.size(); ++i)
        {
            int vlen;
            tnn[i].Serialize((buffer + offset), vlen);
            offset += vlen;
        }
        cout << myRank << "  send len" << endl;
        MPI::COMM_WORLD.Ssend(&totalLen, 1, MPI::INT, 0, myRank * 10 + 0);
        cout << myRank << "  send buffer" << endl;
        MPI::COMM_WORLD.Ssend(buffer, totalLen, MPI::UNSIGNED_CHAR, 0, myRank * 10 + 1);
        cout << myRank << "  done" << endl;
        delete [] buffer;
    } else 
    {
        int totalProc = MPI::COMM_WORLD.Get_size();
        cout << myRank << " totalProc = " << totalProc << endl;
        for (int rnk = 1; rnk < totalProc; ++rnk)
        {
            int len;
            cout << myRank << " recv len form " << rnk << endl;
            MPI::COMM_WORLD.Recv(&len, 1, MPI::INT, rnk, rnk * 10 + 0);
            cout << myRank << " len[" << rnk << "] = " << len;
            cout << myRank << " recv buffer form " << rnk << endl;
            unsigned char* buffer = new unsigned char[len];
            MPI::COMM_WORLD.Recv(buffer, len, MPI::UNSIGNED_CHAR, rnk, rnk * 10 + 1);
            cout << myRank << " recv done " << rnk << endl;
            int offset = 0;
            for (int i = 0; i < tnn.size(); ++i)
            {
                KNN::NN_Queue nn(k);
                int vlen;
                nn.Deserialize((buffer + offset), vlen);
                tnn[i].merge(nn);
                offset += vlen;
            }
            delete[] buffer;
        }
        OutputKNN(tnn, simName + ".result");
    }


    MPI::Finalize();

    return 0;
}

