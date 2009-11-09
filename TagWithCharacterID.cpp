/* 
 * File:   TagWithCharacterID.cpp
 * Author: brucewang
 * 
 * Created on 2009/10/31, 9:07
 */

#include "TagWithCharacterID.h"
#include <stdlib.h>
#include "stdio.h"

TagWithCharacterID::TagWithCharacterID() {
    mCharacterID = 0;
    mDataBuffer = 0;
}

TagWithCharacterID::~TagWithCharacterID() {
    if(mDataBuffer) free(mDataBuffer);
}

void TagWithCharacterID::ReadData(SWFReader *swf, int length) {
    mCharacterID = swf->ReadUI16();
    printf("  Use Character ID=%d\n", mCharacterID);
    if(mDataBuffer) free(mDataBuffer);
    mDataBuffer = (byte*)malloc(length-2);
    for( int i=0; i<length-2; i++ ){
        mDataBuffer[i] = swf->ReadByte();
    }
}

void TagWithCharacterID::WriteData(SWFWriter *swf, int length) {
    swf->WriteUI16(mCharacterID);
    printf("  Use Character ID=%d\n", mCharacterID);
    for( int i=0; i<length-2; i++ ){
        swf->WriteByte(mDataBuffer[i]);
    }
}

ulong TagWithCharacterID::GetLength(void) {
    return this->length;
}

void TagWithCharacterID::SetLength(ulong len) {
    this->length = len;
}

UInt16 TagWithCharacterID::GetCharacterID() {
    return mCharacterID;
}

void TagWithCharacterID::SetCharacterID(UInt16 IdNew) {
    mCharacterID = IdNew;
}
