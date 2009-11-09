/* 
 * File:   PlaceObjec2.h
 * Author: brucewang
 *
 * Created on 2009/10/31, 20:24
 */

#ifndef _PLACEOBJEC2_H
#define	_PLACEOBJEC2_H

#include "TagInstance.h"


//
// 2009-10-28 brucewang
// These bit flags are for Little Endian CPUs.
// Maybe you want to use BitFields, but BitFields are
// compiler dependant, and are not PACKED. So you can not
// directly save to and load from SWF file.
//
#define f_PlaceFlagHasClipActions ((byte)0x80)
#define f_PlaceFlagHasClipDepth ((byte)0x40)
#define f_PlaceFlagHasName ((byte)0x20)
#define f_PlaceFlagHasRatio ((byte)0x10)
#define f_PlaceFlagHasColorTransform ((byte)0x08)
#define f_PlaceFlagHasMatrix ((byte)0x04)
#define f_PlaceFlagHasCharacter ((byte)0x02)
#define f_PlaceFlagMove ((byte)0x01)


typedef struct{
    byte Flags;
    UInt16 Depth;
    // If f_PlaceFlagHasCharacter is true
    UInt16 CharacterId;

    // Continues some other properties...
    byte *pDataChunk;
}stPlaceObject2;



class PlaceObject2 : public TagInstance {
public:
    PlaceObject2();
    virtual ~PlaceObject2();

    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID();
    virtual void SetCharacterID(UInt16 IdNew);
    virtual void IncreaseCharacterID(UInt16 nAmount);
    
private:
    stPlaceObject2 mData;
};

#endif	/* _PLACEOBJEC2_H */

