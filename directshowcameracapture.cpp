#include "directshowcameracapture.h"

DirectShowCameraCapture::DirectShowCameraCapture()
{

}

void DirectShowCameraCapture::initialize(CaptureMode pMode)
{
    this->mMode = pMode;

    CoInitialize(nullptr);
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&mCreateDevEnum);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&mGraph);
    CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&mBuilder);

    mCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &mEnumMoniker, 0);

    if (mEnumMoniker->Next(1, &mMoniker, &nFetched) == S_OK){
        mMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mVideoCaptureFilter);
        mMoniker->Release();
    }

    // Add the Device Filter to the Graph
    mGraph->AddFilter(mVideoCaptureFilter,L"Video Capture");

    //Creates Grabber Filter and adds it to the Filter Graph
    //Once connected, Grabber Filter will capture still images
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mGrabberFilter));
    if(FAILED(hr)) std::cout << "Could not create Grabber filter" << std::endl;

    hr = mGraph->AddFilter(mGrabberFilter, L"Sample Grabber");
    if(FAILED(hr)) std::cout << "Could not add Grabber Filter into GraphBuilder" << std::endl;

    hr = mGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&mGrabber);
    if(FAILED(hr)) std::cout << "Could not retrieve ISampleGrabber from Grabber Filter, sorry..." << std::endl;



    if(mMode != CaptureMode::ONLY_DISPLAY){
        AM_MEDIA_TYPE tMediaType;
        ZeroMemory(&tMediaType,sizeof(AM_MEDIA_TYPE));
        tMediaType.majortype = MEDIATYPE_Video;
        tMediaType.subtype = MEDIASUBTYPE_RGB24;

        mSampleGrabberCallbackClass = new SampleGrabberCallback;
        hr = mGrabber->SetCallback(mSampleGrabberCallbackClass,(int)CallbackMethod::BufferCB);
        if(FAILED(hr)) std::cout << "Could not Grabber callback" << std::endl;

        hr = mGrabber->SetMediaType(&tMediaType);
        if(FAILED(hr)) std::cout << "Could not set media type, sorry..." << std::endl;

        hr = mGrabber->SetOneShot(false);
        if(FAILED(hr)) std::cout << "Could not set one shot, sorry..." << std::endl;

        hr = mGrabber->SetBufferSamples(true);
        if(FAILED(hr)) std::cout << "Could not set buffer samples, sorry..." << std::endl;
    }

    //Channels the camera output to GrabberFilter
    hr = mBuilder->SetFiltergraph(mGraph); //Bunu yapınca video aciliyor. Buffer geliyor, acmayınca video'da buffer'da gelmiyor.
    if(FAILED(hr)) std::cout << "Set Filter Graph has failed!!" << std::endl;

    if(mMode == CaptureMode::ONLY_BUFFER)
        hr = mBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, mVideoCaptureFilter, NULL, mGrabberFilter);
    else{
        hr = mBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, mVideoCaptureFilter, mGrabberFilter, NULL);
    }
    if(FAILED(hr)) std::cout << "Render stream of video capture filter and grabber filter has failed!" << std::endl;

    // Attach the graph control
    hr = mGraph->QueryInterface(IID_IMediaControl, (void **)&mMediaControl);
    // Attach the graph events
    hr = mGraph->QueryInterface(IID_IMediaEvent, (void **)&mMediaEventEx);
    // Attach the graph window
    hr = mGraph->QueryInterface(IID_IVideoWindow, (void**)&mVideoWindow);

    mVideoCaptureFilter->Release();

    if(mVideoWindow)
        mVideoWindow->SetWindowPosition(0,0,640,480);

    if(mMediaControl != nullptr)
        mMediaControl->Run();
}
