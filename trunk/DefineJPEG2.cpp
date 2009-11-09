/* 
 * File:   JPEG2.cpp
 * Author: brucewang
 * 
 * Created on October 27, 2009, 4:07 PM
 */

#include "DefineJPEG2.h"

void DefineJPEG2::_init() {
    memset(&mJPEG2Tag, 0, sizeof (mJPEG2Tag));
    length = 0;
    m_iImgLen = 0;

    marker_start[0] = 0xff;
    marker_start[1] = 0xd8;
    marker_end[0] = 0xff;
    marker_end[1] = 0xd9;
}

DefineJPEG2::DefineJPEG2() {
    _init();
}

DefineJPEG2::DefineJPEG2(const DefineJPEG2& orig) {
    _init();
}

DefineJPEG2::~DefineJPEG2() {
    if (mJPEG2Tag.pImageData) free(mJPEG2Tag.pImageData);
}

ulong DefineJPEG2::GetLength(void) {
    if (mJPEG2Tag.CharacterID == 0) {
        return this->length;
    }


    int iTotSize = 0;
    iTotSize += 2; // mJPEG2Tag.CharacterID
    iTotSize += sizeof (marker_start);
    iTotSize += sizeof (marker_end);
    iTotSize += m_iImgLen;

    return iTotSize;
}

void DefineJPEG2::SetLength(ulong len) {
    this->length = len;
}

void DefineJPEG2::ReadData(SWFReader *swf, int length) {
    mJPEG2Tag.CharacterID = swf->ReadUI16();
    mJPEG2Tag.pImageData = (byte*) malloc(length);
    printf("  JPG character ID = %d\n", mJPEG2Tag.CharacterID);


    bool bStartFound = false;
    bool bEndFound = false;
    byte bOld = 0x00;
    byte bCur = 0x00;
    int iImgLen = 0;
    for (int i = 0; i < length - 2; i++) {
        bCur = swf->ReadByte();
        if (bStartFound) {
            if (bCur == marker_end[1] && bOld == marker_end[0]) { // EndMarker
                bEndFound = true;
            }

            if (!bEndFound) {
                mJPEG2Tag.pImageData[ iImgLen++ ] = bCur;
            } else {
                iImgLen--;
            }
        } else {
            if (i > 0 &&
                    bCur == marker_start[1] &&
                    bOld == marker_start[0]) {
                bStartFound = true;
            }
        }
        bOld = bCur;
    }
    // Remember the Image data size..
    m_iImgLen = iImgLen;


#if 1 // For test.. Extract Image data and save it as a file
    char szImgFile[256];
    sprintf(szImgFile, "%d.jpg", mJPEG2Tag.CharacterID);
    FILE *fp = fopen(szImgFile, "wb");
    if (fp) {
        for (int i = 0; i < m_iImgLen; i++) {
            fwrite(&(this->mJPEG2Tag.pImageData[i]), 1, 1, fp);
        }
        fclose(fp);
    }
#endif

}

void DefineJPEG2::WriteData(SWFWriter *swf, int length) {
    swf->WriteUI16(mJPEG2Tag.CharacterID);
    printf("  JPG character ID = %d\n", mJPEG2Tag.CharacterID);

    // Write Start Marker
    for (int i = 0; i<sizeof (marker_start); i++) swf->WriteByte(marker_start[i]);

    // Image Data
    for (int i = 0; i < m_iImgLen; i++) {
        swf->WriteByte(mJPEG2Tag.pImageData[i]);
    }

    // Write End Marker
    for (int i = 0; i<sizeof (marker_end); i++) swf->WriteByte(marker_end[i]);
}

void DefineJPEG2::ReplaceImg(const char* szFilePath2Replacewith) {
    FILE *fp = fopen(szFilePath2Replacewith, "rb");
    if (fp) {
        if (mJPEG2Tag.pImageData) free(mJPEG2Tag.pImageData);

        fseek(fp, 0L, SEEK_END);
        long lfilesize = ftell(fp);
        fseek(fp, 0L, SEEK_SET);


        m_iImgLen = lfilesize;
        mJPEG2Tag.pImageData = (byte*) malloc(lfilesize);
        fread(this->mJPEG2Tag.pImageData, 1, lfilesize, fp);
        fclose(fp);
    }
}