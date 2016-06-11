#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace cv;
using namespace std;

#define minArea 1000.0

typedef Point_<float> Point2f;
String imgFileFolder = "./images/";

class PaperDetector {
    public:

    void MarkDetectedPapers(Mat &image)
    {
        detectPapers(image);
        drawPapers(image);
    }

    vector<RotatedRect> getListOfPapers()
     {
         return detectedPapers;
     }

    private:

	vector<RotatedRect> detectedPapers;

    void detectPapers(const Mat &image)
    {
        Mat canny_output= processTheImage(image);
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        RNG rng(12345);
        cv::threshold(canny_output, canny_output, 200, 255, cv::THRESH_BINARY);
        Mat whiteImage;
        canny_output.copyTo( whiteImage);
        findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        for( size_t i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            RotatedRect box= minAreaRect(contours[i]);
            float hullArea=contourArea(contours[i]);
            float boxArea = box.size.height*box.size.width;
            //  Rect brect = box.boundingRect();
            // cv::rectangle(paddedImg, brect, Scalar(255,0,0),10);
            Point2f vertices[4];
            box.points(vertices);
            float w,h;

            if(box.size.height>box.size.width){ w=box.size.height;h = box.size.width;}
            else
            { w=box.size.width;h = box.size.height;}

            if(h/w>0.6&&hullArea>minArea){
                float whitePixelsArea = areaOfWhitePixels(whiteImage,box);
                if(whitePixelsArea/boxArea<0.8) continue;
                detectedPapers.push_back(box);
                // drawContours( image, hull, (int)i, color, 10, 8, vector<Vec4i>(), 0, Point() );
                // for (int i = 0; i < 4; i++)
                //             line(paddedImg, vertices[i], vertices[(i+1)%4], Scalar(0,255,0),10);
            }
        }
    }

    void drawPapers(Mat &image)
    {
        cout<<"Number of detected papers: "<<detectedPapers.size()<<endl;
        for(int i=0; i<detectedPapers.size();i++)
        circle( image, detectedPapers.at(i).center, 4, Scalar(0,0,255), 5, 8, 0 );
    }

    float areaOfWhitePixels( const Mat img, RotatedRect box)
    {
        float result = 0.0;
        Rect brect = box.boundingRect();

        Mat M, rotated, cropped;
        // get angle and size from the bounding box
        float angle = box.angle;
        Size boxSize = box.size;

        if (box.angle < -45.)
        {
            angle += 90.0;
            swap(boxSize.width, boxSize.height);
        }
        // get the rotation matrix
        M = getRotationMatrix2D(box.center, angle, 1.0);
        // perform the affine transformation
        warpAffine(img, rotated, M, img.size(), INTER_CUBIC);
        // crop the resulting image
        getRectSubPix(rotated, boxSize, box.center, cropped);
       // imwrite("test.jpg",cropped);
        for(int i=0; i<cropped.size().height; i++)
        	for(int j=0; j<cropped.size().width; j++)
        		{
        		if(cropped.at<uchar>(Point(i, j))==255.0) result+=1.0;
        		}


        return result;
    }

    Mat processTheImage(Mat img)
    {
        /// Reduce noise with a kernel 3x3
        Mat greyMat, edges;
        cv::cvtColor(img, greyMat, CV_BGR2GRAY);
        cv::threshold(greyMat, greyMat, 200, 255, cv::THRESH_BINARY);
        cv::equalizeHist(greyMat, greyMat);
        morphologyEx(greyMat, greyMat, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
        morphologyEx(greyMat, greyMat, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));

        return greyMat;
    }
};

int main( int argc, char** argv )
{
    Mat image = imread(imgFileFolder+"01"+".jpeg", CV_LOAD_IMAGE_COLOR);   // Read the file


    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    PaperDetector paperDetector;
    paperDetector.MarkDetectedPapers(image);

    namedWindow( "Display window", CV_WINDOW_NORMAL );// Create a window for display.
    imshow( "Display window", image );

    // Show our image inside it.

    waitKey(0);
}
