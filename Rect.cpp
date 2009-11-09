/* 
 * File:   Rect.cpp
 * Author: brucewang
 * 
 * Created on October 24, 2009, 9:09 PM
 */

#include "Rect.h"


int Rect::GetSize(){
    return iTotalBytes;
}


int Rect::XMin() {
    return xMin;
}

int Rect::XMax() {
    return xMax;
}

int Rect::YMin() {
    return yMin;
}

int Rect::YMax() {
    return yMax;
}

Rect::Rect(){
}

Rect::Rect(SWFReader *swf){
    Read(swf);
}

int Rect::Read(SWFReader *swf) {
    nBits = (int) swf->ReadUB(5);
    xMin = swf->ReadSB(nBits);
    xMax = swf->ReadSB(nBits);
    yMin = swf->ReadSB(nBits);
    yMax = swf->ReadSB(nBits);

    iTotalBytes = (int) ((5 + nBits * 4) / 8);
    iTotalBytes += ((5 + nBits * 4) % 8) > 0 ? 1 : 0;

    return iTotalBytes;
}


int Rect::Write(SWFWriter *swf){
    //SWFWriter::WriteUB()
    swf->WriteUB(nBits,5);
    swf->WriteSB(xMin,nBits);
    swf->WriteSB(xMax,nBits);
    swf->WriteSB(yMin,nBits);
    swf->WriteSB(yMax,nBits);

    iTotalBytes = (int) ((5 + nBits * 4) / 8);
    iTotalBytes += ((5 + nBits * 4) % 8) > 0 ? 1 : 0;

    return iTotalBytes;
}