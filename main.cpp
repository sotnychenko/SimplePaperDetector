#include <fstream>
#include <vector>

#include "./PaperDetector/PaperDetector.h"

using namespace cv;
using namespace std;


string findFileName (const string& str) {

  unsigned found = str.find_last_of("/\\");

  return str.substr(found+1);
}

int main( int argc, char** argv )
{
	 // Check the number of parameters
	    if (argc < 2) {
	        // Tell the user how to run the program
	        std::cerr << "Usage: " << argv[0] << " enter the file of the image to process" << std::endl;

	        return 1;
	    }
	std::string fileName = argv[1];
    Mat image = imread(fileName, CV_LOAD_IMAGE_COLOR);   // Read the file

    if(!image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    fileName=findFileName(fileName);

    PaperDetector paperDetector;
    paperDetector.MarkDetectedPapers(image);

    imwrite( "processed_"+fileName,image);


}
