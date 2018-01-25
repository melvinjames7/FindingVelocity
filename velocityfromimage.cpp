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

Point get3Dcordinates(int x, int y, int z)
{
    
    Mat inverse=(Mat_<double>(3,3)<<
                 0.0027289,0,-0,
                 -7.1662e-06,0.0027186,-0,
                 -0.71173,-0.56546,1);
    
    Mat depth, m;
    
    depth=(Mat_<double>(1, 3)<<y, x, 1);
    
    m = depth * inverse;
    m = m.mul(z);
    
    double x1 = m.at<double>(0,0);
    double y1 = m.at<double>(0,1);
    
    return Point(x1,y1);
}

void RelativeVelocity(Mat image1, Mat image2)
{
    Point Purple1_3D,Purple2_3D,Orange1_3D,Orange2_3D;
    float PurpleV,RedV, z1,z2;
    
    
    z1 = image1.at<ushort>(Purple1.y,Purple1.x);
    Purple1_3D = get3Dcordinates(Purple1.x,Purple1.y,z1);
    
    z2 = image2.at<ushort>(Purple2.y,Purple2.x);
    Purple2_3D = get3Dcordinates(Purple2.x,Purple2.y,z2);
    
    PurpleV = sqrt(powf((Purple2_3D.x - Purple1_3D.x),2)+powf((Purple2_3D.y - Purple1_3D.y),2)+
                   powf((z2- z1),2));
    
    
    z1 = image1.at<ushort>(Orange1.y,Orange1.x);
    Orange1_3D = get3Dcordinates(Orange1.x,Orange1.y,z1);
    
    
    z2 = image2.at<ushort>(Orange2.y,Orange2.x);
    
    
    Orange2_3D = get3Dcordinates(Orange2.x,Orange2.y,z2);
    
    
    RedV = sqrt(powf((Orange2_3D.x - Orange1_3D.x),2)+powf((Orange2_3D.y - Orange1_3D.y),2)+ powf((z2- z1),2));
    
    float mmPs =  ((PurpleV+RedV)/2433)*1000.0;
    cout<<"Relative Velocity = "<<mmPs<<"mm/s";
}

Mat colorize(Mat image1 , Mat color1)
{
    Mat image = image1.clone();//depth image
    Mat color = color1.clone();//corresponding color image
    
    Mat inverse=(Mat_<double>(3,3)<<
                 0.0027289,0,-0,
                 -7.1662e-06,0.0027186,-0,
                 -0.71173,-0.56546,1);
    Mat D2C=(Mat_<double>(4,4)<<
             0.99998, -0.0062464, 0.0013162, 0,
             0.0062361, 0.99997, 0.0046386, 0,
             -0.0013491, -0.0046356, 0.99999, 0,
             50.775, 11.994, -80.412, 1);
    Mat RGB=(Mat_<double>(3,3)<<
             1027, 0, 0,
             3.4052, 1029.9, 0,
             968, 536.54, 1);
    
    
    Mat depth;
    
    
    Mat new_image1 = Mat::zeros( image.size(), color.type());
    Mat m,m2,m3,m4;
    for( int x = 0; x < image.rows; x++ )
    {
        for( int y = 0; y < image.cols; y++ )
        {
            depth=(Mat_<double>(1, 3)<<y, x, 1);
            int z_coord = image.at<ushort>( x,y);
            if(z_coord==0)
                continue;
            m = depth * inverse;
            m = m.mul(z_coord);
            
            
            m2=(Mat_<double>(1, 4)<<
                m.at<double>(0, 0),m.at<double>(0,1),m.at<double>(0,2),1);
            m3 = m2 * D2C;
            
            m4 = (Mat_<double>(1, 3)<<
                  m3.at<double>(0,0)/m3.at<double>(0,2) , m3.at<double>(0,1)/m3.at<double>(0,2) , 1);
            m4*=RGB;
            
            double x1 = m4.at<double>(0,0);
            double y1 = m4.at<double>(0,1);
            if(x1>=0 && x1<color.cols && y1>=0 && y1< color.rows)
            {
                new_image1.at<Vec3b>(x,y)[0] = saturate_cast<uchar>( color.at<Vec3b>(y1,x1)[0]);
                new_image1.at<Vec3b>(x,y)[1] = saturate_cast<uchar>( color.at<Vec3b>(y1,x1)[1]);
                new_image1.at<Vec3b>(x,y)[2] = saturate_cast<uchar>( color.at<Vec3b>(y1,x1)[2]);
            }
        }
    }
    
    return new_image1;
}

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
