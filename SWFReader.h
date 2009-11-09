/* 
 * File:   SWFReader.h
 * Author: brucewang
 *
 * Created on October 24, 2009, 8:44 PM
 */

#ifndef _SWFREADER_H
#define	_SWFREADER_H

#include "types.h"

///-----------------------------------------------------------
///
///
///-----------------------------------------------------------

class SWFReader {
private:

    byte bitPosition; // Current bit position within byte (only used for reading bit fields)
    byte currentByte; // Value of the current byte (only used for reading bit fields)
    long lDataOffset;
    int iVariables;

public:
    UInt32 *bitValues; // For pre-computed bit values
    Single *powers; // For pre-computed fixed-point powers
    long lFileSizePos;
    byte *SwfData;

    int getNumberOfVariables();

    stSwfVariable VariableList[125];

    void AddVariableInfo(
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
            char *szInitialValue);

    /*
     * 2007-08-07 brucewang
     */
    long GetPosition();
	int GetBitPosition(){ return bitPosition; }
	const byte * GetBuffer(){ return SwfData; }

    ~SWFReader();

    SWFReader(byte *buffer);

    byte ReadByte();

    bool ReadBit();

    // Read an unsigned 8-bit integer

    byte ReadUI8();

    // You need to 'delete' the returned memory chunk
    // after use.
    //
    // Read an array of unsigned 8-bit integers

    byte* ReadUI8(int n);

    // Read a signed byte

    sbyte ReadSI8();

    // Read an unsigned 16-bit integer

    UInt16 ReadUI16();

    // Read a signed 16-bit integer

    Int16 ReadSI16();

    // Read an unsigned 32-bit integer

    UInt32 ReadUI32();

    // Read a signed 32-bit integer

    Int32 ReadSI32();

    // Read a 32-bit 16.16 fixed-point number

    Single ReadFIXED();

    // Read a szstring
    // TODO: Is StringBuilder worth it for these small strings?

    szstring ReadSTRING();


    // Read an unsigned bit value

    UInt32 ReadUB(int nBits);

    // Read a signed bit value

    Int32 ReadSB(int nBits);

    // Read a signed fixed-point bit value
    // TODO: Math.Pow probably isn't the fastest method of accomplishing this
    //Single ReadFB(int nBits);
};

#endif	/* _SWFREADER_H */

