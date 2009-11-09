/* 
 * File:   SWFWriter.cpp
 * Author: brucewang
 * 
 * Created on October 27, 2009, 1:37 AM
 */

#include "SWFWriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



SWFWriter::SWFWriter() {
	mBuffer = (byte*)malloc(SWFWRITER_BUFSIZE);
    memset( mBuffer, 0, SWFWRITER_BUFSIZE );
    mBytePos = mBitPos = 0;
}

SWFWriter::~SWFWriter() {
	free(mBuffer);
}

#if 0
bool SWFWriter::pushByte(byte b){
    if(mBitPos>0) mBytePos++;
    
    mBuffer[mBytePos] = b;
    mBytePos++;
    mBitPos=0;

    return true;
}

bool SWFWriter::pushBit(bool b){
    if(mBitPos>7){
        mBytePos++;
        mBitPos=0;
    }

    if(b){
        byte currentb = mBuffer[mBytePos];
        byte bnew = (byte)( 1 << (7-mBitPos) );
        mBuffer[mBytePos] = currentb | bnew;
    }
    mBitPos++;

    return true;
}
#else
bool SWFWriter::pushByte(byte b){
    if(mBitPos>0) mBytePos++;
    
    mBuffer[mBytePos] = b;
    mBytePos++;
    mBitPos=0;

    return true;
}

bool SWFWriter::pushBit(bool b){
    if(b){
        byte currentb = mBuffer[mBytePos];
        byte bnew = (byte)( 1 << (7-mBitPos) );
        mBuffer[mBytePos] = currentb | bnew;
    }
    mBitPos++;
	if(mBitPos>7){
        mBytePos++;
        mBitPos=0;
    }

    return true;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// WRITE
////////////////////////////////////////////////////////////////////////////////

void SWFWriter::WriteByte(byte data){
    pushByte(data);
}

void SWFWriter::WriteUI8(byte data){
    WriteByte(data);
}

void SWFWriter::WriteUI8(byte* data, int n){
    for( int i=0; i<n; i++ ){
        WriteByte(data[i]);
    }
}

void SWFWriter::WriteSI8(sbyte data){
    return WriteByte(data);
}

void SWFWriter::WriteUI16(UInt16 data){
    byte b;

#if 0 //for BigEndian CPUs
    b = (byte)(data>>8); WriteByte(b);
    b = (byte)(data & 0x00ff); WriteByte(b);
#else
    b = (byte)(data & 0x00ff); WriteByte(b);
    b = (byte)(data>>8); WriteByte(b);
#endif
}

void SWFWriter::WriteSI16(Int16 data){
    WriteUI16(data);
}

void SWFWriter::WriteUI32(UInt32 data){
    byte b;

#if 0 //for BigEndian CPUs
    b = (byte)(data>>24); WriteByte(b);
    b = (byte)(data>>16); WriteByte(b);
    b = (byte)(data>>8); WriteByte(b);
    b = (byte)(data & 0x000000ff); WriteByte(b);
#else
    b = (byte)(data & 0x000000ff); WriteByte(b);
    b = (byte)(data>>8); WriteByte(b);
    b = (byte)(data>>16); WriteByte(b);
    b = (byte)(data>>24); WriteByte(b);
#endif
}

void SWFWriter::WriteSI32(UInt32 data){
    return WriteUI32(data);
}

void SWFWriter::WriteSTRING(const szstring data){
    int iwritten=0;
    while( data[iwritten]!= 0 ){
        WriteByte( data[iwritten] );
        iwritten++;
    }
    // Terminating NULL
    WriteByte(0);
}

void SWFWriter::WriteUB(UInt32 data, int nBits){
    bool b;
    for(int i=nBits-1; i>-1; i--){
        b = 0x00000001 & (data>>i);
        this->pushBit(b);
    }
}

void SWFWriter::WriteSB(Int32 data, int nBits) {
    if( data<0 )
        data = (1<<nBits)+data;
    return WriteUB(data, nBits);
}

bool SWFWriter::SaveFile(char* file){
    bool bReturn=false;
    FILE* fp = fopen(file, "wb");
    if(fp){
        fwrite(mBuffer, 1, mBytePos, fp);
        fclose(fp);
        bReturn = true;
    }

    return bReturn;
}
