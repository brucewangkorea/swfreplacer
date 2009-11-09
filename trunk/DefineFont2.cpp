#include "DefineFont2.h"

DefineFont2::DefineFont2(void)
{
	GlyphData=0;
	iGlyphDataSize=0;
}

DefineFont2::~DefineFont2(void)
{
	OffsetTable.clear();
	GlyphShapeTable.clear();
	if(GlyphData) free(GlyphData);
}


void DefineFont2::ReadData(SWFReader *swf, int l){
	printf("  ---------------------------------------\n");
	printf("  DefineFont\n");

	this->FontID = swf->ReadUI16(); //
	printf("    FontID = %d\n", FontID);
	//FontID UI16 ID for this font character.
	//FontFlagsHasLayout UB[1] Has font metrics/layout
	//information.
	FontFlagsHasLayout = swf->ReadUB(1); //
	printf( "    FontFlagsHasLayout = %s\n", FontFlagsHasLayout?"true":"false");
	//FontFlagsShiftJIS UB[1] ShiftJIS encoding.
	FontFlagsShiftJIS = swf->ReadUB(1); //
	printf( "    FontFlagsShiftJIS = %s\n", FontFlagsShiftJIS?"true":"false");
	//FontFlagsSmallText UB[1] SWF 7 or later:
	//Font is small. Character glyphs
	//are aligned on pixel boundaries
	//for dynamic and input text.
	FontFlagsSmallText = swf->ReadUB(1); //
	printf( "    FontFlagsSmallText = %s\n", FontFlagsSmallText?"true":"false");
	//FontFlagsANSI UB[1] ANSI encoding.
	FontFlagsANSI = swf->ReadUB(1); //
	printf( "    FontFlagsANSI = %s\n", FontFlagsANSI?"true":"false");
	//FontFlagsWideOffsets UB[1] If 1, uses 32 bit offsets.
	FontFlagsWideOffsets = swf->ReadUB(1); //
	printf( "    FontFlagsWideOffsets = %s\n", FontFlagsWideOffsets?"true":"false");
	//FontFlagsWideCodes UB[1] If 1, font uses 16-bit codes;
	//otherwise font uses 8 bit codes.
	FontFlagsWideCodes = swf->ReadUB(1); //
	printf( "    FontFlagsWideCodes = %s\n", FontFlagsWideCodes?"true":"false");
	//FontFlagsItalic UB[1] Italic Font.
	FontFlagsItalic = swf->ReadUB(1); //
	printf( "    FontFlagsItalic = %s\n", FontFlagsItalic?"true":"false");
	//FontFlagsBold UB[1] Bold Font.
	FontFlagsBold = swf->ReadUB(1); //
	printf( "    FontFlagsBold = %s\n", FontFlagsBold?"true":"false");
	//LanguageCode LANGCODE SWF 5 or earlier:
	//always 0
	//SWF 6 or later:
	//language code
	LanguageCode = swf->ReadByte(); //
	printf( "    LanguageCode = %d\n",LanguageCode);
	//FontNameLen UI8 Length of name.
	FontNameLen = swf->ReadByte(); //
	//FontName UI8[FontNameLen] Name of font (see
	//DefineFontInfo).
	memset(FontName, 0, sizeof(FontName)); //
	for( int i=0; i<FontNameLen; i++ ){
		FontName[i]=swf->ReadByte();
	}
	printf( "    font name = [%s]\n", FontName);
	//NumGlyphs UI16 Count of glyphs in font.
	//May be zero for device fonts.
	NumGlyphs = swf->ReadUI16(); //
	printf( "    number of glyphs = [%d]\n", NumGlyphs);
	//OffsetTable If FontFlagsWideOffsets,
	//UI32[NumGlyphs]
	//Otherwise UI16[NumGlyphs]
	//Same as in DefineFont.
	long lOffsetPos = swf->GetPosition();
	for( int i=0; i<NumGlyphs; i++ ){ //
		if(FontFlagsWideOffsets){
			OffsetTable.push_back( swf->ReadUI32() );
		}
		else{
			OffsetTable.push_back( swf->ReadUI16() );
		}
	}
	//CodeTableOffset If FontFlagsWideOffsets,
	//UI32
	//Otherwise UI16
	//Byte count from start of
	//OffsetTable to start of
	//CodeTable.
	if(FontFlagsWideOffsets){ //
		CodeTableOffset = swf->ReadUI32();
	}
	else{
		CodeTableOffset = swf->ReadUI16();
	}

	// TODO
	// Analyze and use the SHAPE information...
	//while( (swf->GetPosition() - lOffsetPos)<CodeTableOffset )
	int iByte2Read = CodeTableOffset - (swf->GetPosition() - lOffsetPos);
	iGlyphDataSize=iByte2Read;
	GlyphData=(byte*)malloc(iGlyphDataSize);
	for( int i=0; i<iByte2Read; i++ )
	{
		GlyphData[i]=swf->ReadByte();
	}
	////GlyphShapeTable SHAPE[NumGlyphs] Same as in DefineFont.
	//for( int i=0; i<NumGlyphs; i++ ){
	//	long lCurOffsetPos = swf->GetPosition() - lOffsetPos;
	//	stSHAPE *shape = new stSHAPE();
	//	shape->ReadData(swf);
	//	
	//	GlyphShapeTable.push_back( *shape );
	//}

	
	//CodeTable If FontFlagsWideCodes,
	//UI16[NumGlyphs]
	//Otherwise UI8[NumGlyphs]
	//Sorted in ascending order.
	//Always UCS-2 in SWF 6 or later.
	for( int i=0; i<NumGlyphs; i++ ){
		
		if(FontFlagsWideCodes){
			CodeTable.push_back( swf->ReadUI16() );
		}
		else{
			CodeTable.push_back( swf->ReadByte() );
		}
	}

	if(FontFlagsHasLayout){
		//FontAscent If FontFlagsHasLayout, SI16 Font ascender height.
		FontAscent = swf->ReadSI16();
		//FontDescent If FontFlagsHasLayout, SI16 Font descender height.
		FontDescent = swf->ReadSI16();
		//FontLeading If FontFlagsHasLayout, SI16 Font leading height (see
		//following).
		FontLeading = swf->ReadSI16();
		//FontAdvanceTable If FontFlagsHasLayout,
		//SI16[NumGlyphs]
		//Advance value to be used for
		//each glyph in dynamic glyph text.
		for( int i=0; i<NumGlyphs; i++ ){
			FontAdvanceTable.push_back( swf->ReadSI16() );
		}
		//FontBoundsTable If FontFlagsHasLayout,
		//RECT[NumGlyphs]
		//Not used in Flash Player through
		//version 7 (but must be present).
		for( int i=0; i<NumGlyphs; i++ ){
			Rect *rect = new Rect();
			rect->Read(swf);
			FontBoundsTable.push_back( *rect );
		}
		//KerningCount If FontFlagsHasLayout, UI16 Not used in Flash Player through
		//version 7 (always set to 0 to save
		//space).
		KerningCount = swf->ReadUI16();
		//FontKerningTable If FontFlagsHasLayout,
		//KERNINGRECORD
		//[KerningCount]
		//Not used in Flash Player through
		//version 7 (omit with
		//KerningCount of 0).
		for( int i=0; i<KerningCount; i++ ){
			stKERNINGRECORD *kern = new stKERNINGRECORD();
			//kern
			kern->ReadData(swf, this->FontFlagsWideCodes );
			FontKerningTable.push_back( *kern );
		}
	}
	printf("  DefineFont2_END\n");
	printf("  ---------------------------------------\n");
}

void DefineFont2::WriteData(SWFWriter *swf, int l){
	printf("  ---------------------------------------\n");
	printf("  DefineFont\n");

	swf->WriteUI16(FontID);
	printf("    FontID = %d\n", FontID);
	//FontID UI16 ID for this font character.
	//FontFlagsHasLayout UB[1] Has font metrics/layout
	//information.
	swf->WriteUB(FontFlagsHasLayout, 1);
	//FontFlagsShiftJIS UB[1] ShiftJIS encoding.
	swf->WriteUB(FontFlagsShiftJIS, 1);
	//FontFlagsSmallText UB[1] SWF 7 or later:
	//Font is small. Character glyphs
	//are aligned on pixel boundaries
	//for dynamic and input text.
	swf->WriteUB(FontFlagsSmallText, 1);
	//FontFlagsANSI UB[1] ANSI encoding.
	swf->WriteUB(FontFlagsANSI, 1);
	//FontFlagsWideOffsets UB[1] If 1, uses 32 bit offsets.
	swf->WriteUB(FontFlagsWideOffsets, 1);
	//FontFlagsWideCodes UB[1] If 1, font uses 16-bit codes;
	//otherwise font uses 8 bit codes.
	swf->WriteUB(FontFlagsWideCodes, 1);
	//FontFlagsItalic UB[1] Italic Font.
	swf->WriteUB(FontFlagsItalic, 1);
	//FontFlagsBold UB[1] Bold Font.
	swf->WriteUB(FontFlagsBold, 1);
	//LanguageCode LANGCODE SWF 5 or earlier:
	//always 0
	//SWF 6 or later:
	//language code
	swf->WriteByte(LanguageCode);
	//FontNameLen UI8 Length of name.
	swf->WriteByte(FontNameLen);
	//FontName UI8[FontNameLen] Name of font (see
	//DefineFontInfo).
	for( int i=0; i<FontNameLen; i++ ){
		swf->WriteByte(FontName[i]);
	}
	//NumGlyphs UI16 Count of glyphs in font.
	//May be zero for device fonts.
	swf->WriteUI16(NumGlyphs);
	//OffsetTable If FontFlagsWideOffsets,
	//UI32[NumGlyphs]
	//Otherwise UI16[NumGlyphs]
	//Same as in DefineFont.
	list<UInt32>::iterator iter;
	for(iter=OffsetTable.begin(); iter!=OffsetTable.end(); ++iter ){
		if(FontFlagsWideOffsets){
			swf->WriteUI32( (*iter)  );
		}
		else{
			swf->WriteUI16( (*iter)  );
		}
	}
	//CodeTableOffset If FontFlagsWideOffsets,
	//UI32
	//Otherwise UI16
	//Byte count from start of
	//OffsetTable to start of
	//CodeTable.
	if(FontFlagsWideOffsets){
		swf->WriteUI32(CodeTableOffset);
	}
	else{
		swf->WriteUI16(CodeTableOffset);
	}

	// TODO
	// Analyze and use the SHAPE information...
	for( int i=0; i<iGlyphDataSize; i++ )
	{
		swf->WriteByte(GlyphData[i]);
	}
	////GlyphShapeTable SHAPE[NumGlyphs] Same as in DefineFont.
	//list<stSHAPE>::iterator iter2;
	//for(iter2=GlyphShapeTable.begin(); iter2!=GlyphShapeTable.end(); ++iter2 ){
	//	(*iter2).WriteData(swf);
	//}

	//CodeTable If FontFlagsWideCodes,
	//UI16[NumGlyphs]
	//Otherwise UI8[NumGlyphs]
	//Sorted in ascending order.
	//Always UCS-2 in SWF 6 or later.
	list<UInt16>::iterator iter3;
	for(iter3=CodeTable.begin(); iter3!=CodeTable.end(); ++iter3 ){
		if(FontFlagsWideCodes){
			swf->WriteUI16( (*iter3) );
		}
		else{
			swf->WriteByte( (*iter3) );
		}
	}

	//
	if(FontFlagsHasLayout){
		//FontAscent If FontFlagsHasLayout, SI16 Font ascender height.
		swf->WriteSI16(FontAscent);
		//FontDescent If FontFlagsHasLayout, SI16 Font descender height.
		swf->WriteSI16(FontDescent);
		//FontLeading If FontFlagsHasLayout, SI16 Font leading height (see
		//following).
		swf->WriteSI16(FontLeading);
		//FontAdvanceTable If FontFlagsHasLayout,
		//SI16[NumGlyphs]
		//Advance value to be used for
		//each glyph in dynamic glyph text.
		list<Int16>::iterator iter4;
		for(iter4=FontAdvanceTable.begin(); iter4!=FontAdvanceTable.end(); ++iter4 ){
			swf->WriteSI16( (*iter4) );
		}

		//FontBoundsTable If FontFlagsHasLayout,
		//RECT[NumGlyphs]
		//Not used in Flash Player through
		//version 7 (but must be present).
		list<Rect>::iterator iter5;
		for(iter5=FontBoundsTable.begin(); iter5!=FontBoundsTable.end(); ++iter5 ){
			(*iter5).Write(swf);
		}
		//KerningCount If FontFlagsHasLayout, UI16 Not used in Flash Player through
		//version 7 (always set to 0 to save
		//space).
		swf->WriteUI16(KerningCount);
		//FontKerningTable If FontFlagsHasLayout,
		//KERNINGRECORD
		//[KerningCount]
		//Not used in Flash Player through
		//version 7 (omit with
		//KerningCount of 0).
		list<stKERNINGRECORD> FontKerningTable;
		list<stKERNINGRECORD>::iterator iter6;
		for(iter6=FontKerningTable.begin(); iter6!=FontKerningTable.end(); ++iter6 ){
			(*iter6).WriteData(swf, this->FontFlagsWideCodes);
		}
	}
	printf("  DefineFont2_END\n");
	printf("  ---------------------------------------\n");
}

ulong DefineFont2::GetLength(void){
	return this->length;
}

void DefineFont2::SetLength(ulong len){
	this->length = len;
}

UInt16 DefineFont2::GetCharacterID(){ 
	return FontID; 
}

void DefineFont2::SetCharacterID(UInt16 IdNew){  
	FontID = IdNew;
}

void DefineFont2::IncreaseCharacterID(UInt16 nAmount) {
    SetCharacterID( GetCharacterID()+nAmount );
}