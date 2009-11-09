/* 
 * File:   EditText.h
 * Author: brucewang
 *
 * Created on October 27, 2009, 4:07 PM
 */

#ifndef _EDITTEXT_H
#define	_EDITTEXT_H


#include "types.h"
#include "Rect.h"
#include "TagInstance.h"




//
// 2009-10-28 brucewang
// These bit flags are for Little Endian CPUs.
// Maybe you want to use BitFields, but BitFields are
// compiler dependant, and are not PACKED. So you can not
// directly save to and load from SWF file.
//
#define f_edit_has_text ((ushort)0x0080)
#define f_edit_word_wrap ((ushort)0x0040)
#define f_edit_multiline ((ushort)0x0020)
#define f_edit_password ((ushort)0x0010)
#define f_edit_readonly ((ushort)0x0008)
#define f_edit_has_color ((ushort)0x0004)
#define f_edit_has_max_length ((ushort)0x0002)
#define f_edit_has_font ((ushort)0x0001)
//if(version >= 6)
//{
#define f_edit_reserved1 ((ushort)0x8000)
#define f_edit_auto_size ((ushort)0x4000)
//}
//else {
//    ushort f_edit_reserved : 2;
//}
#define f_edit_has_layout ((ushort)0x2000)
#define f_edit_no_select ((ushort)0x1000)
#define f_edit_border ((ushort)0x0800)
#define f_edit_reserved ((ushort)0x0400)
#define f_edit_html ((ushort)0x0200)
#define f_edit_use_outlines ((ushort)0x0100)

/*
 * Header RECORDHEADER Tag type = 37.
 * Fonts used by DefineEditText must be defined using DefineFont2, not DefineFont.
 * */
typedef struct {
    // CharacterID UI16 ID for this dynamic text
    // character.
    UInt16 CharacterID;
    
    // Bounds RECT Rectangle that completely
    // encloses the text field.
    Rect Bounds;

    //
    UInt16 Flags;

    //FontID If HasFont, UI16 ID of font to use.
    UInt16 FontID;

    //FontHeight If HasFont, UI16 Height of font in twips.
    UInt16 FontHeight;


    //FontClass If HasFontClass, STRING Class name of font to be loaded
    //from another SWF and used for
    //this text.
    char FontClass[256];

    //TextColor If HasTextColor, RGBA Color of text.
    UInt32 TextColor;

    //MaxLength If HasMaxLength, UI16 Text is restricted to this length.
    UInt16 MaxLength;

    //Align If HasLayout, UI8 0 = Left
    //1 = Right
    //2 = Center
    //3 = Justify
    byte Align;

    //LeftMargin If HasLayout, UI16 Left margin in twips.
    UInt16 LeftMargin;

    //RightMargin If HasLayout, UI16 Right margin in twips.
    UInt16 RightMargin;

    //Indent If HasLayout, UI16 Indent in twips.
    UInt16 Indent;

    //Leading If HasLayout, SI16 Leading in twips (vertical
    //distance between bottom of
    //descender of one line and top of
    //ascender of the next).
    Int16 Leading;

    //VariableName STRING Name of the variable where the
    //contents of the text field are
    //stored. May be qualified with
    //dot syntax or slash syntax for
    //non-global variables.
    char VariableName[256];

    //InitialText If HasText STRING Text that is initially displayed.
    char InitialText[256];
}stDefineText;

    

class DefineEditText : public TagInstance{

    void _init();
public:
    DefineEditText();
    DefineEditText(const DefineEditText& orig);
    virtual ~DefineEditText();


    void SetInitText(const char* sz);
    const char* GetVariableName();

    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID();
    virtual void SetCharacterID(UInt16 IdNew);
    
private:
    stDefineText mDefineTextTag;
};

#endif	/* _EDITTEXT_H */

