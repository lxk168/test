#include "hcopencv.h"

#include "opencv/cv.h"
#include"opencv/cvaux.h"
#include"opencv/cvwimage.h"
#include"opencv/cxcore.h"
#include"opencv/cxmisc.h"
#include"opencv/highgui.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

#include "hcopencv.h"
int HCOpencvDraw(char *fromPath,char *toPath,char *text,int xmin,int ymin,int length,int wideth)
{

    CvFont font;
    IplImage *img=nullptr;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 1, 1, 1, 2, 8);
    img = cvLoadImage(fromPath,1);
    if (!img)
    {

        cvReleaseImage(&img);
        return -1;
    }


    CvScalar textColor = cvScalar(0,0, 255, 255);
    CvPoint textPos = cvPoint(xmin+30, ymin+30);
    cvPutText(img, text, textPos, &font, textColor);
    cv::Mat Img = cv::cvarrToMat(img);
    cv::Rect rect(xmin, ymin, length, wideth);//左上坐标（x,y）和矩形的长(x)宽(y)
    cv::rectangle(Img, rect, cv::Scalar(0, 0, 255),2, cv::LINE_8,0);
    imwrite(toPath, Img);
    cvReleaseImage(&img);

    return 0;


}
