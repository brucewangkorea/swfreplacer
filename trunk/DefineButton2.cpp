#include "DefineButton2.h"

DefineButton2::DefineButton2(void)
{
}

DefineButton2::~DefineButton2(void)
{
}


void DefineButton2::ReadData(SWFReader *swf, int l){
	this->length = mButton.Read(swf);
	printf( "  #Button length : (Org=%d, Read=%d)\n", l, this->length );
}
void DefineButton2::WriteData(SWFWriter *swf, int l){
	this->length = mButton.Write(swf);
	printf( "  #Button length : (Org=%d, Written=%d)\n", l, this->length );
}
ulong DefineButton2::GetLength(void){
	return this->length;
}
void DefineButton2::SetLength(ulong len){
	this->length = len;
}

UInt16 DefineButton2::GetCharacterID(){
	return mButton.ButtonId;
}
void DefineButton2::SetCharacterID(UInt16 IdNew){
	mButton.ButtonId = IdNew;
}
void DefineButton2::IncreaseCharacterID(UInt16 nAmount){
	SetCharacterID( GetCharacterID()+nAmount );
	for(int i=0; i<mButton.mi_NumCharacters; i++){
		mButton.Characters[i].CharacterID += nAmount;
	}
}