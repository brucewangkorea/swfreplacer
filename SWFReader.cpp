/* 
 * File:   SWFReader.cpp
 * Author: brucewang
 * 
 * Created on October 24, 2009, 8:44 PM
 */

#include "SWFReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int SWFReader::getNumberOfVariables() {
    return this->iVariables;
}


void SWFReader::AddVariableInfo(
        ulong lPosition,
        ulong length,
        int iOriginalOffset,
        ushort usEditId,
        int iRectInfoLength,
        byte *bRectData,
        int iOptionFlagSize,
        byte *bOptionFlagData,
        ushort usFlag,
        char *szVariableName,
        char *szInitialValue) {
    VariableList[iVariables].lPosition = lPosition;

    VariableList[iVariables].lOriginalLength = VariableList[iVariables].lNewLength = length;
    VariableList[iVariables].iOriginalOffset = iOriginalOffset;
    VariableList[iVariables].usEditId = usEditId;
    VariableList[iVariables].usFlag = usFlag;

    VariableList[iVariables].iRectInfoLength = iRectInfoLength;
    memcpy(VariableList[iVariables].bRectData, bRectData, iRectInfoLength);

    VariableList[iVariables].iOptionFlagSize = iOptionFlagSize;
    memcpy(VariableList[iVariables].bOptionFlagData, bOptionFlagData, iOptionFlagSize);

    strcpy(VariableList[iVariables].szVariableName, szVariableName);
    strcpy(VariableList[iVariables].szInitialValue, szInitialValue);

    iVariables++;
}

////////////////////////////////////////////////////////////////////////////////
// READ
////////////////////////////////////////////////////////////////////////////////

/*
 * 2007-08-07 brucewang
 */
long SWFReader::GetPosition() {
    return lDataOffset;
}

SWFReader::~SWFReader() {
    if (bitValues) delete bitValues;
    if (powers) delete powers;
}

SWFReader::SWFReader(byte *buffer) {
    SwfData = buffer;
    bitPosition = 0; // Current bit position within byte (only used for reading bit fields)
    currentByte = 0; // Value of the current byte (only used for reading bit fields)
    lDataOffset = 0L;
    lFileSizePos = 4;
    iVariables = 0;

    // Setup bit values for later lookup
    bitValues = new UInt32[32]; /// deletion is done at the destructor
    for (byte power = 0; power < 32; power++) {
        bitValues[power] = (UInt32) (1 << power);
    }

    // Setup power values for later lookup
    powers = new Single[32]; /// deletion is done at the destructor
    for (byte power = 0; power < 32; power++) {
        powers[power] = (Single) pow(2.0f, (power - 16)*1.0f);
    }
}


byte SWFReader::ReadByte() {
    byte byteRead = 0;

    // !!
    // !!
    memcpy(&byteRead, this->SwfData + lDataOffset, 1);
    lDataOffset += 1;

    this->bitPosition = 8; // So that ReadBit() knows that we've "used" this byte already

    return (byte) byteRead;
}




bool SWFReader::ReadBit() {
    bool result;

    // Do we need another byte?
    if (this->bitPosition > 7) {
        this->currentByte = ReadByte();
        this->bitPosition = 0; // Reset, since we haven't "used" this byte yet
    }

    // Read the current bit
    result = ((this->currentByte & bitValues[(7 - bitPosition)]) != 0);

    // Move to the next bit
    this->bitPosition++;

    return result;
}




// Read an unsigned 8-bit integer

byte SWFReader::ReadUI8() {
    return ReadByte();
}




// You need to 'delete' the returned memory chunk
// after use.
//
// Read an array of unsigned 8-bit integers

byte* SWFReader::ReadUI8(int n) {
    byte* result = new byte[n];

    for (int index = 0; index < n; index++) {
        result[index] = ReadUI8();
    }

    return result;
}


// Read a signed byte

sbyte SWFReader::ReadSI8() {
    return (sbyte) ReadByte();
}


// Read an unsigned 16-bit integer

UInt16 SWFReader::ReadUI16() {
    UInt16 result = 0;

    result |= (UInt16) ReadByte();
    result |= (UInt16) (ReadByte() << 8);

    return result;
}


// Read a signed 16-bit integer

Int16 SWFReader::ReadSI16() {
    return (Int16) ReadUI16();
}


// Read an unsigned 32-bit integer

UInt32 SWFReader::ReadUI32() {
    UInt32 result = 0;

    result |= (UInt32) ReadByte();
    result |= (UInt32) (ReadByte() << 8);
    result |= (UInt32) (ReadByte() << 16);
    result |= (UInt32) (ReadByte() << 24);

    return result;
}


// Read a signed 32-bit integer

Int32 SWFReader::ReadSI32() {
    return (Int32) ReadUI32();
}




// Read a szstring
// TODO: Is StringBuilder worth it for these small strings?

szstring SWFReader::ReadSTRING() {
    static char result[4096] = {0,};
    memset(result,0,4096);
    int index = 0;
    byte byteTemp = 0;

    // Grab characters until we hit 0x00
    do {
        byteTemp = ReadByte();
        result[index] = (char) byteTemp;
        index++;
    } while (byteTemp != 0x00);
    
    return result;
}


// Read an unsigned bit value

UInt32 SWFReader::ReadUB(int nBits) {
    UInt32 result = 0;

    // Is there anything to read?
    if (nBits > 0) {
        // Calculate value
        for (int index = nBits - 1; index > -1; index--) {
            if (ReadBit()) {
                result |= bitValues[index];
            }
        }
    }

    return result;
}

// Read a signed bit value

Int32 SWFReader::ReadSB(int nBits) {
    Int32 result = 0;

    // Is there anything to read?
    if (nBits > 0) {
        // Is this a negative number (MSB will be set)?
        if (ReadBit()) {
            result -= (Int32) bitValues[nBits - 1];
        }

        // Calculate rest of value
        for (int index = nBits - 2; index > -1; index--) {
            if (ReadBit()) {
                result |= (Int32) bitValues[index];
            }
        }
    }

    return result;
}



