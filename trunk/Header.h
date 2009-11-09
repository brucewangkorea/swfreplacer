/* 
 * File:   Header.h
 * Author: brucewang
 *
 * Created on October 27, 2009, 10:38 AM
 */

#ifndef _HEADER_H
#define	_HEADER_H

#include "SWFReader.h"
#include "Rect.h"
#include "types.h"
#include "SWFWriter.h"

class Header {
    char signature[6];
    byte version;
    uint fileLength;
    uint contentLength;
    int frameWidth;
    int frameHeight;
    Single frameRate;
    ushort frameCount;
    ushort frameRateMinor;
    ushort frameRateMajor;

    Rect m_frameSize;
public:
    Header();
    Header(const Header& orig);
    virtual ~Header();

    bool Read(SWFReader *swf);
    bool Write(SWFWriter *swf);

    szstring Signature();
    byte Version();
    uint FileLength();
    uint HeaderLength();
    int FrameWidth();
    int FrameHeight();
    Single FrameRate();
    ushort FrameCount();

    // Change the lengh of sum of all internal Tag contents,
    // excluding the file header.
    // File compression is not considered.
    void ChangeContentsLength(int len);
private:

};

#endif	/* _HEADER_H */

