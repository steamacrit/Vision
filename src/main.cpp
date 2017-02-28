#include <opencv2/core/core.hpp>

#include "frame_grabber.h"
#include "frame_processor.h"
#include "Queue.h"

#include <string>
#include <thread>
#include <chrono>

int main(int argc, char** argv)
{
    bool show_camera = false;
    bool show_track = false;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            std::string param(argv[i]);
            if (param.compare("-show-camera") == 0)
                show_camera = true;
            if (param.compare("-show-track") == 0)
                show_track = true;
        }
    }

    Queue<cv::Mat> frame_queue(5);

    FrameGrabber grabber(&frame_queue, show_camera);
    grabber.Start();

    int width = 0;
    int height = 0;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));

    grabber.GetResolution(width, height);

    FrameProcessor processor(&frame_queue, width, height, show_track);
    processor.Start();

    while (1);

	return 0;
}



