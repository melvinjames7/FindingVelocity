//
//
//  Created by Melvin James 
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

Mat makeBinary(Mat image)
{
    Mat d_image = Mat::zeros( image.size(), image.type() );  //for new brightness adjusted image
    for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
    {
        float r = (image.at<Vec3b>(y,x)[2]);
        float g = (image.at<Vec3b>(y,x)[1]);
        float b = (image.at<Vec3b>(y,x)[0]);
        
        if(r-g >20 || b-g>20){
            r = 255; g=255; b=255;      //white
        }
        else{
            r = 0; g = 0; b = 0;        //black
        }
        
        d_image.at<Vec3b>(y,x)[0] =
        saturate_cast<uchar>( b);
        d_image.at<Vec3b>(y,x)[1] =
        saturate_cast<uchar>( g );
        d_image.at<Vec3b>(y,x)[2] =
        saturate_cast<uchar>( r);
        
    }
    }
    
    medianBlur(d_image, d_image, 17);                   //to filter noise
    
    int i=0,X1=0,Y1=0, X2=0,Y2=0,width1 =0,width2=0;
    for( int y = 0; y < d_image.rows; y++ )
    {
        for( int x = 0; x < d_image.cols/2; x++ )
        {
            float r = (d_image.at<Vec3b>(y,x)[2]);
            
            if(r==255)
            {  i++;}
            else
            {
                if(i>width1)
                {
                    X1=x;
                    Y1=y;
                    width1 = i;
                    
                    
                }i=0;
            }
        }i=0;
    }X1-=width1/2;
    
    rectangle(d_image, Point(X1-width1/2,Y1-width1/2), Point(X1+width1/2,Y1+width1/2), Scalar(0,0,255));
    
    for( int y = 0; y < d_image.rows; y++ )
    {
        for( int x = d_image.cols/2; x < d_image.cols; x++ )
        {
            float r = (d_image.at<Vec3b>(y,x)[2]);
            
            if(r==255)
            {  i++;}
            else
            {
                if(i>width2)
                {
                    X2=x;
                    Y2=y;
                    width2 = i;
                    
                }i=0;
            }
        }i=0;
    }X2-=width2/2;
    
    if(flag == 0){
        Purple1.x=X1;Purple1.y = Y1;
        Orange1=Point(X2,Y2);
        flag++;
        Pradius = width1;Oradius = width2;
    }
    else{
        Purple2=Point(X2,Y2);
        Orange2=Point(X1,Y1);
    }
    
    rectangle(d_image, Point(X2-width2/2,Y2-width2/2), Point(X2+width2/2,Y2+width2/2), Scalar(0,0,255));
    return d_image;
}

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
