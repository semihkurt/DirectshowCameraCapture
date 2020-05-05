#ifndef SAMPLEGRABBERCALLBACK_H
#define SAMPLEGRABBERCALLBACK_H
#include <dshow.h>
#include "qedit.h"
#include <iostream>
#include <string>
#include <mutex>
using namespace std;

class SampleGrabberCallback : public ISampleGrabberCB
{
public:
    SampleGrabberCallback(){}
    HRESULT SampleCB(double SampleTime, IMediaSample *pSample);
    HRESULT BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen);

    HRESULT QueryInterface(const IID &riid, void **ppvObject){cout << "QueryInterface"; return 1;}
    ULONG AddRef(){cout << "Added REF" << endl; return 1;}
    ULONG Release(){cout << "Released" << endl; return 1;}
private:
    ::std::mutex mMutex;
};

#endif // SAMPLEGRABBERCALLBACK_H
