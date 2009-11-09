#include "DefineShape.h"

DefineShape::DefineShape(void)
{
}

DefineShape::~DefineShape(void)
{
}


void DefineShape::ReadData(SWFReader *swf, int l){
	this->length = Shape.Read(swf, l);
	printf( "  #Shape length : (Org=%d, Read=%d)\n", l, this->length );
}

void DefineShape::WriteData(SWFWriter *swf, int l){
	this->length = Shape.Write(swf, l);
	printf( "  #Shape length : (Org=%d, Written=%d)\n", l, this->length );
}

ulong DefineShape::GetLength(void){
	return this->length;
}

void DefineShape::SetLength(ulong len){
	this->length = len;
}

UInt16 DefineShape::GetCharacterID(){ 
	return Shape.ShapeId; 
}

void DefineShape::SetCharacterID(UInt16 IdNew){  
	Shape.ShapeId = IdNew;
}

void DefineShape::IncreaseCharacterID(UInt16 nAmount) {
    SetCharacterID( GetCharacterID()+nAmount );

	for( int i=0; i<Shape.Shapes.FillStyles.iRealCount; i++ ){
		//0x40, 0x41, 0x42 or 0x43
		if( Shape.Shapes.FillStyles.pStyles[i].FillStyleType==0x40 || 
			Shape.Shapes.FillStyles.pStyles[i].FillStyleType==0x41 ||
			Shape.Shapes.FillStyles.pStyles[i].FillStyleType==0x42 ||
			Shape.Shapes.FillStyles.pStyles[i].FillStyleType==0x43 ){
				Shape.Shapes.FillStyles.pStyles[i].BitmapId += nAmount;
		}
	}
}