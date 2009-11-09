/* 
 * File:   Exports.cpp
 * Author: brucewang
 * 
 * Created on October 29, 2009, 3:41 PM
 */

#include "Exports.h"

Exports::Exports() {
}

Exports::Exports(const Exports& orig) {
}

Exports::~Exports() {
}


ulong Exports::GetLength(void){
    if( m_ExportTag.Count == 0 )
        return this->length;

    ulong lLen = 0;
    lLen += 2; // Count
    list<stExportItem>::iterator i;
    for (i = this->m_ExportTag.Items.begin(); i != this->m_ExportTag.Items.end(); ++i) {
        lLen += 2; // ID
        lLen += strlen((*i).Name) + 1;
    }
    return lLen;
}


void Exports::SetLength(ulong len){
    this->length = len;
}

void Exports::ReadData(SWFReader *swf, int length){
    m_ExportTag.Count = swf->ReadUI16();
    m_ExportTag.Items.clear();
    for( int i=0; i<m_ExportTag.Count; i++ ){
        stExportItem a;
        a.ID = swf->ReadUI16();
        strcpy( a.Name, swf->ReadSTRING() );
        m_ExportTag.Items.push_back( a );

        printf( "  Exported {ID => %d, Name => \"%s\"}\n", a.ID, a.Name );
    }
}




void Exports::WriteData(SWFWriter *swf, int length){
    swf->WriteUI16( m_ExportTag.Count );
    list<stExportItem>::iterator i;
    for (i = this->m_ExportTag.Items.begin(); i != this->m_ExportTag.Items.end(); ++i) {
        swf->WriteUI16( (*i).ID );
        swf->WriteSTRING( (*i).Name );

        printf( "  Exported {ID => %d, Name => \"%s\"}\n", (*i).ID, (*i).Name );
    }
}

void Exports::IncreaseCharacterID(UInt16 nAmount) {
    SetCharacterID( GetCharacterID()+nAmount );

    list<stExportItem>::iterator i;
    for (i = this->m_ExportTag.Items.begin(); i != this->m_ExportTag.Items.end(); ++i) {
        (*i).ID += nAmount;
    }
}