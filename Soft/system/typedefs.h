/*********************************************************************
 * FileName:        typedefs.h
 ********************************************************************/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef unsigned char   byte;           // 8-bit
typedef unsigned int    word;           // 16-bit

typedef union _BITS
{
    byte bits;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
} BITS;

typedef union _WORD
{
    word words;
    struct
    {
        byte byte0;
        byte byte1;
    };
    struct
    {
        byte v[2];
    };
} WORD;

#endif //TYPEDEFS_H
