#pragma once
#include "Similar.h"

class SimilarSet :
    public Similar
{
public:

    SimilarSet(void)
    {
    }
public:

    ~SimilarSet(void)
    {
    }

    double sim(Vector& _v1, Vector& _v2)
    {
        //不排序，默认下标有序
        int i1 = 0, i2 = 0;
        double count = 0;
        double count1 = 0, count2 = 0;
        for (;i1 < _v1.size() || i2 < _v2.size();)
        {
            if (i1 < _v1.size() && i2 < _v2.size())
            {
                if (_v1[i1].dim == _v2[i2].dim)
                {
                    count += 1;
                    ++i1;
                    ++i2;
                } else if (_v1[i1].dim < _v2[i2].dim)
                {
                    count1 += 1;
                    ++i1;
                }  else 
                {
                    count2 += 1;
                    ++i2;
                }
            } else if (i1 < _v1.size())
            {
                count1 += 1;
                ++i1;
            } else 
            {
                count2 += 1;
                ++i2;
            }
        }
        return count / (count1 + count2 + count);
    };

    double sim(Document& _d1, Document& _d2)
    {
        return sim(_d1.vec, _d2.vec);
    };
};
