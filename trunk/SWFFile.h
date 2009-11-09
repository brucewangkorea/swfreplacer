/* 
 * File:   SWFFile.h
 * Author: brucewang
 *
 * Created on October 24, 2009, 9:27 PM
 */

#ifndef _SWFFILE_H
#define	_SWFFILE_H

#include "SWFReader.h"
#include "Tag.h"
#include "Rect.h"
#include "Header.h"

#include <list>
using namespace std;

class SWFFile {
private:
    SWFReader *swf;
    szstring fileName;

    unsigned char *SwfData;

    list<Tag> m_TagList;
    Header m_Header;

    //
    //
    //

    bool ReadHeader();

    //
    //
    //
    // Doesn't do much but iterate through the tags

    void IdentifyTags();



public:
    //#region Properties

    szstring FileName();

    list<Tag>* GetTagList(){ return (list<Tag> *)&m_TagList; }


    //#endregion

    void ChangeEditValue(const char *szVariableName, const char *szInitialValue);
    void ChangeJpgImg(const char* szExportName, const char* szFilePath2Replacewith);
    void ChangeSprite(const char* szExportName, const char* szFilePath2Replacewith);

    UInt16 FindMatchingIdFromExptName(const char* szExportName);

    ulong SaveTo(char* filename);


    /// Destructor

    ~SWFFile();

    //
    //
    //
    //#region Constructors

    SWFFile(const char* fileName);
    //#endregion
};


#endif	/* _SWFFILE_H */

