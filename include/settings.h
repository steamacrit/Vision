#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

//#define LOGITECH_C525
//#define LOGITECH_C525_HALFED
//#define GENIUS_120_DEGREE
#define GENIUS_120_DEGREE_HALFED

#ifdef LOGITECH_C525
    #define FRAME_WIDTH             1280
    #define FRAME_HEIGHT            720
    #define FOV                     69.0
#endif
#ifdef LOGITECH_C525_HALFED
    #define FRAME_WIDTH             640
    #define FRAME_HEIGHT            360
    #define FOV                     69.0
#endif
#ifdef GENIUS_120_DEGREE
    #define FRAME_WIDTH             1920
    #define FRAME_HEIGHT            1080
    #define FOV                     120.0
#endif
#ifdef GENIUS_120_DEGREE_HALFED
    #define FRAME_HEIGHT            960
    #define HEIGHT                  540
    #define FOV                     120.0
#endif

#define CAMERA_DEVICE       0

// TEMP
#define LOW_H               47.0
#define HIGH_H              68.0

#define LOW_S               44.0
#define HIGH_S              255.0

#define LOW_L               28.0
#define HIGH_L              255.0


/*
#define LOW_H               77.6978417266187
#define HIGH_H              91.37521222410867

#define LOW_S               171.98741007194246
#define HIGH_S              255.0

#define LOW_L               43.57014388489208
#define HIGH_L              255.0
*/

#define BRIGHTNESS          -64
#define EXPOSURE            4

#define FILTER_CONTOURS_MIN_AREA        125.0
#define FILTER_CONTOURS_MIN_PERIMETER   0.0
#define FILTER_CONTOURS_MIN_WIDTH       0.0
#define FILTER_CONTOURS_MAX_WIDTH       1000.0
#define FILTER_CONTOURS_MIN_HEIGHT      0.0
#define FILTER_CONTOURS_MAX_HEIGHT      1000.0
#define FILTER_CONTOURS_SOLIDARITY_LOW  0.0
#define FILTER_CONTOURS_SOLIDARITY_HIGH 100.0
#define FILTER_CONTOURS_MAX_VERTICES    1000000.0
#define FILTER_CONTOURS_MIN_VERTICES    0.0
#define FILTER_CONTOURS_MIN_RATIO       0.0
#define FILTER_CONTOURS_MAX_RATIO       1000.0

#define MIN_TARGET_RADIUS   10.0f

#endif // SETTINGS_H_INCLUDED
