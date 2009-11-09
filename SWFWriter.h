/* 
 * File:   SWFWriter.h
 * Author: brucewang
 *
 * Created on October 27, 2009, 1:37 AM
 */

#ifndef _SWFWRITER_H
#define	_SWFWRITER_H

#include "types.h"


class SWFWriter {
	#define SWFWRITER_BUFSIZE 1048576 // 1mb
    byte* mBuffer; 

	int mBytePos;
    int mBitPos;
    bool pushByte(byte b);
    bool pushBit(bool b);
public:
    SWFWriter();
    virtual ~SWFWriter();

    int GetPosition(){ return mBytePos; }
	int GetBitPosition(){ return mBitPos; }
	const byte * GetBuffer(){ return mBuffer; }

    void WriteByte(byte data);
    void WriteUI8(byte data);
    void WriteUI8(byte* data, int n);
    void WriteSI8(sbyte data);
    void WriteUI16(UInt16 data);
    void WriteSI16(Int16 data);
    void WriteUI32(UInt32 data);
    void WriteSI32(UInt32 data);
    void WriteSTRING(const szstring data);
    void WriteUB(UInt32 data, int nBits);
    void WriteSB(Int32 data, int nBits);

    bool SaveFile(char* file);
private:

};

#endif	/* _SWFWRITER_H */

