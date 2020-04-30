#include "atlbase.h"
#include <iostream>
#include "directshowcameracapture.h"
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    DirectShowCameraCapture* mDirectshow = new DirectShowCameraCapture;
    mDirectshow->initialize(CaptureMode::ONLY_BUFFER);

    cout << "EXEC" << endl;

    int a;
    a = getchar();
    return 0;
}
