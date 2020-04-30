#ifndef DIRECTSHOWSAMPLEGRABBER_H
#define DIRECTSHOWSAMPLEGRABBER_H
#include <dshow.h>
#include <qedit.h>
class SampleGrabberCallbackClass;

class DirectShowSampleGrabber
{
public:
    // 0 = ISampleGrabberCB::SampleCB, 1 = ISampleGrabberCB::BufferCB
    enum class CallbackMethod : long
    {
        SampleCB,
        BufferCB
    };

private:
    ISampleGrabber* mSampleGrabber;

    CallbackMethod mCallbackType = CallbackMethod::BufferCB;
    SampleGrabberCallbackClass* mSampleGrabberCB;
public:
    DirectShowSampleGrabber();
    ~DirectShowSampleGrabber();

    void create(ISampleGrabber* pSampleGrabber);
    void create();

    IBaseFilter* mFilter;
    void stop();
    bool isValid() const;
    bool getConnectedMediaType(AM_MEDIA_TYPE *mediaType);
    bool setMediaType(const AM_MEDIA_TYPE *mediaType);

    IBaseFilter *getFilter();
    ISampleGrabber *getSampleGrabber();
    void start(DirectShowSampleGrabber::CallbackMethod method, bool oneShot, bool bufferSamples);
};



#endif // DIRECTSHOWSAMPLEGRABBER_H
