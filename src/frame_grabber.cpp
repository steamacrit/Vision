#include "frame_grabber.h"
#include "settings.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

FrameGrabber::FrameGrabber(Queue<cv::Mat> * frame_queue, bool show_camera)
    : _p_grabber_thread(0)
    , _p_frame_queue(frame_queue)
    , _exit(false)
    , _show_camera(show_camera)
    , _width(FRAME_WIDTH)
    , _height(FRAME_HEIGHT)
{
    if (_p_frame_queue == 0)
        throw("FrameGrabber::FrameGrabber - Bad pointer (Queue<cv::Mat> *)");
}

FrameGrabber::~FrameGrabber()
{
    //dtor
}

void FrameGrabber::GetResolution(int & width, int & height)
{
    width = _width;
    height = _height;
}

void FrameGrabber::Start()
{
    if (_p_grabber_thread != 0)
        Stop();

    _exit = false;
    _p_grabber_thread = new std::thread(Grab, this);
}

void FrameGrabber::Stop()
{
    _exit = true;
    if (_p_grabber_thread->joinable())
        _p_grabber_thread->join();

    delete _p_grabber_thread;
    _p_grabber_thread = 0;
}

void FrameGrabber::Grab(FrameGrabber * p_grabber)
{
    if (p_grabber == 0)
        return;

    bool show_camera = p_grabber->_show_camera;

    cv::VideoCapture cap(CAMERA_DEVICE); //capture the video from webcam


    if (!cap.isOpened())  // if not success, exit program
    {
 	std::cout << "Cannot open the web cam" << std::endl;
	return;
    }


    cap.set(CV_CAP_PROP_FRAME_WIDTH, p_grabber->_width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, p_grabber->_height);
    //	cap.set(CV_CAP_PROP_EXPOSURE, -8);
    cap.set(CV_CAP_PROP_BRIGHTNESS, BRIGHTNESS);

    cv::Size frame_size(p_grabber->_width, p_grabber->_height);

    while (p_grabber->_exit == false)
    {
        cv::Mat frame;
        cap.read(frame);
        cv::resize(frame, frame, frame_size);

        if (show_camera)
        {
            cv::imshow("Camera", frame);
            cv::waitKey(1);
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS);
        p_grabber->_p_frame_queue->push(frame);
    }
}
