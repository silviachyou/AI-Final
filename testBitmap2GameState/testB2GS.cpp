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
Boundary FindBoundary(Pixel** &bitmap, int ScreenX, int ScreenY);
bool IsBoundColor(Pixel** &bitmap, int i, int j);
bool HasCandy(Pixel** &bitmap, int w, int h, int i, int j);
bool IsGray(Pixel** &bitmap, int i, int j);
int FindColor(Pixel** &bitmap, int i, int j);

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
	//CheckBitmap(bitmap, ScreenX, ScreenY);


	// Put image RGB values into bitmap
	Mat image;
	image = imread("screenShot3.png", CV_LOAD_IMAGE_COLOR);

	
	for(int i = 0 ; i < ScreenY; i++) {
		for(int j = 0; j < ScreenX; j++) {
			bitmap[i][j].b = image.at<Vec3b>(i,j).val[0];
			bitmap[i][j].g = image.at<Vec3b>(i,j).val[1];
			bitmap[i][j].r = image.at<Vec3b>(i,j).val[2];
		}
	}

	Bmp2GameState(bitmap, ScreenX, ScreenY);


	// Create a window for display, check if img read properly 
	/*
	namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );                   

    waitKey(0);
    */

	return 0;
}

void CheckBitmap(Pixel** &bitmap, int ScreenX, int ScreenY){
	
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


	// Width/ Height of a candy block
	int block_width = 51, block_height = 52;

	// For testing
	Mat test;
	test = imread("screenShot3.png", CV_LOAD_IMAGE_COLOR);

	Boundary b = FindBoundary(bitmap, ScreenX, ScreenY);

	// For testing, show the bounds
	// line(test,Point(0, b.top),Point(ScreenX, b.top), Scalar(255,0,0), 5);
	// line(test,Point(0, b.bottom),Point(ScreenX, b.bottom), Scalar(255,0,0), 5);
	// line(test,Point(b.left, 0),Point(b.left, ScreenY), Scalar(255,0,0), 5);
	// line(test,Point(b.right, 0),Point(b.right, ScreenY), Scalar(255,0,0), 5);

	int map_width = (b.right - b.left) / (block_width + 20);
	int map_height = (b.bottom - b.top) / (block_height + 11);

	// Find game map size
	cout << "MapSize: " << map_width << " x " << map_height << endl;
	

	vector< vector<int> > Map;

	for (int i = b.top + 7; i < b.bottom; i += (block_height+11)){
		vector<int> row;
		for(int j = b.left + 13; j< b.right; j += (block_width+20)){
			
			if( HasCandy(bitmap, block_width, block_height, i, j) ){
			 	int center_i = i + block_height/2;
				int center_j = j + block_width/2;

				int color = FindColor(bitmap, center_i, center_j);
				if(color == -1)
					cout << Map.size() << "," << row.size() << endl << "------" <<endl;
				row.push_back( color );
			}
			else{
				row.push_back(0);
			}

			// line(test,Point(j, i), Point(j + block_width, i + block_height), Scalar(0,0,255), 3);
			// line(test,Point(j+block_width, i), Point(j, i + block_height), Scalar(0,0,255), 3);
		}
		Map.push_back(row);
	}
	
	for(int i = 0; i < Map.size() ; i++){
		for(int j = 0 ; j < Map[i].size(); j++)
			cout << Map[i][j] << " ";
		cout << endl;
	}
	
	// For testing
	namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", test );                   
    waitKey(0);
    
}

int FindColor(Pixel** &bitmap, int i, int j){
	
	// Color Code
	// 0: invalid 
	// 1: red
	// 2: orange
	// 3: yellow
	// 4: green
	// 5: blue
	// 6: purple

	if(bitmap[i][j].r == 255 && bitmap[i][j].g == 1 && bitmap[i][j].b == 0)
		return 1;	//red

	else if(bitmap[i][j].r == 255 && bitmap[i][j].g == 140 && bitmap[i][j].b == 14)
		return 2;	//orange

	else if(bitmap[i][j].r == 253 && bitmap[i][j].g == 232 && bitmap[i][j].b == 0)
		return 3;	//yellow

	else if(bitmap[i][j].r >= 49 && bitmap[i][j].r <= 52 &&
			bitmap[i][j].g >= 179 && bitmap[i][j].g <= 196 &&
			bitmap[i][j].b >= 0 && bitmap[i][j].b <= 1)
		return 4;	//green

	else if(bitmap[i][j].r >= 39 && bitmap[i][j].r <=44 &&
			bitmap[i][j].g >= 158 && bitmap[i][j].g <= 175 && 
			bitmap[i][j].b == 255)
		return 5;	//blue
	
	else if(bitmap[i][j].r == 190 && bitmap[i][j].g == 25 && bitmap[i][j].b == 255)
		return 6;	//purple

	//RGB 6 221 255 strip blue
	
	else{
		cout << (int)bitmap[i][j].r << " " << (int)bitmap[i][j].g << " " << (int)bitmap[i][j].b <<endl;
		return -1;	//cannot detect
	}
}

bool HasCandy(Pixel** &bitmap, int w, int h, int i, int j){

	if(IsGray(bitmap, i, j) && IsGray(bitmap, i, j+w) && IsGray(bitmap, i+h, j) && IsGray(bitmap, i+h, j+w))
		return true;
	else
		return false;
				
}

bool IsGray(Pixel** &bitmap, int i, int j){

	if (bitmap[i][j].r >= 65 && bitmap[i][j].r <= 105 && 
		bitmap[i][j].g >= 90 && bitmap[i][j].g <= 125 &&
		bitmap[i][j].b >= 95 && bitmap[i][j].b <= 150){
		return true;
	}
	else
		return false;
}


Boundary FindBoundary(Pixel** &bitmap, int ScreenX, int ScreenY){
	
	int initLeft = 150, initRight = 920;

	Boundary bound;
	bound.top = 0;
	bound.bottom = ScreenY;
	bound.left = initLeft;
	bound.right = initRight;

	int threshold = 100;
	int count = 0;


	// Find top boundary
	for(int i = 0; i < ScreenY; i++) {
		if(bound.top != 0)
			break;

		count = 0;
		
		for(int j = 0; j < ScreenX; j++) {
			
			if(IsBoundColor(bitmap, i, j)){
				count++;
			}
			if(count >= threshold){
				bound.top = i;
				break;
			}

		}
	}

	// Find bottom boundary
	for(int i = ScreenY-1; i >= 0; i--) {
		if(bound.bottom != ScreenY)
			break;

		count = 0;

		for(int j = 0; j < ScreenX; j++) {
			
			if(IsBoundColor(bitmap, i, j)){
				count++;
			}
			if(count >= threshold){
				bound.bottom = i;
				break;
			}

		}
	}

	// Find left boundary
	for(int j = initLeft; j < ScreenX; j++) {
		if(bound.left != initLeft)
			break;

		count = 0;

		for(int i = 0; i < ScreenY; i++) {
			
			if(IsBoundColor(bitmap, i, j)){
				count++;
			}
			if(count >= threshold){
				bound.left = j;
				break;
			}

		}
	}

	// Find right boundary
	for(int j = initRight; j >= 0; j--) {
		if(bound.right != initRight)
			break;
		
		count = 0;

		for(int i = 0; i < ScreenY; i++) {
			
			if(IsBoundColor(bitmap, i, j)){
				count++;
			}
			if(count >= threshold){
				bound.right = j;
				break;
			}

		}
	}

    return bound;
}

bool IsBoundColor(Pixel** &bitmap, int i, int j){

	if (bitmap[i][j].r >= 75 && bitmap[i][j].r <= 100 && 
		bitmap[i][j].g >= 95 && bitmap[i][j].g <= 120 &&
		bitmap[i][j].b >= 115 && bitmap[i][j].b <= 150){
			return true;
	}
	else
		return false;
}
