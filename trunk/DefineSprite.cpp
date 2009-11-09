/* 
 * File:   Sprite.cpp
 * Author: brucewang
 * 
 * Created on October 29, 2009, 5:41 PM
 */

#include "DefineSprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "SWFFile.h"

DefineSprite::DefineSprite() {
    mSprite.SpriteID = 0;
}

DefineSprite::DefineSprite(const DefineSprite& orig) {
    mSprite.SpriteID = 0;
}

DefineSprite::~DefineSprite() {
    mSprite.TagList.clear();
}

void DefineSprite::ReadData(SWFReader *swf, int length){
    mSprite.SpriteID = swf->ReadUI16();
    printf("  SpriteID = %d\n", mSprite.SpriteID );
    mSprite.FrameCount = swf->ReadUI16();
    printf("  FrameCount = %d\n", mSprite.FrameCount );

    printf("  ----------------------------------------\n");
    mSprite.TagList.clear();
    Tag *p;

    do {
        printf("  ");
        p = new Tag(swf);
        mSprite.TagList.push_back(*p);
    } while (p->ID() != 0);
    list<Tag>::iterator i;
    int iContentsLength = 0;
    for (i = mSprite.TagList.begin(); i != mSprite.TagList.end(); ++i) {
        iContentsLength += (*i).GetLength();
    }
    printf("  # Content length in this Sprite = %d\n", iContentsLength);
    printf("  ----------------------------------------\n");

}

void DefineSprite::WriteData(SWFWriter *swf, int length){
    swf->WriteUI16( mSprite.SpriteID );
    printf("  SpriteID = %d\n", mSprite.SpriteID );
    swf->WriteUI16( mSprite.FrameCount );
    printf("  FrameCount = %d\n", mSprite.FrameCount );
    
    printf("  ----------------------------------------\n");
    list<Tag>::iterator i;
    int iContentsLength = 0;
    for (i = mSprite.TagList.begin(); i != mSprite.TagList.end(); ++i) {
        iContentsLength += (*i).GetLength();
        printf("  ");
        (*i).Write(swf);
    }
    printf("  # Content length in this Sprite = %d\n", iContentsLength);
    printf("  ----------------------------------------\n");
}


UInt16 DefineSprite::GetCharacterID(){ 
    return mSprite.SpriteID;
}
void DefineSprite::SetCharacterID(UInt16 IdNew){
    mSprite.SpriteID = IdNew;
}
void DefineSprite::IncreaseCharacterID(UInt16 nAmount) {
    SetCharacterID( GetCharacterID()+nAmount );
    
    printf("Changing all internal references to characters.\n");
    list<Tag>::iterator i;
    for (i = mSprite.TagList.begin(); i != mSprite.TagList.end(); ++i) {
        (*i).GetTagContent()->IncreaseCharacterID(nAmount);
        printf("  %s : [%d] -> [%d]\n",
                (*i).GetType((*i).ID()),
                (*i).GetTagContent()->GetCharacterID(),
                (*i).GetTagContent()->GetCharacterID() + nAmount);
    }
}




ulong DefineSprite::GetLength(void){
    if( 0==mSprite.SpriteID ){
        return this->length;
    }

    int iContentsLength = 4; // SpriteID + FrameCount

    list<Tag>::iterator i;
    for (i = mSprite.TagList.begin(); i != mSprite.TagList.end(); ++i) {
        iContentsLength += (*i).GetLength();
    }
    return iContentsLength;
}

void DefineSprite::SetLength(ulong len){
    this->length = len;
}




void DefineSprite::ReplaceSprite(list<Tag>* pMainTagList, const char* szFile){

    

#if 1
    
	Tag* _TagList_Definitions[256];
	Tag* _TagList_4Sprite[256];
    Header _Header;
	int iDefCount = 0;
	int iActionScount = 0;

    // 1. Read in new Tag list from file
    FILE* fp = fopen(szFile, "rb");
    if( NULL==fp ){
        printf("Open file (%s)error\n", szFile);
        return;
    }

    unsigned char *Buf=0;
    fseek(fp, 0L, SEEK_END);
    long lfilesize = ftell(fp);

    Buf = (unsigned char *)malloc(lfilesize);
    if(Buf){
        fseek(fp, 0L, SEEK_SET);
        fread(Buf, 1, lfilesize, fp);

        //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		// 1) Read in the new Tag contents in the file
        SWFReader* swf = new SWFReader(Buf);
        _Header.Read(swf);
        Tag* p;
        do {
            p = new Tag(swf);
            if( p->ID()>0 ){
				/*
				The following tags are valid within a DefineSprite tag:

				. ShowFrame(1) . StartSound(15)
				. PlaceObject(4) . FrameLabel(43)
				. PlaceObject2(26) . SoundStreamHead(18)
				. RemoveObject(5) . SoundStreamHead2(45)
				. RemoveObject2(28) . SoundStreamBlock(19)
				. All Actions (see Actions)(12, 59) . End(0)
				*/
				if( p->ID()==1 || p->ID()==15 || p->ID()==4 || p->ID()==43 || 
					p->ID()==26 || p->ID()==18 || p->ID()==5 || p->ID()==45 || 
					p->ID()==28 || p->ID()==19 || p->ID()==12 || p->ID()==59 )
				{
					_TagList_4Sprite[iActionScount++] = p;
				}
				else{
					// 9 : SetBackgroundColor
					if( p->ID()!=9 ){
						_TagList_Definitions[iDefCount++] = p;
					}
				}
            }
			else{
				_TagList_4Sprite[iActionScount++] = p;
			}
        } while (p->ID() != 0);
        //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        
        free(Buf);
    }
    fclose(fp);


	//-----------------------------------------------------
	// 2) Increase the Character IDs in the original list.
	list<Tag>::iterator i;
    for (i = pMainTagList->begin(); i != pMainTagList->end(); ++i) {
        (*i).GetTagContent()->IncreaseCharacterID( iDefCount );
    }
	//-----------------------------------------------------

	
	//-----------------------------------------------------
	// 3) Inject the new Definition Tags into this original Tag list
	for( int j=iDefCount-1; j>=0; j-- ){
	//list<Tag>::iterator j;
    //for (j = _TagList_Definitions.end(); j != _TagList_Definitions.begin(); --j) {
	//	pMainTagList->push_front( *j );
		pMainTagList->insert( ++pMainTagList->begin(), *_TagList_Definitions[j] );
    }
	//pMainTagList->insert( pMainTagList->begin(), _TagList_Definitions.begin(), _TagList_Definitions.end());
	//-----------------------------------------------------

	//-----------------------------------------------------
	// 4) Inject the new Action and other Tags for this Sprite
	list<Tag>::iterator k;
	mSprite.FrameCount = _Header.FrameCount();
    mSprite.TagList.clear();
	int iContentsLength = 0;

	for( int k=iActionScount-1; k>=0; k-- ){
    //for (k = _TagList_4Sprite.begin(); k != _TagList_4Sprite.end(); ++k) {
		//mSprite.TagList.push_back( *k );
		//iContentsLength += (*k).GetLength();
		mSprite.TagList.push_front( *_TagList_4Sprite[k] );
		iContentsLength += _TagList_4Sprite[k]->GetLength();
    }
    printf("  # Content length in this Sprite = %d\n", iContentsLength);

#endif
}