#include "frame_processor.h"
#include "settings.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

FrameProcessor::FrameProcessor(Queue<cv::Mat> * frame_queue, int width, int height, bool show_track)
    : _p_processor_thread(0)
    , _p_frame_queue(frame_queue)
    , _exit(false)
    , _show_track(show_track)
    , _width(width)
    , _height(height)
{
    if (_p_frame_queue == 0)
        throw("FrameProcessor::FrameProcessor - Bad pointer (Queue<cv::Mat> *)");
}

FrameProcessor::~FrameProcessor()
{
    //dtor
}

void FrameProcessor::Start()
{
    if (_p_processor_thread != 0)
        Stop();

    _exit = false;
    _p_processor_thread = new std::thread(Process, this);
}

void FrameProcessor::Stop()
{
    _exit = true;
    if (_p_processor_thread->joinable())
        _p_processor_thread->join();

    delete _p_processor_thread;
    _p_processor_thread = 0;
}

void FrameProcessor::Process(FrameProcessor * p_processor)
{
    if (p_processor == 0)
        return;

    int width = p_processor->_width;
    int height = p_processor->_height;
    double target_center = 0.0;
    double target_angle = 0.0;
    double half = (width -1) / 2;
    double angle = FOV / width;

    cv::Scalar lowerb = cv::Scalar(LOW_H, LOW_L, LOW_S);
    cv::Scalar upperb = cv::Scalar(HIGH_H, HIGH_L, HIGH_S);

    cv::Scalar contour_color = cv::Scalar(255, 255, 255);
    cv::Scalar bounds_color = cv::Scalar(0, 0, 255);

    while (p_processor->_exit == false)
    {
        bool show_track = p_processor->_show_track;
        bool active_track = false;

	cv::Mat thresholded_frame = p_processor->_p_frame_queue->pop();
	cv::inRange(thresholded_frame, lowerb, upperb, thresholded_frame); //Threshold the image

        // FIND CONTOURS
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(thresholded_frame, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // FILTER CONTOURS
        std::vector<cv::Point> hull;
        std::vector<std::vector<cv::Point> > filtered_contours;
        std::vector<cv::Point> approximation;
        int min_x = width, max_x = 0, min_y = height, max_y = 0;
        cv::Rect bounding_rect;

        for (std::vector<cv::Point> contour : contours)
        {

            cv::Rect bb = boundingRect(contour);
            if (bb.width < FILTER_CONTOURS_MIN_WIDTH || bb.width > FILTER_CONTOURS_MAX_WIDTH) continue;
            if (bb.height < FILTER_CONTOURS_MIN_HEIGHT || bb.height > FILTER_CONTOURS_MAX_HEIGHT) continue;
            double area = cv::contourArea(contour);
            if (area < FILTER_CONTOURS_MIN_AREA) continue;

            double perimeter = cv::arcLength(contour, true);

            if (perimeter < FILTER_CONTOURS_MIN_PERIMETER) continue;
            cv::convexHull(cv::Mat(contour, true), hull);
            double solid = 100 * area / cv::contourArea(hull);
            if (solid < FILTER_CONTOURS_SOLIDARITY_LOW || solid > FILTER_CONTOURS_SOLIDARITY_HIGH) continue;
            if (contour.size() < FILTER_CONTOURS_MIN_VERTICES || contour.size() > FILTER_CONTOURS_MAX_VERTICES)	continue;
            double ratio = (double) bb.width / (double) bb.height;
            if (ratio < FILTER_CONTOURS_MIN_RATIO || ratio > FILTER_CONTOURS_MAX_RATIO) continue;

            cv::approxPolyDP(contour, approximation, 0.05 * perimeter, true);
            if (approximation.size() == 4)
            {
                filtered_contours.push_back(contour);
                cv::Rect br = cv::boundingRect(contour);
                min_x = std::min(br.x, min_x);
                max_x = std::max(br.x + br.width, max_x);
                min_y = std::min(br.y, min_y);
                max_y = std::max(br.y + br.height, max_y);
            }
        }


        if (!filtered_contours.empty())
        {
            bounding_rect.x = min_x;
            bounding_rect.y = min_y;
            bounding_rect.width = max_x - min_x;
            bounding_rect.height = max_y - min_y;

            target_center = (bounding_rect.x + (bounding_rect.width / 2)) - half;
            target_angle = target_center * angle;
            active_track = true;
            std::cout << "Tracking Target: center( " << target_center << " )\t angle( " << target_angle << " )" << std::endl;
        }
        else
        {
            active_track = false;
            std::cout << "No Target" << std::endl;
        }



        if (show_track)
        {
            /// Draw contours
            cv::Mat drawing = cv::Mat::zeros(thresholded_frame.size(), CV_8UC3);
            for (int i = 0; i < filtered_contours.size(); i++)
                cv::drawContours(drawing, filtered_contours, i, contour_color, 2, 8, hierarchy, 0, cv::Point());

            cv::rectangle(drawing, bounding_rect, bounds_color, 2, 8, 0);


            imshow("Track", drawing);
            cv::waitKey(1);
        }
    }
}

