#pragma once
#include "Similar.h"
#include <cmath>

class SimilarCos :
    public Similar
{
public:

    SimilarCos(void)
    {
    }
public:

    ~SimilarCos(void)
    {
    }

    double sim(Vector& _v1, Vector& _v2)
    {
        //不排序，默认下标有序
        int i1 = 0, i2 = 0;
        double setpi = 0;
        double count1 = 0, count2 = 0;
        for (;i1 < _v1.size() || i2 < _v2.size();)
        {
            if (i1 < _v1.size() && i2 < _v2.size())
            {
                if (_v1[i1].dim == _v2[i2].dim)
                {
                    count1 += sqr(_v1[i1].value);
                    count2 += sqr(_v2[i2].value);
                    setpi += _v1[i1++].value * _v2[i2++].value;
                } else if (_v1[i1].dim < _v2[i2].dim)
                {
                    count1 += sqr(_v1[i1++].value);
                }  else 
                {
                    count2 += sqr(_v2[i2++].value);
                }
            } else if (i1 < _v1.size())
            {
                count1 += sqr(_v1[i1++].value);
            } else 
            {
                count2 += sqr(_v2[i2++].value);
            }
        }
        return setpi / (sqrt(count1) * sqrt(count2));
    };

    double sim(Document& _d1, Document& _d2)
    {
        return sim(_d1.vec, _d2.vec);
    };
};
