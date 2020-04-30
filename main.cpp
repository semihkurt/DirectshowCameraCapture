#include "atlbase.h"
#include <iostream>
#include <qedit.h>
#include <directshowsamplegrabber.h>
#include <string>
using namespace std;

class HSampleGrabber;

HRESULT hr;
ICreateDevEnum* tCreateDevEnum = nullptr;
IEnumMoniker* tEnumMoniker = nullptr;
IGraphBuilder* tGraphBuilder = nullptr;
IMoniker* tMoniker = nullptr;
IBaseFilter* mVideoCapture = nullptr;
IBaseFilter *mGrabberFilter = nullptr;
IBaseFilter *mNullRenderer = nullptr;
IVideoWindow* tVideoWindow = nullptr;
IMediaControl* tMediaControl = nullptr;
IMediaEventEx* tMediaEventEx = nullptr;
ICaptureGraphBuilder2* tCaptureGraphBuilder2 = nullptr;

ULONG nFetched = 0;

IBaseFilter* tGrabFilter = nullptr;
IBaseFilter* tBaseFilter = nullptr;
IBaseFilter* tRendererFilter = nullptr;
ISampleGrabber* tSampleGrabber = nullptr;
HSampleGrabber* tSampleGrabberCallbackClass = nullptr;

DirectShowSampleGrabber* mDirectShowSampleGrabber = nullptr;

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


void openWindow(){

    CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&tCaptureGraphBuilder2);

    IBaseFilter* tRenderer = nullptr;

    tCaptureGraphBuilder2->SetFiltergraph(tGraphBuilder);

    tGraphBuilder->AddFilter(mVideoCapture,L"Video Capture");
    tCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, mVideoCapture, NULL, NULL );


    //    if(mDirectShowSampleGrabber == nullptr){
    //        tGraphBuilder->AddFilter(tFilter,L"Video Capture");
    //        tCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, tFilter, NULL, NULL );
    //    }else{
    //        tGraphBuilder->AddFilter(mDirectShowSampleGrabber->mFilter,L"Video Capture");
    //        tCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, mDirectShowSampleGrabber->mFilter, NULL, NULL);
    //    }

    ////    if(hWnd != NULL) {
    ////        hr = m_pVideoWindow->put_Owner((OAHWND)hWnd);
    ////        hr = m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
    ////        hr = m_pVideoWindow->put_MessageDrain((OAHWND)hWnd);
    ////        long Width, Height;
    ////        RECT rect;
    ////        GetClientRect(hWnd, &rect);
    ////        Width = rect.right;
    ////        Height = rect.bottom;
    ////        SetWindowSize(Width, Height);
    ////    }

    if(tVideoWindow)
        tVideoWindow->SetWindowPosition(0,0,640,480);

    // Obtain interfaces for media control and Video Window
    hr = tGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&tMediaControl);
    hr = tGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&tVideoWindow);
    //    hr = tGraphBuilder->QueryInterface(IID_IMediaEventEx, (LPVOID *) &tMediaEventEx);


    if(tMediaControl != nullptr)
        tMediaControl->Run();

    //    IMediaSample* tMediaSample = nullptr;
    //    hr = tMediaControl->QueryInterface(IID_IMediaSample,(void**)& tMediaSample);
    //    if(FAILED(hr))
    //        cout<<"\nERROR: Get Media Sample" << endl;

    //    BYTE* tData = new BYTE(640 * 480 * 3);
    //    tMediaSample->GetPointer(&tData);

}

void useClass(){
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&tCreateDevEnum);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&tGraphBuilder);

    tCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &tEnumMoniker, 0);

    mDirectShowSampleGrabber = new DirectShowSampleGrabber;
    mDirectShowSampleGrabber->create();

    if (tEnumMoniker->Next(1, &tMoniker, &nFetched) == S_OK){
        tMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mDirectShowSampleGrabber->mFilter);
        //        tMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&tFilter);
        tMoniker->Release();
    }

    CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&tCaptureGraphBuilder2);
    tCaptureGraphBuilder2->SetFiltergraph(tGraphBuilder);
    tGraphBuilder->AddFilter(mDirectShowSampleGrabber->mFilter,L"Video Capture");
    tCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, mDirectShowSampleGrabber->mFilter, NULL, NULL );
    //    tGraphBuilder->AddFilter(tFilter,L"Video Capture");
    //    tCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, tFilter, NULL, NULL );

    if(tVideoWindow)
        tVideoWindow->SetWindowPosition(0,0,640,480);

    // Obtain interfaces for media control and Video Window
    hr = tGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&tMediaControl);
    hr = tGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&tVideoWindow);
    //    hr = tGraphBuilder->QueryInterface(IID_IMediaEventEx, (LPVOID *) &tMediaEventEx);


    if(tMediaControl != nullptr)
        tMediaControl->Run();

    if(mDirectShowSampleGrabber != nullptr){

        if(mDirectShowSampleGrabber->getSampleGrabber() == nullptr){
            hr = tGraphBuilder->QueryInterface(IID_ISampleGrabber, (void**)&tSampleGrabber);
            if(tSampleGrabber != nullptr){
                cout << "Sample grabber successfuly got form graph builder" << endl;
                mDirectShowSampleGrabber->create(tSampleGrabber);
            }else{
                cout << "Error couldnt get sample grabber from graph builder" << endl;
            }
        }
        mDirectShowSampleGrabber->start(DirectShowSampleGrabber::CallbackMethod::BufferCB,true,true);
    }
}

void useDefault(){
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&tCreateDevEnum);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&tGraphBuilder);

    tCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &tEnumMoniker, 0);

    if (tEnumMoniker->Next(1, &tMoniker, &nFetched) == S_OK){
        tMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mVideoCapture);
        tMoniker->Release();
    }

    CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&tCaptureGraphBuilder2);

    tCaptureGraphBuilder2->SetFiltergraph(tGraphBuilder);

    tGraphBuilder->AddFilter(mVideoCapture,L"Video Capture");
    tCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, mVideoCapture, NULL, NULL );

    if(tVideoWindow)
        tVideoWindow->SetWindowPosition(0,0,640,480);

    // Obtain interfaces for media control and Video Window
    hr = tGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&tMediaControl);
    hr = tGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&tVideoWindow);
    //    hr = tGraphBuilder->QueryInterface(IID_IMediaEventEx, (LPVOID *) &tMediaEventEx);

    if(tMediaControl != nullptr)
        tMediaControl->Run();

    tSampleGrabberCallbackClass = new HSampleGrabber;
    hr = tGraphBuilder->QueryInterface(IID_ISampleGrabber, (void**)&tSampleGrabber);
    if(tSampleGrabber != nullptr && tSampleGrabberCallbackClass != nullptr){
        hr = tSampleGrabber->SetCallback(tSampleGrabberCallbackClass, 0);

        tSampleGrabber->SetOneShot(true);
        tSampleGrabber->SetBufferSamples(true);
        if(FAILED(hr))
            cout<<"\nERROR: SetCallback" << endl;
        long cbBuffer = 0;
        hr = tSampleGrabber->GetCurrentBuffer(&cbBuffer, NULL);
        if(hr == VFW_E_WRONG_STATE)
            cout<<"\nERROR: GetCurrentBuffer1" << endl;

        char *pBuffer = new char[cbBuffer];
        hr = tSampleGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);
        if(hr == VFW_E_WRONG_STATE)
            cout<<"\nERROR: GetCurrentBuffer2" << endl;
    }else{
        cout << "Sample grabber is NULL! Couldn't get it from graph builder" << endl;
    }
}

int main(int argc, char *argv[])
{
    //    MainWindow w;
    //    w.show();

    useDefault();

    cout << "EXEC" << endl;

    int a;
    a = getchar();
    return 0;
}

int store_bmp (char *pBMP)
{
    LPCOLESTR oleStr;   //pointer to a constant OLESTR
    USES_CONVERSION;   //in header file ATLBASE.H
    //    oleStr = T2OLE (pBMP);  //T2OLE //A2COLE  //OLESTR //CComBSTR

    //    HRESULT hr = tCaptureGraphBuilder2->SetOutputFileName(&MEDIASUBTYPE_Avi, oleStr, &pMux, &pFileSinkFilter);
    //    if(hr != S_OK)
    //    {
    //        printf("nERROR: could not set output file.\n\n");
    //        return(0);
    //    }

    //    //sets the name of the file into which media samples will be written
    //    hr = pFileSinkFilter->SetFileName(oleStr, NULL);
    //    if(hr != S_OK)
    //    {
    //        printf("ERROR: could not set the file.\n\n");
    //        return(0);
    //    }

    hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&tRendererFilter);
    if(FAILED(hr)) cout<<"\nERROR: CoCreateInstance";

    hr = tGraphBuilder->AddFilter(tRendererFilter, NULL);
    if(FAILED(hr)) cout<<"\nERROR: AddFilter";

    //    //connects an output pin on a source filter to a sink filter
    //    hr = tCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, tBaseFilter, NULL, pMux);  //pMux
    //    if(hr != S_OK)
    //    {
    //        printf("ERROR: could not render VIDEO.\n\n");
    //        return(0);
    //    }

    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&tGrabFilter);
    if(FAILED(hr)) cout<<"\nERROR: CoCreateInstance";

    hr = tGraphBuilder->AddFilter(tGrabFilter, NULL);
    if(FAILED(hr)) cout<<"\nERROR: AddFilter";

    hr = tGrabFilter->QueryInterface(IID_ISampleGrabber, (void**)&tSampleGrabber);
    if(FAILED(hr)) cout<<"\nERROR: QueryInterface";

    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;

    hr = tSampleGrabber->SetMediaType(&mt);
    if(FAILED(hr)) cout<<"\nERROR: SetMediaType";

    hr = tSampleGrabber->SetOneShot(TRUE);
    if(FAILED(hr)) cout<<"\nERROR: SetOneShot";

    hr = tSampleGrabber->SetBufferSamples(TRUE);
    if(FAILED(hr)) cout<<"\nERROR: SetBufferSamples";

    hr = tMediaControl->Run();
    if(FAILED(hr)) cout<<"\nERROR: Run";

    hr = tMediaControl->Pause();
    if(FAILED(hr)) cout<<"\nERROR: Pause";

    tSampleGrabberCallbackClass = new HSampleGrabber;

    hr = tSampleGrabber->SetCallback(tSampleGrabberCallbackClass, 1);
    if(FAILED(hr)) cout<<"\nERROR: SetCallback";

    long cbBuffer = 0;
    hr = tSampleGrabber->GetCurrentBuffer(&cbBuffer, NULL);
    if(hr == VFW_E_WRONG_STATE) cout<<"\nERROR: GetCurrentBuffer1";

    char *pBuffer = new char[cbBuffer];
    hr = tSampleGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);
    if(hr == VFW_E_WRONG_STATE) cout<<"\nERROR: GetCurrentBuffer2";

    printf("The image is been captured in %s file\n\n", pBMP);
    return(1);
}

