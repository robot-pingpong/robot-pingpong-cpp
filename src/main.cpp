#include "capture.h"

int main()
{
    Capture c(0);
    c.setGlobalMask();
    cv::Mat screen;

    auto prev = std::chrono::steady_clock::now();
    while (true)
    {
        c.captureFrame();
        c.render(screen);

        auto curr = std::chrono::steady_clock::now();
        const auto elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev).
            count());
        const auto fps = 1000.0 / elapsed;
        prev = curr;

        cv::putText(screen, "FPS: " + std::to_string(fps), cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));

        cv::imshow("screen", screen);
        if (cv::waitKey(1) == 27) break;
    }
    return 0;
}
