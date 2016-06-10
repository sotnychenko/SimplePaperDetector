#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>


#include <iostream>
#include <fstream>
#include <vector>

using namespace cv;
using namespace std;

#define lowThreshold  100
#define ratio  3
#define kernelSize  3

typedef Point_<float> Point2f;

String imgFileFolder = "./images/";

Mat processTheImage(Mat img,Mat &edges)
{
  /// Reduce noise with a kernel 3x3


  Mat greyMat, dst;
  cv::cvtColor(img, greyMat, CV_BGR2GRAY);
  cv::threshold(greyMat, greyMat, 200, 255, cv::THRESH_BINARY);

  cv::equalizeHist(greyMat, greyMat);

  Mat detected_edges;
  blur( greyMat, greyMat, Size(3,3) );

   morphologyEx(greyMat, greyMat, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
   morphologyEx(greyMat, greyMat, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
 return greyMat;
}



int main( int argc, char** argv )
{

	int thresh = 100;
	int max_thresh = 255;

    Mat image = imread(imgFileFolder+"01"+".jpeg", CV_LOAD_IMAGE_COLOR);   // Read the file
   // cv::resize(image, image, Size(image.size().height/4,image.size().width/4));
    if(! image.data )                              // Check for invalid input
      {

       	  cout <<  "Could not open or find the image" << std::endl ;
          return -1;
      }
  //  cout<<"asdas "<<abs(angleBetween( Point2f(0.0,1.0), Point2f(0.0,1.0))-CV_PI/2.0)<<endl;
    Mat edges;
    Mat canny_output= processTheImage(image,edges);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);

  //  Mat asd;
  //  canny_output.copyTo( asd);

    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

     vector<vector<Point> >hull( contours.size() );
       for( size_t i = 0; i < contours.size(); i++ )
          {   convexHull( Mat(contours[i]), hull[i], false ); }

       /// Get the moments
         vector<Moments> mu(contours.size() );
         for( int i = 0; i < contours.size(); i++ )
            { mu[i] = moments( contours[i], false ); }

         ///  Get the mass centers:
         vector<Point2f> mc( contours.size() );
         for( int i = 0; i < contours.size(); i++ )
            { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

       for( size_t i = 0; i< contours.size(); i++ )
          {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           // drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );

            RotatedRect box= minAreaRect(contours[i]);

            float hullArea=contourArea(contours[i]);
            float boxArea = box.size.height*box.size.width;


           // Rect brect = box.boundingRect();
           // cv::rectangle(image, brect, Scalar(255,0,0),10);

            Point2f vertices[4];
            box.points(vertices);


            float w,h;

           if(box.size.height>box.size.width){ w=box.size.height;h = box.size.width;}
                   			         else
                                             { w=box.size.width;h = box.size.height;}

            if(h/w>0.6&&hullArea>1000.0){
            	 circle( image, mc[i], 4, Scalar(0,0,255), 5, 8, 0 );

             // drawContours( image, hull, (int)i, color, 10, 8, vector<Vec4i>(), 0, Point() );
              for (int i = 0; i < 4; i++)
                          line(image, vertices[i], vertices[(i+1)%4], Scalar(0,255,0),10);
            }

           }

       namedWindow( "Display window", CV_WINDOW_NORMAL );// Create a window for display.
       	       imshow( "Display window", image );


	  // Show our image inside it.

	    waitKey(0);
}





