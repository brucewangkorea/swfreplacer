/* 
 * File:   TagInstance.h
 * Author: brucewang
 *
 * Created on October 27, 2009, 6:31 PM
 */

#ifndef _TAGINSTANCE_H
#define	_TAGINSTANCE_H

#include "SWFReader.h"
#include "SWFWriter.h"

class TagInstance {
    byte* binbuff;
    
protected:
    ulong length;

public:
    TagInstance();
    TagInstance(const TagInstance& orig);
    virtual ~TagInstance();

    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID(){ return 0; }
    virtual void SetCharacterID(UInt16 IdNew){  }
    virtual void IncreaseCharacterID(UInt16 nAmount) {
        SetCharacterID( GetCharacterID()+nAmount );
    }

private:

};

#endif	/* _TAGINSTANCE_H */

