#ifndef FRAMEGRABBER_H_INCLUDED
#define FRAMEGRABBER_H_INCLUDED

#include "Queue.h"
#include <opencv2/core/core.hpp>
#include <thread>
#include <atomic>

class FrameGrabber
{
    public:
        FrameGrabber(Queue<cv::Mat> * frame_queue, bool show_camera = false);
        virtual ~FrameGrabber();

        void GetResolution(int & width, int & height);

        void Start();
        void Stop();

        static void Grab(FrameGrabber * p_grabber);

    protected:
        std::atomic<bool> _exit;
        std::thread * _p_grabber_thread;
        Queue<cv::Mat> * _p_frame_queue;

        bool _show_camera;
        int _width;
        int _height;
    private:
};

#endif // FRAMEGRABBER_H_INCLUDED
