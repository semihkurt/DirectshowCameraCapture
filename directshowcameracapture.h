#ifndef DIRECTSHOWCAMERACAPTURE_H
#define DIRECTSHOWCAMERACAPTURE_H
#include "qedit.h"
#include "samplegrabbercallback.h"

#include <dshow.h>

enum class CaptureMode{
    ONLY_DISPLAY,
    ONLY_BUFFER,
    BOTH
};

enum class CallbackMethod{
    SampleCB,
    BufferCB
};

class DirectShowCameraCapture
{
public:
    DirectShowCameraCapture();

    void initialize(CaptureMode pMode);
private:
    HRESULT hr;
    ICreateDevEnum* mCreateDevEnum = nullptr;
    IEnumMoniker* mEnumMoniker = nullptr;
    IGraphBuilder* mGraph = nullptr;
    IMoniker* mMoniker = nullptr;
    IBaseFilter* mVideoCaptureFilter = nullptr;
    IBaseFilter *mGrabberFilter = nullptr;
    IBaseFilter *mNullRenderer = nullptr;
    IVideoWindow* mVideoWindow = nullptr;
    IMediaControl* mMediaControl = nullptr;
    IMediaEvent* mMediaEvent = nullptr;
    IMediaEventEx* mMediaEventEx = nullptr;
    ICaptureGraphBuilder2* mBuilder = nullptr;

    ULONG nFetched = 0;

    ISampleGrabber* mGrabber = nullptr;
    SampleGrabberCallback* mSampleGrabberCallbackClass = nullptr;

    CaptureMode mMode = CaptureMode::ONLY_DISPLAY;
};

#endif // DIRECTSHOWCAMERACAPTURE_H
