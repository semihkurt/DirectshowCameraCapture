#ifndef SAMPLEGRABBERCALLBACK_H
#define SAMPLEGRABBERCALLBACK_H
#include <dshow.h>
#include "qedit.h"
#include <iostream>
#include <string>
using namespace std;

class SampleGrabberCallback : public ISampleGrabberCB
{
    // ISampleGrabberCB interface
public:
//    virtual ~SampleGrabberCallback();
    SampleGrabberCallback(){}
    HRESULT SampleCB(double SampleTime, IMediaSample *pSample){
        cout << "Sample time: " << std::to_string(SampleTime) << endl;
        return 1;
    }
    HRESULT BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){
        cout << "Buffer Length: " << std::to_string(BufferLen) << endl;
        return 1;
    }

public:
    HRESULT QueryInterface(const IID &riid, void **ppvObject){cout << "QueryInterface"; return 1;}
    ULONG AddRef(){cout << "Added REF" << endl; return 1;}
    ULONG Release(){cout << "Released" << endl; return 1;}
};

#endif // SAMPLEGRABBERCALLBACK_H
