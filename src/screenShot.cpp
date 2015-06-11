#include "CandyLib.h"
#include <stdio.h>
void GetScreenShot(void)
{
    int x1, y1, x2, y2, w, h;
    int ScreenX = 0;
    int ScreenY = 0;
    BYTE* ScreenData = 0;
    // get screen dimensions
    x1  = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y1  = GetSystemMetrics(SM_YVIRTUALSCREEN);
    x2  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    y2  = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    w   = x2 - x1;
    h   = y2 - y1;
    ScreenX = w;
    ScreenY = h;

    // copy screen to bitmap
    HDC     hScreen = GetDC(NULL);
    HDC     hDC     = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL    bRet    = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);

    // Debug : save bitmap to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = ScreenX;
    bmi.biHeight = -ScreenY;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

    // get byteStream
    ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);
    GetDIBits(hDC, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    FILE* fp = fopen("byteStream", "w");
    fwrite(ScreenData, sizeof(BYTE), 4 * ScreenX * ScreenY, fp);
    fclose(fp);

    // convert byteStream to r/g/b bitmap
    Pixel** bitmap = new Pixel*[ScreenY];
    for(int i = 0; i < ScreenY; i++){
        bitmap[i] = new Pixel[ScreenX];
    }
    for(int i = 0; i < ScreenY; i++) {
        for(int j = 0; j < ScreenX; j++) {
            bitmap[i][j].b = ScreenData[4*((i*ScreenX)+j)];
            bitmap[i][j].g = ScreenData[4*((i*ScreenX)+j)+1];
            bitmap[i][j].r = ScreenData[4*((i*ScreenX)+j)+2];
        }
    }

    // call Bmp2GameState
    Bmp2GameState(bitmap, ScreenX, ScreenY);


    // clean up
    for(int i = 0; i < ScreenY; i++) {
        delete bitmap[i];
    }
    delete bitmap;
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}
