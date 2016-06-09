#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace cv;
using namespace std;

String imgFileFolder = "./images/";

int main( int argc, char** argv )
{



    Mat image = imread(imgFileFolder+"01"+".jpeg", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
      {

       	  cout <<  "Could not open or find the image" << std::endl ;
          return -1;
      }



	       namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
	       imshow( "Display window", image );                   // Show our image inside it.

	    waitKey(0);
}
