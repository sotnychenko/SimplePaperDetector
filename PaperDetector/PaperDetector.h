#ifndef PAPERDETECTOR_H					// avoid repeated expansion
#define PAPERDETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <iostream>

using namespace cv;

class PaperDetector {
    public:

	PaperDetector(float AspectRatio =0.6, float MinArea=1000.0): aspectRatio(AspectRatio), minArea(MinArea)
    {

    }

    void MarkDetectedPapers(Mat &image);

    vector<RotatedRect> getListOfPapers()
     {
         return detectedPapers;
     }

    virtual ~PaperDetector(){};

    private:

    float aspectRatio;
    float minArea;

	vector<RotatedRect> detectedPapers;

    void detectPapers(const Mat &image);

    void drawPapers(Mat &image);

    float areaOfWhitePixels( const Mat img, RotatedRect box);

    Mat processTheImage(Mat img);

};
#endif /* PAPERDETECTOR_H */
