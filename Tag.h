/* 
 * File:   Tag.h
 * Author: brucewang
 *
 * Created on October 24, 2009, 9:05 PM
 */

#ifndef _TAG_H
#define	_TAG_H

#include "types.h"
#include "SWFReader.h"
#include "SWFWriter.h"
#include "TagInstance.h"


#include <map>




///-----------------------------------------------------------
///
///
///-----------------------------------------------------------

class Tag {
protected:
    ushort tagInfo;
    ushort id;

    void _readIdAndLength(SWFReader *swf);
    void _writeIdAndLength(SWFWriter *swf);

    TagInstance* m_pTagInstance;

    
    
    static std::map<unsigned short, const char*> m_IdNameTable;
    static void _initMatchTable();


public:

    ushort ID();

    ulong GetLength();
    TagInstance* GetTagContent(){
        return m_pTagInstance;
    }


    /*
     *	2007-08-07 brucewang
     *
     */
    long lTagPosition;

    Tag(){
        m_pTagInstance = 0;
        _initMatchTable();
    }
    Tag(SWFReader *swf);
    ~Tag();
    void Read(SWFReader *swf);
    void Write(SWFWriter *swf);

    // Return the tag type
    // Special thanks to Alexis: http://sswf.sourceforge.net/SWFalexref.html#table_of_swf_tags

    const char* GetType(ushort id);
};


#endif	/* _TAG_H */

