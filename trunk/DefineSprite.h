/* 
 * File:   Sprite.h
 * Author: brucewang
 *
 * Created on October 29, 2009, 5:41 PM
 */

#ifndef _SPRITE_H
#define	_SPRITE_H

#include "types.h"
#include "TagInstance.h"
#include "Tag.h"

#include <list>
using namespace std;

typedef struct{
    UInt16 SpriteID;
    UInt16 FrameCount;
    list<Tag> TagList;
}stSprite;


typedef struct{
    byte FillStyleType;
    byte Color[3]; //RGB
}stFILLSTYLE;

class DefineSprite : public TagInstance  {
public:
    DefineSprite();
    DefineSprite(const DefineSprite& orig);
    virtual ~DefineSprite();

    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID();
    virtual void SetCharacterID(UInt16 IdNew);
    virtual void IncreaseCharacterID(UInt16 nAmount);
    
    //void ReplaceSprite(list<Tag>* pMainTagList, byte* p, int len);
    void ReplaceSprite(list<Tag>* pMainTagList, const char* szFile);
    
private:
    stSprite mSprite;
    
};

#endif	/* _SPRITE_H */

