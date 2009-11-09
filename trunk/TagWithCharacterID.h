/* 
 * File:   TagWithCharacterID.h
 * Author: brucewang
 *
 * Created on 2009/10/31, 9:07
 */

#ifndef _TAGWITHCHARACTERID_H
#define	_TAGWITHCHARACTERID_H

#include "types.h"
#include "TagInstance.h"

class TagWithCharacterID : public TagInstance {
    UInt16 mCharacterID;
    byte *mDataBuffer;
public:
    TagWithCharacterID();
    virtual ~TagWithCharacterID();

    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID();
    virtual void SetCharacterID(UInt16 IdNew);
private:

};

#endif	/* _TAGWITHCHARACTERID_H */

