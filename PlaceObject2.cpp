/* 
 * File:   PlaceObjec2.cpp
 * Author: brucewang
 * 
 * Created on 2009/10/31, 20:24
 */

#include "PlaceObject2.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

PlaceObject2::PlaceObject2() {
    memset( &mData, 0, sizeof(mData) );
}

PlaceObject2::~PlaceObject2() {
    if( mData.pDataChunk ) free( mData.pDataChunk );
}

void PlaceObject2::ReadData(SWFReader *swf, int length) {
    int iLen2Read = length;
    mData.Flags = swf->ReadByte();
    mData.Depth = swf->ReadUI16();
    iLen2Read -= 3;
    
    if( 0 != (mData.Flags & f_PlaceFlagHasCharacter) ){
        mData.CharacterId = swf->ReadUI16();
        printf("  Uses Character, ID = %d\n", mData.CharacterId );
        iLen2Read -= 2;
    }

    if( mData.pDataChunk ) free( mData.pDataChunk );
    mData.pDataChunk = (byte*)malloc(iLen2Read);
    for( int i=0; i<iLen2Read; i++ ){
        mData.pDataChunk[i] = swf->ReadByte();
    }
}

void PlaceObject2::WriteData(SWFWriter *swf, int length) {
    int iLen2Write = length;
    swf->WriteByte( mData.Flags );
    swf->WriteUI16( mData.Depth );
    iLen2Write -= 3;
    
    if( 0 != (mData.Flags & f_PlaceFlagHasCharacter) ){
        swf->WriteUI16(mData.CharacterId);
        printf("  Uses Character, ID = %d\n", mData.CharacterId );
        iLen2Write -= 2;
    }

    for( int i=0; i<iLen2Write; i++ ){
        swf->WriteByte( mData.pDataChunk[i] );
    }
}

ulong PlaceObject2::GetLength(void) {
    return this->length;
}

void PlaceObject2::SetLength(ulong len) {
    this->length = len;
}

UInt16 PlaceObject2::GetCharacterID() {
    if( 0 != (mData.Flags & f_PlaceFlagHasCharacter) ){
        return mData.CharacterId;
    }
    return 0;
}

void PlaceObject2::SetCharacterID(UInt16 IdNew) {
    if( 0 != (mData.Flags & f_PlaceFlagHasCharacter) ){
        mData.CharacterId = IdNew;
    }
}

void PlaceObject2::IncreaseCharacterID(UInt16 nAmount){
    if( 0 != (mData.Flags & f_PlaceFlagHasCharacter) ){
        mData.CharacterId += nAmount;
    }
}