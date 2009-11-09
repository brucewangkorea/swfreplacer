#include "DefineFont.h"

DefineFont::DefineFont(void)
{
	nGlyphs=0;
}

DefineFont::~DefineFont(void)
{
	OffsetTable.clear();
	GlyphShapeTable.clear();
}


void DefineFont::ReadData(SWFReader *swf, int l){
	printf("  DefineFont\n");

	this->FontID = swf->ReadUI16();
	printf("    FontID = %d\n", FontID);

	UInt16 nFirstOffset = swf->ReadUI16();
	this->nGlyphs = nFirstOffset/2;
	printf("    Number of glyphs = %d\n", nGlyphs);

	this->OffsetTable.push_back(nFirstOffset);
	for(int i=1; i<nGlyphs;  i++){
		this->OffsetTable.push_back( swf->ReadUI16() );
	}
	for(int i=0; i<nGlyphs;  i++){
		DefineShape *shape = new DefineShape();
		shape->ReadData(swf,0);// 0 means unknown
		this->GlyphShapeTable.push_back(*shape);
	}
}

void DefineFont::WriteData(SWFWriter *swf, int l){
	printf("  DefineFont\n");

	swf->WriteUI16(this->FontID);
	printf("    FontID = %d\n", FontID);


	list<UInt16>::iterator i;;
	list<DefineShape>::iterator j;

	for(i=OffsetTable.begin(); i!=OffsetTable.end();  ++i){
		swf->WriteUI16( (*i) );
	}
	for(j=GlyphShapeTable.begin(); j!=GlyphShapeTable.end();  ++j){
		(*j).WriteData(swf, (*j).GetLength());
	}
}

ulong DefineFont::GetLength(void){
	return this->length;
}

void DefineFont::SetLength(ulong len){
	this->length = len;
}

UInt16 DefineFont::GetCharacterID(){ 
	return FontID; 
}

void DefineFont::SetCharacterID(UInt16 IdNew){  
	FontID = IdNew;
}

void DefineFont::IncreaseCharacterID(UInt16 nAmount) {
    SetCharacterID( GetCharacterID()+nAmount );
}