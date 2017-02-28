#ifndef FRAMEPROCESSOR_H_INCLUDED
#define FRAMEPROCESSOR_H_INCLUDED

#include "Queue.h"
#include <opencv2/core/core.hpp>
#include <thread>
#include <atomic>

class FrameProcessor
{
    public:
        FrameProcessor(Queue<cv::Mat> * frame_queue, int width, int height, bool show_track = false);
        virtual ~FrameProcessor();

        void Start();
        void Stop();

        static void Process(FrameProcessor * p_processor);

    protected:
        std::atomic<bool> _exit;
        std::thread * _p_processor_thread;
        Queue<cv::Mat> * _p_frame_queue;

        bool _show_track;
        int _width;
        int _height;
    private:
};

#endif // FRAMEPROCESSOR_H_INCLUDED

