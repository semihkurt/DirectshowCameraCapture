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
    ICreateDevEnum* mCreateDevEnum;
    IEnumMoniker* mEnumMoniker;
    IGraphBuilder* mGraph;
    IMoniker* mMoniker;
    IBaseFilter* mVideoCaptureFilter;
    IBaseFilter *mGrabberFilter;
    IVideoWindow* mVideoWindow;
    IMediaControl* mMediaControl;
    IMediaEvent* mMediaEvent;
    ICaptureGraphBuilder2* mBuilder;
    ISampleGrabber* mGrabber;
    SampleGrabberCallback* mSampleGrabberCallbackClass;
    ULONG nFetched;
    CaptureMode mMode;
};

#endif // DIRECTSHOWCAMERACAPTURE_H
