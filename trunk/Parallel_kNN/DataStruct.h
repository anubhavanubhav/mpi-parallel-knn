#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "Serializable.h"

using namespace std;

struct DimPair
{
    int dim; 
    double value;

    const bool operator<(const DimPair& _ot) const
    {
        return dim < _ot.dim;
    }

};

typedef std::vector<DimPair> Vector;

struct Document
{
//    double label;
    int label;
    Vector vec;

    void parse(string _str)
    {
        stringstream ss(_str);
        ss >> label;
        char cc;
        int d;
        double v;
        while (ss >> d >> cc >> v){
            DimPair dp;
            dp.dim = d;
            dp.value = v;
            vec.push_back(dp);
        }
        sort(vec.begin(), vec.end());
    }
};

struct NN : public Serializable
{
//    double label;
    int label;
    double similar;
    bool operator < (const NN& _n) const
    {
        return similar < _n.similar;
    }

    int Length()
    {
//        return sizeof(double) * 2;
        return sizeof(double) + sizeof(int);
    }

    void Serialize(unsigned char* _out, int& _len)
    {
        _len = Length();
//        memcpy(_out, &label, sizeof(double));
        memcpy(_out, &label, sizeof(int));
//        memcpy(_out + sizeof(double), &similar, sizeof(double));
        memcpy(_out + sizeof(int), &similar, sizeof(double));
    }

    void Deserialize(unsigned char* _in, int& _len)
    {
        _len = Length();
//        memcpy(&label, _in, sizeof(double));
        memcpy(&label, _in, sizeof(int));
//        memcpy(&similar, _in + sizeof(double), sizeof(double));
        memcpy(&similar, _in + sizeof(int), sizeof(double));
    }
};

template <typename _Type>
struct priorityQueue : public Serializable
{
    int size;
    int curSize;
    vector<_Type> que;

    priorityQueue(int _size)
    {
        size = _size;
        curSize = 0;
        que.resize(size);
    }

    void Push(_Type _val)
    {
        int i;
        for (i = 0; i < curSize; ++i)
        {
            if (que[i] < _val)
            {
                break;
            }
        }
        if (i >= size)
        {
            return;
        }
        for (int j = curSize; j >= i; --j)
        {
            if (j + 1 >= size)
            {
                continue;
            }
            que[j + 1] = que[j];
        }
        que[i] = _val;
        if (curSize < size)
        {
            curSize++;
        }
    }

    _Type& operator[](int _id)
    {
        return que[_id];
    }

    void merge(priorityQueue<_Type>& _o)
    {
        for (int i = 0; i < _o.size; ++i)
        {
            Push(_o[i]);
        }
    }

    int Length()
    {
        int ret = 0;
        ret += sizeof(int); //size
        ret += sizeof(int); //curSize
        for (int i = 0; i < curSize; ++i)
        {
            ret += ((NN)que[i]).Length();
        }
        return ret;
    }

    void Serialize(unsigned char* _out, int& length)
    {
        int offset = 0;
        memcpy(_out + offset, &size, sizeof(int));
        offset += sizeof(int);
        memcpy(_out + offset, &curSize, sizeof(int));
        offset += sizeof(int);
        for (int i = 0; i < curSize; ++i)
        {
            int vlen;
            ((NN)que[i]).Serialize(_out + offset, vlen);
            offset += vlen;
        }
        length = offset;
    }

    void Deserialize(unsigned char* _in, int& length)
    {
        int offset = 0;
        memcpy(&size, _in + offset, sizeof(int));
        offset += sizeof(int);
        memcpy(&curSize, _in + offset, sizeof(int));
        offset += sizeof(int);
        que.clear();
        for (int i = 0; i < curSize; ++i)
        {
            int vlen;
            NN tpy;
            tpy.Deserialize(_in + offset, vlen);
            offset += vlen;
            que.push_back(tpy);
        }
        length = offset;
    }
};

#endif

