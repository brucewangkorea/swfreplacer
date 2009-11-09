/* 
 * File:   EditText.cpp
 * Author: brucewang
 * 
 * Created on October 27, 2009, 4:07 PM
 */

#include "DefineEditText.h"

void DefineEditText::_init(){
    mDefineTextTag.CharacterID = 0;
    length = 0;
}

DefineEditText::DefineEditText() {
    _init();
}

DefineEditText::DefineEditText(const DefineEditText& orig) {
    _init();
}

DefineEditText::~DefineEditText() {
}

ulong DefineEditText::GetLength(void){
    if(mDefineTextTag.CharacterID == 0){
        //printf("EditText Instance length = 0 (Not initialized)\n");
        return this->length;
    }


    int iTotSize=0;
    int iOptionFlagSize = 0;

    iTotSize+= 2; // mDefineTextTag.CharacterID
    iTotSize+= mDefineTextTag.Bounds.GetSize();
    iTotSize+= 2; //mDefineTextTag.Flags

    //--
    if ( (mDefineTextTag.Flags & f_edit_has_font) != 0x0000){
        iOptionFlagSize += 2;//mDefineTextTag.FontID
        iOptionFlagSize += 2;//mDefineTextTag.FontHeight
    }
    if ( (mDefineTextTag.Flags & f_edit_has_color) != 0x0000){
        iOptionFlagSize += 4;//mDefineTextTag.TextColor
    }
    if ( (mDefineTextTag.Flags & f_edit_has_max_length) != 0x0000){
        iOptionFlagSize += 2;//mDefineTextTag.MaxLength
    }
    if ( (mDefineTextTag.Flags & f_edit_has_layout) != 0x0000){
        //mDefineTextTag.Align
        //mDefineTextTag.LeftMargin
        //mDefineTextTag.RightMargin
        //mDefineTextTag.Indent
        //mDefineTextTag.Leading
        iOptionFlagSize += 9;
    }
    iTotSize+=iOptionFlagSize;
    iTotSize += strlen(mDefineTextTag.VariableName) + 1;
    if( (mDefineTextTag.Flags & f_edit_has_text) != 0x0000){
        iTotSize += strlen(mDefineTextTag.InitialText) + 1;
    }
    //printf("EditText Instance length = %d\n", iTotSize);
    return iTotSize;
}


void DefineEditText::SetLength(ulong len){
    this->length = len;
}


UInt16 DefineEditText::GetCharacterID(){
    return this->mDefineTextTag.CharacterID;
}
void DefineEditText::SetCharacterID(UInt16 IdNew){
    this->mDefineTextTag.CharacterID = IdNew;
}


void DefineEditText::SetInitText(const char* sz){
    mDefineTextTag.Flags |= f_edit_has_text;
    strncpy( mDefineTextTag.InitialText, sz, sizeof(mDefineTextTag.InitialText) );
}

const char* DefineEditText::GetVariableName(){
    return mDefineTextTag.VariableName;
}


void DefineEditText::ReadData(SWFReader *swf, int length){
    mDefineTextTag.CharacterID = swf->ReadUI16();
    mDefineTextTag.Bounds.Read(swf);

    mDefineTextTag.Flags = swf->ReadUI16();
    if ( (mDefineTextTag.Flags & f_edit_has_font) !=  0x0000 )
    {
        mDefineTextTag.FontID = swf->ReadUI16();
        mDefineTextTag.FontHeight = swf->ReadUI16();
    }
    /*
    if ( mDefineTextTag.Flags.HasFontClass ) {
        strncpy( mDefineTextTag.FontClass, swf->ReadSTRING(), 256);
        printf("  FontClass : [%s]\n", mDefineTextTag.FontClass);
    }*/
    if ( (mDefineTextTag.Flags & f_edit_has_color) != 0x0000)
    {
        mDefineTextTag.TextColor = swf->ReadUI32();
    }
    if ( (mDefineTextTag.Flags & f_edit_has_max_length) != 0x0000)
    {
        mDefineTextTag.MaxLength = swf->ReadUI16();
    }
    if ( (mDefineTextTag.Flags & f_edit_has_layout) != 0x0000)
    {
        mDefineTextTag.Align = swf->ReadByte();
        mDefineTextTag.LeftMargin = swf->ReadUI16();
        mDefineTextTag.RightMargin = swf->ReadUI16();
        mDefineTextTag.Indent = swf->ReadSI16();
        mDefineTextTag.Leading = swf->ReadSI16();
    }

    strncpy( mDefineTextTag.VariableName, swf->ReadSTRING(), 256);
    printf("  VariableName : [%s]\n", mDefineTextTag.VariableName);


    if ( (mDefineTextTag.Flags & f_edit_has_text) != 0x0000)
    {
        strncpy( mDefineTextTag.InitialText, swf->ReadSTRING(), 256);
        printf("  InitialText : [%s]\n", mDefineTextTag.InitialText);
    }
}




void DefineEditText::WriteData(SWFWriter *swf, int length){
    swf->WriteUI16(mDefineTextTag.CharacterID );
    mDefineTextTag.Bounds.Write(swf);

    swf->WriteUI16( mDefineTextTag.Flags );

    //--
    if ( (mDefineTextTag.Flags & f_edit_has_font) != 0x0000)
    {
        swf->WriteUI16(mDefineTextTag.FontID);
        swf->WriteUI16(mDefineTextTag.FontHeight);
    }
    /*
    if ( mDefineTextTag.Flags & ..fontclass.. != 0x0000)
    {
        swf->WriteSTRING(mDefineTextTag.FontClass);
        printf("  FontClass : [%s]\n", mDefineTextTag.FontClass);
    }*/
    if ( (mDefineTextTag.Flags & f_edit_has_color) != 0x0000)
    {
        swf->WriteUI32(mDefineTextTag.TextColor);
    }
    if ( (mDefineTextTag.Flags & f_edit_has_max_length) != 0x0000)
    {
        swf->WriteUI16(mDefineTextTag.MaxLength);
    }
    if ( (mDefineTextTag.Flags & f_edit_has_layout) != 0x0000)
    {
        swf->WriteByte(mDefineTextTag.Align);
        swf->WriteUI16(mDefineTextTag.LeftMargin);
        swf->WriteUI16(mDefineTextTag.RightMargin);
        swf->WriteSI16(mDefineTextTag.Indent);
        swf->WriteSI16(mDefineTextTag.Leading);
    }

    swf->WriteSTRING(mDefineTextTag.VariableName);
    printf("  VariableName : [%s]\n", mDefineTextTag.VariableName);

    if( (mDefineTextTag.Flags & f_edit_has_text) != 0x0000)
    {
        swf->WriteSTRING(mDefineTextTag.InitialText);
        printf("  InitialText : [%s]\n", mDefineTextTag.InitialText);
    }
}
