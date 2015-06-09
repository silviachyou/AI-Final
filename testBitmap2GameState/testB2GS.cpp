#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#define invalid 0
#define white 50

#define red 1
#define orange 2
#define yellow 3
#define green 4
#define blue 5
#define purple 6

#define horizontal_red 6
#define horizontal_orange 7
#define horizontal_yellow 8
#define horizontal_green 9
#define horizontal_blue 10
#define horizontal_purple 11
#define vertical_red 12
#define vertical_orange 13
#define vertical_yellow 14
#define vertical_green 15
#define vertical_blue 16
#define vertical_purple 17
#define square_red 18
#define square_orange 19
#define square_yellow 20
#define square_green 21
#define square_blue 22
#define square_purple 23




typedef unsigned char BYTE;
typedef struct pixel {
	BYTE r, g, b;
	//BYTE a;
} Pixel;

typedef struct boundary{
	int top, bottom, left, right;
} Boundary;

typedef struct coordinate
{
	int x, y;
} Coordinate;

// You can change prototype
int** Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY, Coordinate** &candyXY);
Boundary FindBoundary(Pixel** &bitmap, int ScreenX, int ScreenY);
bool IsBoundColor(Pixel p);
bool HasCandy(Pixel** &bitmap, int w, int h, int i, int j);
bool IsSameRange(Pixel p, Pixel q);
bool IsGray(Pixel p);
int DetectCandy(Pixel** &bitmap, int w, int h, int i, int j);
int FindColor(Pixel pixel);


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
	
	// for(int i = 0 ; i < ScreenY; i++) {
	// 	for(int j = 0; j < ScreenX; j++) {
	// 		//bitmap[i][j].a = ScreenData[4*((i*ScreenX)+j)];
	// 		bitmap[i][j].b = ScreenData[4*((i*ScreenX)+j)+1];
	// 		bitmap[i][j].g = ScreenData[4*((i*ScreenX)+j)+2];
	// 		bitmap[i][j].r = ScreenData[4*((i*ScreenX)+j)+3];
	// 	}
	// }
	
	//CheckBitmap(bitmap, ScreenX, ScreenY);


	// Put image RGB values into bitmap using opcnCV
	Mat image;
	image = imread("../img/screenShot4.png", CV_LOAD_IMAGE_COLOR);

	for(int i = 0 ; i < ScreenY; i++) {
		for(int j = 0; j < ScreenX; j++) {
			bitmap[i][j].b = image.at<Vec3b>(i,j).val[0];
			bitmap[i][j].g = image.at<Vec3b>(i,j).val[1];
			bitmap[i][j].r = image.at<Vec3b>(i,j).val[2];
		}
	}

	// candyXY store the (x,y) coordinate of each candy
	Coordinate** candyXY = new Coordinate*[9];
	for(int i = 0; i < 9; i++) {
		candyXY[i] = new Coordinate[9];
	}
	for(int i = 0; i < 9 ; i++){
		for(int j = 0 ; j < 9; j++){
			candyXY[i][j].x = -1;
			candyXY[i][j].y = -1;
		}
	}

	// The candy matrix is stored in candMap[][]
	// candyXY[][] is passed in by reference
	// The values of candyXY matrix will change during Bmp2GameState()
	int** candyMap = Bmp2GameState(bitmap, ScreenX, ScreenY, candyXY);

	// Print out the candy matrix
	for(int i = 0; i < 9 ; i++){
		for(int j = 0 ; j < 9; j++)
			cout << candyMap[i][j] << " ";
		cout << endl;
	}


	// For testing
	// Create a window for display, check if img read properly 	
	namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );                   

    waitKey(0);
    

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

int** Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY, Coordinate** &candyXY) {
	// This fuction will analyze the image bitmap,
	// detect boundaries, block and candy color,
	// then return the 9x9 candy matrix candyMap to main function


	// For debugging
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


	// Allocate and initialize candyMap
	int** candyMap = new int*[9];
	for(int i = 0; i < 9; i++) {
		candyMap[i] = new int[9];
	}

	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			candyMap[i][j] = invalid;
	

	// Here define the Width and Height of a candy block
	int block_width = 51, block_height = 52;
	int x = 0, y = 0;
	int map_width = 9, map_height = 9;


	// For debugging
	// Mat test;
	// test = imread("../img/5.png", CV_LOAD_IMAGE_COLOR);


	// Retreive boundary of the candy map
	// We need boundaries because every level has different matrix shapes
	Boundary b = FindBoundary(bitmap, ScreenX, ScreenY);

	// For debugging 
	// Show the bounds
	// line(test,Point(0, b.top),Point(ScreenX, b.top), Scalar(255,0,0), 5);
	// line(test,Point(0, b.bottom),Point(ScreenX, b.bottom), Scalar(255,0,0), 5);
	// line(test,Point(b.left, 0),Point(b.left, ScreenY), Scalar(255,0,0), 5);
	// line(test,Point(b.right, 0),Point(b.right, ScreenY), Scalar(255,0,0), 5);
	

	map_width = (b.right - b.left) / (block_width + 20);
	map_height = (b.bottom - b.top) / (block_height + 11);
	
	cout << "MapSize: " << map_width << " x " << map_height << endl;


	// The code below fill out the 9x9 candyMap
	// x, y is the index of the 9x9 candyMap
	// i, j is the index of the 1366x768 image bitmap
	for (int i = b.top + 7; i < b.bottom; i += (block_height+11)){
		y = 0;
		for(int j = b.left + 13; j< b.right; j += (block_width+20)){

			if( HasCandy(bitmap, block_width, block_height, i, j) ){
				int color = DetectCandy(bitmap, i, j, i + block_height, j + block_width);
				
				// For debugging
				// if(color == invalid)
				// 	cout << x << "," << y << endl << "------" <<endl;
				candyMap[x][y] = color;

			}
			else
				candyMap[x][y] = invalid;
			
			candyXY[x][y].x = i + block_height / 2;
			candyXY[x][y].y = j + block_width / 2;

			y++;

			// For debugging
			// line(test,Point(j, i), Point(j + block_width, i + block_height), Scalar(0,0,255), 3);
			// line(test,Point(j+block_width, i), Point(j, i + block_height), Scalar(0,0,255), 3);
		}
		x++;
	}
	

	// For debugging
	// Create a window for display, check if img read properly
	// namedWindow( "Display window", WINDOW_AUTOSIZE );
 	// imshow( "Display window", test );                   
 	// waitKey(0);
    
    return candyMap;
}


bool HasCandy(Pixel** &bitmap, int w, int h, int i, int j){
	// This function is to determine whether a block contains candy
	// If all four corners of the block is gray, then it is a valid block
	// else it is not a candy block and cannot be moved.

	if(IsGray(bitmap[i][j]) && IsGray(bitmap[i][j+w]) && IsGray(bitmap[i+h][j]) && IsGray(bitmap[i+h][j+w]))
		return true;
	else if(IsSameRange(bitmap[i+20][j], bitmap[i+40][j]) && IsSameRange(bitmap[i+20][j+w], bitmap[i+40][j+w]))
		return true;
	else
		return false;
				
}

bool IsGray(Pixel p){
	// This is a function called by HasCandy()
	// This function is to determine wheter a pixel is gray
	// If you want to modify the range of the "gray color", modify it below

	if (p.r >= 65 && p.r <= 105 && 
		p.g >= 85 && p.g <= 125 &&
		p.b >= 95 && p.b <= 150)
		return true;
	
	else{
		//cout << (int)p.r << " " << (int)p.g << " " << (int)p.b << endl;
		return false;
	}
}

bool IsSameRange(Pixel p, Pixel q){
	// This is a function called by HasCandy()
	// This function is used to identify the square candy
	// which is not useful in this version of code but will be used soon

	if (abs(p.r - q.r) <= 50 && 
		abs(p.g - q.g) <= 50 &&
		abs(p.b - q.b) <= 50)
		return true;
	
	else
		return false;
	
}




Boundary FindBoundary(Pixel** &bitmap, int ScreenX, int ScreenY){
	// This is where we find the boundaries of the candy matrix in the image
	// This is a function called by main()
	// We can proceed detecting each candy blocks only if we first find the boundaries
	// There are 4 boundaries : top / bottom / left / right
	
	int initLeft = 150, initRight = 920;

	Boundary bound;
	bound.top = 0;
	bound.bottom = ScreenY;
	bound.left = initLeft;
	bound.right = initRight;

	int threshold = 100;

	// Find top boundary
	for(int i = 0; i < ScreenY; i++) {
		if(bound.top != 0)
			break;

		int count = 0;		
		for(int j = 0; j < ScreenX; j++) {
			
			if(IsBoundColor(bitmap[i][j]))
				count++;
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

		int count = 0;
		for(int j = 0; j < ScreenX; j++) {
			
			if(IsBoundColor(bitmap[i][j]))
				count++;
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

		int count = 0;
		for(int i = 0; i < ScreenY; i++) {
			
			if(IsBoundColor(bitmap[i][j]))
				count++;
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
		
		int count = 0;
		for(int i = 0; i < ScreenY; i++) {
			
			if(IsBoundColor(bitmap[i][j]))
				count++;
			if(count >= threshold){
				bound.right = j;
				break;
			}

		}
	}

    return bound;
}


bool IsBoundColor(Pixel p){
	// This function that will be called by FindBoundary(),
	// to determine whether a pixel's color is boundary color

	if (p.r >= 75 && p.r <= 100 && 
		p.g >= 95 && p.g <= 120 &&
		p.b >= 115 && p.b <= 150)
			return true;
	
	else
		return false;
}


int DetectCandy(Pixel** &bitmap, int start_i, int start_j, int end_i, int end_j){
	// This is the function called by main(), 
	// it detect the candy color and pattern within a block by calling FindColor() function
	
	
	int color = invalid;
	bool colorFound = false;
	
	// variables not used by now
	// int center_i = (start_i + end_i) / 2;
	// int center_j = (start_j + end_j) / 2;
	// Pixel pixel = bitmap[center_i][center_j];
	// int whiteSum = 0;
	// float ratio;

	for(int i = start_i; i < end_i; i++){
		for(int j = start_j; j< end_j; j++){
			if(colorFound)
				break;
			color = FindColor(bitmap[i][j]);
			if( color != white && color != invalid ){
				colorFound = true;
				break;
			}
		}
	}
	

	return color;


	// if(pixel.r >= 225 && pixel.r <= 250 &&
	// 		pixel.g >= 240 && pixel.g <= 250 &&
	// 		pixel.b >= 230 && pixel.b <= 240)
	// 	return horizontal_red; 

	// else if(pixel.r >= 250 && pixel.r <= 255 &&
	// 		pixel.g >= 245 && pixel.g <= 255 &&
	// 		pixel.b >= 215 && pixel.b <= 225)
	// 	return horizontal_orange;

	// else if(pixel.r == 255 && 
	// 		pixel.g == 211 && 
	// 		pixel.b == 0)
	// 	return horizontal_yellow;	

	// else if(pixel.r == 138 && 
	// 		pixel.g == 255 && 
	// 		pixel.b == 102)
	// 	return horizontal_green;

	// else if(pixel.r >= 5 && pixel.r <= 20 &&
	// 		pixel.g >= 195 && pixel.g <= 225 &&
	// 		pixel.b >= 250 && pixel.b <= 255)
	// 	return horizontal_blue;

	// else if(pixel.r >= 250 && pixel.r <= 255 &&
	// 		pixel.g >= 250 && pixel.g <= 255 &&
	// 		pixel.b >= 250 && pixel.b <= 255)
	// 	return horizontal_purple;

	// else if(pixel.r == 247 && 
	// 		pixel.g == 146 && 
	// 		pixel.b == 12)
	// 	return vertical_orange;

	// else if(pixel.r == 30 && 
	// 		pixel.g == 159 && 
	// 		pixel.b == 233)
	// 	return vertical_blue;

	// else if(pixel.r >= 25 && pixel.r <= 35 &&
	// 		pixel.g >= 230 && pixel.g <= 245 &&
	// 		pixel.b >= 0 && pixel.b <= 5)
	// 	return vertical_green;

	// else if(pixel.r == 248 && 
	// 		pixel.g == 255 && 
	// 		pixel.b == 222)
	// 	return vertical_purple;

	// else if(pixel.r >= 89 && pixel.r <= 100 &&
	// 		pixel.g >= 225 && pixel.g <= 235 &&
	// 		pixel.b >= 45 && pixel.b <= 60)
	// 	return square_green;

	// else if(pixel.r >= 30 && pixel.r <= 40 &&
	// 		pixel.g >= 190 && pixel.g <= 205 &&
	// 		pixel.b >= 250 && pixel.b <= 255)
	// 	return square_blue;

	// else if(pixel.r >= 215 && pixel.r <= 225 &&
	// 		pixel.g >= 60 && pixel.g <= 70 &&
	// 		pixel.b >= 250 && pixel.b <= 255)
	// 	return square_purple;

	
	// else{
	// 	cout << (int)pixel.r << " " << (int)pixel.g << " " << (int)pixel.b <<endl;
	// 	return invalid;	//cannot detect
	// }
}

int FindColor(Pixel pixel){
	// This function is called by DetectCandy()
	// which will return the color of a pixel

	// Color Code
	// 0: invalid 
	// 1: red
	// 2: orange
	// 3: yellow
	// 4: green
	// 5: blue
	// 6: purple


	if (pixel.r >= 250 && pixel.r <= 255 &&
		pixel.g >= 0 && pixel.g <= 5 &&
		pixel.b >= 0 && pixel.b <= 5)
		return red;

	else if(pixel.r >= 250 && pixel.r <= 255 &&
			pixel.g >= 135 && pixel.g <= 145 &&
			pixel.b >= 10 && pixel.b <= 20)
		return orange;

	else if(pixel.r >= 230 && pixel.r <= 255 && 
			pixel.g >= 210 && pixel.g <= 230 &&
			pixel.b >= 0 && pixel.b <= 10)
		return yellow;

	else if(pixel.r >= 45 && pixel.r <= 55 &&
			pixel.g >= 175 && pixel.g <= 195 &&
			pixel.b >= 0 && pixel.b <= 5)
		return green;

	else if(pixel.r >= 30 && pixel.r <= 45 &&
			pixel.g >= 150 && pixel.g <= 175 && 
			pixel.b >= 250 && pixel.b <= 255)
		return blue;
	
	else if(pixel.r >= 180 && pixel.r <= 190 &&
			pixel.g >= 20 && pixel.g <= 25 &&
			pixel.b >= 250 && pixel.b <= 255)
		return purple;

	else if (pixel.r >= 245 && pixel.r <= 255 &&
			pixel.g >= 245 && pixel.g <= 255 &&
			pixel.b >= 245 && pixel.b <= 255)
		return white;

	
	else{
		//cout << (int)pixel.r << " " << (int)pixel.g << " " << (int)pixel.b <<endl;
		return invalid;
	}
}
