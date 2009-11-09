#include "DefineText.h"

DefineText::DefineText(void)
{
}

DefineText::~DefineText(void)
{
	TextRecords.clear();
}


void DefineText::ReadData(SWFReader *swf, int l){
	//CharacterID UI16 ID for this text character.
	CharacterID = swf->ReadUI16();
	//TextBounds RECT Bounds of the text.
	TextBounds.Read(swf);
	//TextMatrix MATRIX Transformation matrix for the
	//text.
	TextMatrix.Read_Bits(swf);
	//GlyphBits UI8 Bits in each glyph index.
	GlyphBits = swf->ReadByte();
	//AdvanceBits UI8 Bits in each advance value.
	AdvanceBits = swf->ReadByte();
	//TextRecords TEXTRECORD[zero or more] Text records.
	list<stTEXTRECORD>::iterator itr;
	for( itr=TextRecords.begin(); itr!=TextRecords.end(); ++itr ){
		if( (*itr).Read(swf) <= 8 ){
			break;
		}
	}
	//EndOfRecordsFlag UI8 Must be 0.
}

void DefineText::WriteData(SWFWriter *swf, int l){
	//CharacterID UI16 ID for this text character.
	swf->WriteUI16(CharacterID);
	//TextBounds RECT Bounds of the text.
	TextBounds.Write(swf);
	//TextMatrix MATRIX Transformation matrix for the
	//text.
	TextMatrix.Write_Bits(swf);
	//GlyphBits UI8 Bits in each glyph index.
	swf->WriteByte(GlyphBits);
	//AdvanceBits UI8 Bits in each advance value.
	swf->WriteByte(AdvanceBits);
	//TextRecords TEXTRECORD[zero or more] Text records.
	list<stTEXTRECORD>::iterator itr;
	for( itr=TextRecords.begin(); itr!=TextRecords.end(); ++itr ){
		if( (*itr).Write(swf) <= 8 ){
			break;
		}
	}
}

ulong DefineText::GetLength(void){
	return this->length;
}

void DefineText::SetLength(ulong len){
	this->length = len;
}

UInt16 DefineText::GetCharacterID(){ 
	return CharacterID; 
}

void DefineText::SetCharacterID(UInt16 IdNew){  
	CharacterID = IdNew;
}

void DefineText::IncreaseCharacterID(UInt16 nAmount) {
    SetCharacterID( GetCharacterID()+nAmount );

	list<stTEXTRECORD>::iterator itr;
	for( itr=TextRecords.begin(); itr!=TextRecords.end(); ++itr ){
		(*itr).FontID += nAmount;
	}
}