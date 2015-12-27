#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;
/**/
string cube = "logo.png";
string src = "raw.jpg";
//string light = "light.png";
/**********************************************************************
 *    ncnxj5														  *
 *    2015/12														  *
 *		Copy Right @												  *
/*********************************************************************/

int main()
{
	string file;
	cout << "set source:" << endl;
	cin >> file;
	IplImage *Image_CUBE;
	IplImage *Image_SOURCE;
	IplImage *Image_RESULT;
	IplImage *Image_OUT;
	IplImage *Image_LIGHTIN;
	IplImage *Image_LIGHT;

	CvScalar sR_CUBE;
	CvScalar sR_SOURCE;
	CvScalar sR_LIGHT;
	int x, y;
	
	Image_CUBE = cvLoadImage((cube).c_str(), 1);		//Load the pic matrix
	Image_SOURCE = cvLoadImage((file).c_str(), 1);		//Load the pic matrix
	//Image_LIGHTIN = cvLoadImage((light).c_str(), 1);
	if(!Image_SOURCE)	{
		Image_SOURCE = cvLoadImage((src).c_str(), 1);   //Load the pic matrix
		cout << "cannot open file:" + file<<endl;
	}

	int height_c, width_c, height, width,height_r,width_r;

	int K = 1;											//to ensure the pic is big enough
	height_c = Image_CUBE->height;
	width_c = Image_CUBE->width;
	height = Image_SOURCE->height;
	width = Image_SOURCE->width;
	//cout << height_c << " " << width_c << endl;

	CvSize czSize;										//the size of the new
	K = width_c * 17 / width+1;
														//caculate the size
	czSize.width = width * K;
	czSize.height = height * K;

	Image_RESULT = cvCreateImage(czSize, Image_SOURCE->depth, Image_SOURCE->nChannels);
	cvResize(Image_SOURCE, Image_RESULT, CV_INTER_AREA);
	//Image_LIGHT= cvCreateImage(czSize, Image_SOURCE->depth, Image_SOURCE->nChannels);
	//cvResize(Image_LIGHTIN, Image_LIGHT, CV_INTER_AREA);

	height = Image_RESULT->height;
	width = Image_RESULT->width;

	int R = 0;
	int G = 0;
	int B = 0;
	cout << "start caculating average...\n";
	int i = 0;
	int j = 0;

	for (i = 0;i < height- height_c;i += height_c)
	{
		
		for (j = 0;j < width- width_c;j += width_c)
		{

			for (int p = 0;p < height_c;p++)
				for (int q = 0;q < width_c;q++)
				{
					sR_SOURCE = cvGet2D(Image_RESULT, i + p, j + q);
					//naive mode : just caculate the average
					G += sR_SOURCE.val[0];							
					B += sR_SOURCE.val[1];
					R += sR_SOURCE.val[2];
				}
			G /= height_c*width_c;
			B /= height_c*width_c;
			R /= height_c*width_c;
			for (int p = 0;p < height_c;p++)
				for (int q = 0;q < width_c;q++)
				{
					sR_SOURCE = cvGet2D(Image_RESULT, i + p, j + q);
					sR_SOURCE.val[0] = G;
					sR_SOURCE.val[1] = B;
					sR_SOURCE.val[2] = R;
					cvSet2D(Image_RESULT, i + p, j + q, sR_SOURCE);
				}
			G = B = R = 0;
			height_r = i;
			width_r = j;
		}

	}
	cout << "start combining the cover...\n";

	for (int i = 0;i < height;i++)
	{
		for (int j = 0;j < width;j++)
		{
			//cout << i << " " << j << endl;
			//sR_LIGHT= cvGet2D(Image_LIGHT, i, j);
			sR_SOURCE = cvGet2D(Image_RESULT, i, j);
			sR_CUBE = cvGet2D(Image_CUBE, i%height_c, j%width_c);
			sR_SOURCE.val[0] += sR_CUBE.val[0];//+ sR_LIGHT.val[0];
			sR_SOURCE.val[1] += sR_CUBE.val[1];// +sR_LIGHT.val[1];
			sR_SOURCE.val[2] += sR_CUBE.val[2];// +sR_LIGHT.val[2];
			cvSet2D(Image_RESULT, i, j, sR_SOURCE);
		}

	}
	cvSetImageROI(Image_RESULT, cvRect(0, 0, width_r, height_r));
	//cut the coner of the
	Image_OUT = cvCreateImage(cvSize(width_r, height_r),IPL_DEPTH_8U, Image_RESULT->nChannels);
	cvCopy(Image_RESULT, Image_OUT, 0);

	cout << "finished!\n";

	cvNamedWindow("Source", 1);			//print the source pic
	cvShowImage("Source", Image_OUT);
	cout << "saving as result.png\n";
	cvSaveImage("result.png", Image_OUT);
	cvWaitKey(0);
	cvReleaseImage(&Image_CUBE);
	cvReleaseImage(&Image_SOURCE);
	return 0;
}
