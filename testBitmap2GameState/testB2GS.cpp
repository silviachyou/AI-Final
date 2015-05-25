#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

typedef unsigned char BYTE;
typedef struct pixel {
	BYTE r, g, b;
	//BYTE a;
} Pixel;

typedef struct boundary{
	int top, bottom, left, right;
} Boundary;

// You can change prototype
void Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY);
Boundary findBoundary(Pixel** &bitmap, int ScreenX, int ScreenY);

int main() {
	FILE* fp = fopen("byteStream", "r");
	int ScreenX = 1366;
	int ScreenY = 768;
	BYTE* ScreenData = new BYTE[4 * ScreenX * ScreenY];

	Pixel** bitmap = new Pixel*[ScreenY];
	for(int i = 0; i < ScreenY; i++) {
		bitmap[i] = new Pixel[ScreenX];
	}

	fread(ScreenData, sizeof(BYTE), 4 * ScreenX * ScreenY, fp);

	// copy byteStream to bitmap
	/*
	for(int i = 0 ; i < ScreenY; i++) {
		for(int j = 0; j < ScreenX; j++) {
			//bitmap[i][j].a = ScreenData[4*((i*ScreenX)+j)];
			bitmap[i][j].b = ScreenData[4*((i*ScreenX)+j)+1];
			bitmap[i][j].g = ScreenData[4*((i*ScreenX)+j)+2];
			bitmap[i][j].r = ScreenData[4*((i*ScreenX)+j)+3];
		}
	}
	*/
	//checkBitmap(bitmap, ScreenX, ScreenY);


	// Put rgb into bitmap
	Mat image;
	image = imread("screenShot4.png", CV_LOAD_IMAGE_COLOR);

	
	for(int i = 0 ; i < ScreenY; i++) {
		for(int j = 0; j < ScreenX; j++) {
			bitmap[i][j].b = image.at<Vec3b>(i,j).val[0];
			bitmap[i][j].g = image.at<Vec3b>(i,j).val[1];
			bitmap[i][j].r = image.at<Vec3b>(i,j).val[2];
		}
	}

	Bmp2GameState(bitmap, ScreenX, ScreenY);


	/*--- Chech if img read properly ---*/
	/*
	namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);
    */

	return 0;
}

void checkBitmap(Pixel** &bitmap, int ScreenX, int ScreenY){
	
	Mat test = Mat(ScreenY, ScreenX, CV_8UC3);
	// 0 B
	// 1 G
	// 2 R
	for(int i = 0 ; i < ScreenY; i++) {
		for(int j = 0; j < ScreenX; j++) {
			test.at<Vec3b>(i,j) = Vec3b(bitmap[i][j].b, bitmap[i][j].g, bitmap[i][j].r);
		}
	}

	namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", test );                   // Show our image inside it.

    waitKey(0);

}

void Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY) {

	// this is used for verify the correctness of content of bitmap
	// you can modify the code here
	/*
	FILE* fp = fopen("bitmap1.txt", "w");
	for(int i = 0; i < ScreenY; i++) {
		fprintf(fp, "%d\n", i);
		for(int j = 0; j < ScreenX; j++) {

			//fprintf(fp, "(%d,%d,%d) ", bitmap[i][j].r, bitmap[i][j].g, bitmap[i][j].b);
			
			
			if(bitmap[i][j].r <= 70 && bitmap[i][j].r >= 60 && 
				bitmap[i][j].g <= 95 && bitmap[i][j].g >= 75 &&
				bitmap[i][j].b <= 120 && bitmap[i][j].b >= 100){
				fprintf(fp, "%d ", j);
			}
			

		}
		fprintf(fp, "\n\n\n");
	}
	fclose(fp);
	*/

	Boundary b = findBoundary(bitmap, ScreenX, ScreenY);

	int blockWidth = 63, blockHeight = 55;
	
	cout << "top: " << b.top << ", bottom: " << b.bottom << ", left: " << b.left << ", right: " << b.right << endl;
	
	Mat test;
	test = imread("screenShot4.png", CV_LOAD_IMAGE_COLOR);

	line(test,Point(0, b.top),Point(ScreenX, b.top), Scalar(255,0,0), 5);
	line(test,Point(0, b.bottom),Point(ScreenX, b.bottom), Scalar(255,0,0), 5);
	line(test,Point(b.left, 0),Point(b.left, ScreenY), Scalar(255,0,0), 5);
	line(test,Point(b.right, 0),Point(b.right, ScreenY), Scalar(255,0,0), 5);

/*
	for (int i = b.top + 10; i < b.bottom; i += (blockHeight+8)){
		for(int j = b.left + 10; j<b.right; j+= (blockWidth+8)){
			line(test,Point(j, i), Point(j + blockWidth, i + blockHeight), Scalar(0,0,255), 3);
		}
	}
*/
	//line(test,Point(342, 140),Point(600, 140), Scalar(255,255,255), 3);
	
	

	namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", test );                   // Show our image inside it.
    waitKey(0);
    
}

Boundary findBoundary(Pixel** &bitmap, int ScreenX, int ScreenY){
	
	int initLeft = 150, initRight = 920;

	Boundary bound;
	bound.top = 0;
	bound.bottom = ScreenY;
	bound.left = initLeft;
	bound.right = initRight;

	int threshold = 100;
	int count = 0;

	//-- For testing use --//
	/*
	Mat test;
	test = imread("screenShot4.png", CV_LOAD_IMAGE_COLOR);
	//line(test,Point(initLeft, 0),Point(initLeft, ScreenY), Scalar(255,0,0), 3);
	//line(test,Point(initRight, 0),Point(initRight, ScreenY), Scalar(255,0,0), 3);
	*/

	//-- Find Top Boundary --//
	for(int i = 0; i < ScreenY; i++) {
		if(bound.top != 0)
			break;

		count = 0;
		
		for(int j = 0; j < ScreenX; j++) {
			
			if((bitmap[i][j].r >= 75) && (bitmap[i][j].r <= 100) && 
				(bitmap[i][j].g >= 95) && (bitmap[i][j].g <= 120) &&
				(bitmap[i][j].b >= 115) && (bitmap[i][j].b <= 150)){
				count++;
				cout << i << "," << j << "  ";
			}
			if(count >= threshold){
				bound.top = i;
			//	line(test,Point(0, i),Point(ScreenX, i), Scalar(0,0,255), 5);
				break;
			}

		}
	}
	cout <<endl;
	//-- Find Bottom Boundary --//
	for(int i = ScreenY-1; i >= 0; i--) {
		if(bound.bottom != ScreenY)
			break;

		count = 0;

		for(int j = 0; j < ScreenX; j++) {
			
			if(bitmap[i][j].r >= 75 && bitmap[i][j].r <= 100 && 
				bitmap[i][j].g >= 95 && bitmap[i][j].g <= 120 &&
				bitmap[i][j].b >= 115 && bitmap[i][j].b <= 150){
				count++;
			}
			if(count >= threshold){
				bound.bottom = i;
			//	line(test,Point(0, i),Point(ScreenX, i), Scalar(0,0,255), 5);
				break;
			}

		}
	}

	//-- Find Left Boundary --//
	for(int j = initLeft; j < ScreenX; j++) {
		if(bound.left != initLeft)
			break;

		count = 0;

		for(int i = 0; i < ScreenY; i++) {
			
			if(bitmap[i][j].r >= 75 && bitmap[i][j].r <= 100 && 
				bitmap[i][j].g >= 95 && bitmap[i][j].g <= 120 &&
				bitmap[i][j].b >= 115 && bitmap[i][j].b <= 150){
				count++;
			}
			if(count >= threshold){
				bound.left = j;
			//	line(test,Point(j, 0),Point(j, ScreenY), Scalar(0,0,255), 5);
				break;
			}

		}
	}

	for(int j = initRight; j >= 0; j--) {
		if(bound.right != initRight)
			break;
		
		count = 0;

		for(int i = 0; i < ScreenY; i++) {
			
			if(bitmap[i][j].r >= 75 && bitmap[i][j].r <= 100 && 
				bitmap[i][j].g >= 95 && bitmap[i][j].g <= 120 &&
				bitmap[i][j].b >= 115 && bitmap[i][j].b <= 150){
				count++;
			}
			if(count >= threshold){
				bound.right = j;
			//	line(test,Point(j, 0),Point(j, ScreenY), Scalar(0,0,255), 5);
				break;
			}

		}
	}

	/*
	namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", test );                   // Show our image inside it.
    waitKey(0);
	*/
    return bound;
}
