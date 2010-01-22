#ifndef _SERIALIZABLE_H
#define _SERIALIZABLE_H

/**
* –Ú¡–ªØ
*
*/

class Serializable
{
public:

    Serializable(void)
    {
    }

public:

    ~Serializable(void)
    {
    }

public:

    virtual void Serialize(unsigned char* _out, int& length) = 0;

    virtual void Deserialize(unsigned char* _in, int& length) = 0;

    virtual int Length() = 0;

};

#endif
