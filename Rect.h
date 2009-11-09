/* 
 * File:   Rect.h
 * Author: brucewang
 *
 * Created on October 24, 2009, 9:09 PM
 */

#ifndef _RECT_H
#define	_RECT_H

#include "SWFReader.h"
#include "SWFWriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

///-----------------------------------------------------------
///
///
///-----------------------------------------------------------

class Rect {
private:
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    int nBits;

    /*
     * 2007-08-07 brucewang
     */
    int iTotalBytes;

public:

    int GetSize();
    int XMin();
    int XMax();
    int YMin();
    int YMax();
    Rect();
    Rect(SWFReader *swf);
    int Read(SWFReader *swf);
    int Write(SWFWriter *swf);
};



#endif	/* _RECT_H */

