/* 
 * File:   SWFFile.cpp
 * Author: brucewang
 * 
 * Created on October 24, 2009, 9:27 PM
 */

#include "SWFFile.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "DefineEditText.h"
#include "DefineJPEG2.h"
#include "Exports.h"
#include "DefineSprite.h"


/// Destructor

SWFFile::~SWFFile() {
    if (SwfData != 0) {
        free(SwfData);
    }

    if (this->swf)
        delete this->swf;
}

//
//
//
//#region Constructors

SWFFile::SWFFile(const char* fileName) {
    //fileName = \0;
    //signature = String.Empty;


    long lFileSize = 0L;
    SwfData = 0;
    FILE *fp = fopen(fileName, "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        lFileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (lFileSize > 0) {
            SwfData = (unsigned char *) malloc(lFileSize);
            if (SwfData) {
                if (fread(SwfData, 1, lFileSize, fp) > 0) {
                    this->swf = new SWFReader(SwfData); // deletion is done at the destructor

                    if (ReadHeader()) {
                        // Just identify the tag types
                        // ** This would normally be the place to start processing tags **
                        IdentifyTags();
                        //printf("## FILE SIZE = %d\n", this->m_Header.FileLength());
                    }
                }// read file
            }// memory allocation
        }// get file size

        fclose(fp);
    }// file open
}

bool SWFFile::ReadHeader()//byte * SwfData)
{
    this->m_Header.Read(this->swf);
    //printf("##Header Length = %d\n", this->m_Header.HeaderLength());
    return true;
}

//
//
//
// Doesn't do much but iterate through the tags

void SWFFile::IdentifyTags()//unsigned char * SwfData)
{
    m_TagList.clear();
    Tag *p;

    do {
        p = new Tag(swf);
        m_TagList.push_back(*p);
    } while (p->ID() != 0);


    list<Tag>::iterator i;
    int iContentsLength = 0;
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        iContentsLength += (*i).GetLength();
    }
    //printf("#Content length = %d\n", iContentsLength);
}



//#region Properties

szstring SWFFile::FileName() {
    return fileName;
}


//#endregion

void SWFFile::ChangeEditValue(
        const char *szVariableName,
        const char *szInitialValue) {

    list<Tag>::iterator i;
    DefineEditText* pEditText;
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        if ((*i).ID() == 37) // DefineEditText
        {

            pEditText = (DefineEditText*) ((*i).GetTagContent());
#if 0 
            // For test... change all variables..
#else
            if (0 == strcmp(pEditText->GetVariableName(), szVariableName))
#endif
                pEditText->SetInitText(szInitialValue);
        }
    }
}


// Find matching character id for given exportname.
// Returns 0 when couldn't find it.
UInt16 SWFFile::FindMatchingIdFromExptName(const char* szExportName){
    list<Tag>::iterator i;
    UInt16 nId2Change = 0;
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        if ((*i).ID() == 56) // Export
        {
            Exports* pItem = (Exports*)( (*i).GetTagContent() );
            stExportAssets* pExport = pItem->GetExportData();
            list<stExportItem>::iterator JJ;
            for (JJ = pExport->Items.begin(); JJ != pExport->Items.end(); ++JJ) {
                if( 0 == strcmp( (*JJ).Name, szExportName) ){
                    nId2Change = (*JJ).ID;
                    break;
                }
            }
        }

        if(nId2Change!=0){
            break;
        }
    }
    if( 0== nId2Change ){
        printf("# Could not find Matching Export [%s].. Canceling image replace\n", szExportName);
        return 0;
    }

    return nId2Change;
}

void SWFFile::ChangeJpgImg(const char* szExportName, const char* szFilePath2Replacewith) {
    // 1. Find matching character id for given exportname.
    UInt16 nId2Change = FindMatchingIdFromExptName(szExportName);
    if( nId2Change==0 ) return;

    // 2. Change it..
    list<Tag>::iterator i;
    DefineJPEG2* pJpg;
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        if ((*i).ID() == 21) // DefineBitsJPEG2
        {
            pJpg = (DefineJPEG2*) ((*i).GetTagContent());
            // For test... change all variables..
            if (pJpg->GetCharacterID() == nId2Change)
                pJpg->ReplaceImg(szFilePath2Replacewith);
        }
    }
}

void SWFFile::ChangeSprite(const char* szExportName, const char* szFilePath2Replacewith){
    // 1. Find matching character id for given exportname.
    UInt16 nId2Change = FindMatchingIdFromExptName(szExportName);
    if( nId2Change==0 ) return;

    // 2. Change it..
    list<Tag>::iterator i;
    DefineSprite* pInst;
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        if ((*i).ID() == 39) // DefineSprite
        {
            pInst = (DefineSprite*) ((*i).GetTagContent());
            // For test... change all variables..
            if (pInst->GetCharacterID() == nId2Change){
                pInst->ReplaceSprite(&m_TagList, szFilePath2Replacewith);
                //break;
            }
        }
    }
}

ulong SWFFile::SaveTo(char* filename) {
    SWFWriter swf;
    list<Tag>::iterator i;

    //
    // Check the contents size. (Compression is not considered)
    int iContentsLength = 0;
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        iContentsLength += (*i).GetLength();
    }
    this->m_Header.ChangeContentsLength(iContentsLength);

    //
    // Write Header
    this->m_Header.Write(&swf);

    //
    // Write the rest of the contents.
    for (i = this->m_TagList.begin(); i != this->m_TagList.end(); ++i) {
        (*i).Write(&swf);
    }

    swf.SaveFile(filename);

    return 0L;
}


