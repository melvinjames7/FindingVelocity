//
//
//  Created by Melvin James on 3/8/17.
//  Copyright © 2017 Melvin James. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <algorithm>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Point Purple1,Purple2, Orange1, Orange2;
int flag = 0,Pradius=0,Oradius=0;


int main(int argc, const char * argv[]) {
    string CimageName1("/Users/melvinjames/Desktop/color_92331.png");
    string CimageName2("/Users/melvinjames/Desktop/color_94764.png");
    string DimageName1("/Users/melvinjames/Desktop/depth_92331d.png");
    string DimageName2("/Users/melvinjames/Desktop/depth_94764d.png");
    
    Mat color1,color2,depth1,depth2;
    
    color1 = imread(CimageName1.c_str(), IMREAD_COLOR); // Read the file
    if( color1.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
//    namedWindow( "Display color1", WINDOW_AUTOSIZE ); // Create a window for display.
//    imshow( "Display color1", color1 );
    
    color2 = imread(CimageName2.c_str(), IMREAD_COLOR); // Read the file
    if( color2.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
//    namedWindow( "Display color2", WINDOW_AUTOSIZE ); // Create a window for display.
//    imshow( "Display color2", color2 );
    
    depth1 = imread(DimageName1.c_str(), CV_LOAD_IMAGE_UNCHANGED); // Read the file
    if( depth1.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
//    namedWindow( "Display depth1", WINDOW_AUTOSIZE ); // Create a window for display.
//    imshow( "Display depth1", depth1 );
    
    depth2 = imread(DimageName2.c_str(), CV_LOAD_IMAGE_UNCHANGED); // Read the file
    if( depth2.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
//    namedWindow( "Display depth2", WINDOW_AUTOSIZE ); // Create a window for display.
//    imshow( "Display depth2", depth2 );
    
    Mat coldep1 = colorize(depth1,color1);
    
    imshow("Colorized Depth Image 1", coldep1);
    
    Mat coldep2 = colorize(depth2,color2);
    imshow("Colorized Depth Image 2", coldep2);
    
    Mat bin1 = makeBinary(coldep1);
    //imshow("Bin 1", bin1);
    
    Mat bin2 = makeBinary(coldep2);
    //imshow("Bin 2", bin2);
    
    rectangle(coldep1, Point(Purple1.x-Pradius/2,Purple1.y-Pradius/2), Point(Purple1.x+Pradius/2,Purple1.y+Pradius/2), Scalar(0,0,255));
    rectangle(coldep1, Point(Orange1.x-Oradius/2,Orange1.y-Oradius/2), Point(Orange1.x+Oradius/2,Orange1.y+Oradius/2), Scalar(0,0,255));
    imshow("Colorized Depth Image with rectangle 1", coldep1);
    
    rectangle(coldep2, Point(Purple2.x-Pradius/2,Purple2.y-Pradius/2), Point(Purple2.x+Pradius/2,Purple2.y+Pradius/2), Scalar(0,0,255));
    rectangle(coldep2, Point(Orange2.x-Oradius/2,Orange2.y-Oradius/2), Point(Orange2.x+Oradius/2,Orange2.y+Oradius/2), Scalar(0,0,255));
    imshow("Colorized Depth Image with rectangle 2", coldep2);
    
    RelativeVelocity(coldep1,coldep2);
    
    waitKey(0);                         //to view output
    return 0;
}
