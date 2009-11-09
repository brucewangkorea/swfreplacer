/* 
 * File:   JPEG2.h
 * Author: brucewang
 *
 * Created on October 27, 2009, 4:07 PM
 */

#ifndef _JPEG2_H
#define	_JPEG2_H


#include "types.h"
#include "Rect.h"
#include "TagInstance.h"

typedef struct {
    // CharacterID UI16 ID for this character.
    UInt16 CharacterID;

    // Compressed image data in
    // either JPEG, PNG, or GIF89a
    // format
    byte* pImageData;
} stDefineJPEG2;

class DefineJPEG2 : public TagInstance {
    int m_iImgLen;
    byte* mImgData;
    byte marker_start[2];
    byte marker_end[2];

    void _init();
public:
    DefineJPEG2();
    DefineJPEG2(const DefineJPEG2& orig);
    virtual ~DefineJPEG2();


    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID(){
        return mJPEG2Tag.CharacterID;
    }
    virtual void SetCharacterID(UInt16 IdNew){
        mJPEG2Tag.CharacterID = IdNew;
    }

    void ReplaceImg(const char* szFilePath2Replacewith);

private:
    stDefineJPEG2 mJPEG2Tag;
};

#endif	/* _JPEG2_H */

