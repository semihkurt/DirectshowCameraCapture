#ifndef SAMPLEGRABBERCALLBACKCLASS_H
#define SAMPLEGRABBERCALLBACKCLASS_H
#include <qedit.h>
#include "directshowsamplegrabber.h"
#include <iostream>
#include <string>

using namespace std;

class SampleGrabberCallbackClass : public ISampleGrabberCB
{
//    Q_DISABLE_COPY(SampleGrabberCallbackPrivate)
public:
    virtual ~SampleGrabberCallbackClass(){}
    explicit SampleGrabberCallbackClass(DirectShowSampleGrabber *grabber);


//    virtual ~SampleGrabberCallbackPrivate() = default;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;
    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject) override;
    STDMETHODIMP SampleCB(double time, IMediaSample *mediaSample) override;
    STDMETHODIMP BufferCB(double time, BYTE *buffer, long bufferLen) override
;
private:
    ULONG m_ref;
    DirectShowSampleGrabber *m_grabber;
};

class HSampleGrabber :
        public ISampleGrabberCB
{
    // ISampleGrabberCB interface
public:
    HSampleGrabber(){}
    HRESULT SampleCB(double SampleTime, IMediaSample *pSample){
        cout << "Sample time: " << std::to_string(SampleTime) << endl;
        return 1;
    }
    HRESULT BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){
        cout << "Buffer Length: " << std::to_string(BufferLen) << endl;
        return 1;
    }

    // IUnknown interface
public:
    HRESULT QueryInterface(const IID &riid, void **ppvObject){cout << "QueryInterface"; return 1;}
    ULONG AddRef(){cout << "Added REF" << endl; return 1;}
    ULONG Release(){cout << "Released" << endl; return 1;}
};


#endif // SAMPLEGRABBERCALLBACKCLASS_H
