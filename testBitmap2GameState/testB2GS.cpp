#include <stdio.h>

typedef unsigned char BYTE;
typedef struct pixel {
    BYTE r, g, b;
    //BYTE a;
} Pixel;
// You can change prototype
void Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY);

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
    for(int i = 0 ; i < ScreenY; i++) {
        for(int j = 0; j < ScreenX; j++) {
            //bitmap[i][j].a = ScreenData[4*((i*ScreenX)+j)];
            bitmap[i][j].b = ScreenData[4*((i*ScreenX)+j)+1];
            bitmap[i][j].g = ScreenData[4*((i*ScreenX)+j)+2];
            bitmap[i][j].r = ScreenData[4*((i*ScreenX)+j)+3];
        }
    }

    Bmp2GameState(bitmap, ScreenX, ScreenY);

    return 0;
}

void Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY) {

    // this is used for verify the correctness of content of bitmap
    // you can modify the code here
    
	/*FILE* fp = fopen("bitmap.txt", "w");
    for(int i = 0; i < ScreenY; i++) {
        for(int j = 0; j < ScreenX; j++) {

            fprintf(fp, "(%d,%d,%d) ", bitmap[i][j].r, bitmap[i][j].g, bitmap[i][j].b);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);*/
}
