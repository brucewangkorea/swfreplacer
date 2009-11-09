/* 
 * File:   Exports.h
 * Author: brucewang
 *
 * Created on October 29, 2009, 3:41 PM
 */

#ifndef _EXPORTS_H
#define	_EXPORTS_H

#include "types.h"
#include "TagInstance.h"

#include <list>
using namespace std;

typedef struct{
    // character ID to export
    UInt16 ID;

    // Identifier for first exported character
    char Name[256];
}stExportItem;

typedef struct {
    // Number of assets to export
    UInt16 Count;

    // list of 'stExportItem'
    list<stExportItem> Items;
} stExportAssets;

class Exports : public TagInstance {
public:
    Exports();
    Exports(const Exports& orig);
    virtual ~Exports();

    virtual void ReadData(SWFReader *swf, int length);
    virtual void WriteData(SWFWriter *swf, int length);
    virtual ulong GetLength(void);
    virtual void SetLength(ulong len);

    virtual UInt16 GetCharacterID(){ return 0; }
    virtual void SetCharacterID(UInt16 IdNew){  }
    virtual void IncreaseCharacterID(UInt16 nAmount);

    stExportAssets* GetExportData(){ return &m_ExportTag; }
private:
    stExportAssets m_ExportTag;
};

#endif	/* _EXPORTS_H */

