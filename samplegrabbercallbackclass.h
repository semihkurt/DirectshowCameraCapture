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
    SampleGrabberCallbackClass();
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

#endif // SAMPLEGRABBERCALLBACKCLASS_H
