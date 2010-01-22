#ifndef _SIMILIAR_H
#define _SIMILIAR_H

#include "DataStruct.h"

/**  相似度函数，默认为欧几里德距离
*
*/
class Similar
{
public:

    Similar(void)
    {
    }

public:

    ~Similar(void)
    {
    }

public:
    template<typename T>
    T sqr(T _v){
        return _v * _v;
    }

    virtual double sim(Vector& _v1, Vector& _v2)
    {
        //不排序，默认下标有序
        int i1 = 0, i2 = 0;
        double ret = 0;
        for (;i1 < _v1.size() || i2 < _v2.size();)
        {
            if (i1 < _v1.size() && i2 < _v2.size())
            {
                if (_v1[i1].dim == _v2[i2].dim)
                {
                    ret += sqr(_v1[i1++].value - _v2[i2++].value);
                } else if (_v1[i1].dim < _v2[i2].dim)
                {
                    ret += sqr(_v1[i1++].value);
                }  else 
                {
                    ret += sqr(_v2[i2++].value);
                }
            } else if (i1 < _v1.size())
            {
                ret += sqr(_v1[i1++].value);
            } else 
            {
                ret += sqr(_v2[i2++].value);
            }
        }
        return -ret;
    };

    virtual double sim(Document& _d1, Document& _d2)
    {
        return sim(_d1.vec, _d2.vec);
    };

};

#endif
