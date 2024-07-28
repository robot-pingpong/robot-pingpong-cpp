#include "capture.h"

int main()
{
    Capture c(0);
    c.setGlobalMask();
    cv::Mat screen;

    while (true)
    {
        c.captureFrame();
        c.render(screen);
        cv::imshow("screen", screen);
        if (cv::waitKey(1) == 27) break;
    }
    return 0;
}
