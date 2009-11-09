/* 
 * File:   TagInstance.cpp
 * Author: brucewang
 * 
 * Created on October 27, 2009, 6:31 PM
 */

#include "TagInstance.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

TagInstance::TagInstance() {
    binbuff=0;
}

TagInstance::TagInstance(const TagInstance& orig) {
    binbuff=0;
}

TagInstance::~TagInstance() {
    if(binbuff) free(binbuff);
}

ulong TagInstance::GetLength(void){
    return this->length;
}
void TagInstance::SetLength(ulong len){
    this->length = len;
}

/*
 * Virtual function
 * */
void TagInstance::ReadData(SWFReader *swf, int length){
    binbuff = (byte*)malloc(length);
    // For now, just skip the remainder of the tag
    // ** This is normally where you'd process each tag in the file **
    for (ulong index = 0; index < length; index++) {
        binbuff[index] = swf->ReadByte();
    }
}

/*
 * Virtual function
 * */
void TagInstance::WriteData(SWFWriter *swf, int length){
    // For now, just skip the remainder of the tag
    // ** This is normally where you'd process each tag in the file **
    for (ulong index = 0; index < length; index++) {
        swf->WriteByte( binbuff[index] );
    }
}