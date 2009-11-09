/* 
 * File:   types.h
 * Author: brucewang
 *
 * Created on October 24, 2009, 8:49 PM
 */

#ifndef _TYPES_H
#define	_TYPES_H



typedef char * szstring;
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short uint;
typedef unsigned short ushort;
typedef short Int16;
typedef unsigned short UInt16;
typedef float Single;
typedef unsigned int UInt32;
typedef int Int32;
typedef unsigned long ulong;
//typedef float 

typedef struct tag_stSwfVariable {
    ulong lPosition;
    ulong lNewLength;
    ulong lOriginalLength;
    int iOriginalOffset;
    ushort usEditId;
    int iRectInfoLength;
    byte bRectData[32];
    ushort usFlag;
    int iOptionFlagSize;
    byte bOptionFlagData[20];
    char szVariableName[1024];
    char szInitialValue[1024];
} stSwfVariable;




#endif	/* _TYPES_H */

