/* 
 * File:   Header.cpp
 * Author: brucewang
 * 
 * Created on October 27, 2009, 10:38 AM
 */

#include "Header.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


Header::Header() {
    contentLength=0;
}

Header::Header(const Header& orig) {
}

Header::~Header() {
}

// Change the lengh of sum of all internal Tag contents,
// excluding the file header.
// File compression is not considered.
void Header::ChangeContentsLength(int len){
    contentLength=len;
    //printf("Now the Content length is %d.\n", contentLength);
}


uint Header::HeaderLength(){
    int iLen = 0;

    iLen += 3; // Signature
    iLen += 1; // File Version
    iLen += 4; // File Length
    iLen += m_frameSize.GetSize(); // Frame Size
    iLen += 2; // Frame rate
    iLen += 2; // Frame count

    return iLen;
}

uint Header::FileLength() {
    if( contentLength== 0){
        printf("Content length is 0.\n");
        return fileLength;
    }

    int iLen = 0;
    iLen = this->HeaderLength() + contentLength; // And the rest..

    return iLen;
}




bool Header::Read(SWFReader *swf){
    // Read file signature
    memset(signature, 0, 6);

    szstring strTemp = 0;
    strTemp = (szstring)swf->ReadUI8(3); // this is deleted properly after use.
    memcpy(signature, strTemp, 3); // "FWS" or "CWS" for ZLIB compressed (v6.0 or later)
    if (strTemp) delete strTemp;

    if (strncmp(signature, "FWS", 3) != 0 &&
            strncmp(signature, "CWS", 3) != 0) {
        printf("Not a valid SWF (Flash) file signature\n");

        return false;
    }
    printf("Signature   : %s\n", Signature());

    // Read file version
    version = swf->ReadUI8();
    printf("Version     : %d\n", Version());

    // File length
    fileLength = swf->ReadUI32();
    printf("File length : %d bytes\n", FileLength());

    // If the file is compressed, this is where the ZLIB decompression ("inflate") begins
    if (strncmp(signature, "CWS", 3) == 0) {
        printf("Currently I don't support compressed SWF file.\n");
        return false;
    }

    // Frame size
    m_frameSize.Read(swf); // deletion is done within this function
    frameWidth = m_frameSize.XMax();
    frameHeight = m_frameSize.YMax();
    printf("Frame width : %d twips (%d pixels)\n", FrameWidth(), FrameWidth() / 20);
    printf("Frame height: %d twips (%d pixels)\n", FrameHeight(), FrameHeight() / 20);

    // Frame rate (stored in UI8.UI8 format)
    frameRateMinor = swf->ReadUI8();
    frameRateMajor = swf->ReadUI8();
    char szframerate[64];
    sprintf(szframerate, "%d.%d", frameRateMajor, frameRateMinor);
    frameRate = (Single) atof(szframerate); // TODO: Improve this later
    printf("Frame rate  : %f fps\n", FrameRate());

    // Frame count
    frameCount = swf->ReadUI16();
    printf("Frame count : %d\n", FrameCount());
    
    return true;
}

bool Header::Write(SWFWriter *swf){
    // "FWS" or "CWS" for ZLIB compressed (v6.0 or later)
    for(int i=0; i<3; i++){
        swf->WriteByte(this->signature[i]);
    }

    // Read file version
    swf->WriteByte(this->version);

    // File length
    swf->WriteUI32( this->FileLength() );
    

    // Frame size
    m_frameSize.Write(swf);
    
    // Frame rate (stored in UI8.UI8 format)
    swf->WriteUI8(frameRateMinor);
    swf->WriteUI8(frameRateMajor);
    
    // Frame count
    swf->WriteUI16(frameCount);

    return true;
}











szstring Header::Signature() {
    return signature;
}

byte Header::Version() {
    return version;
}


int Header::FrameWidth() {
    return frameWidth;
}

int Header::FrameHeight() {
    return frameHeight;
}

Single Header::FrameRate() {
    return frameRate;
}

ushort Header::FrameCount() {
    return frameCount;
}
