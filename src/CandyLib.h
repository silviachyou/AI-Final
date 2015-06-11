#ifndef CANDYLIB_H_INCLUDED
#define CANDYLIB_H_INCLUDED

#include <windows.h>

typedef struct pixel{
    BYTE r, g, b;
} Pixel;

// use for keyboard detection
void SetHook();
void ReleaseHook();
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

// screen shot
void GetScreenShot();

// bitmap to game state
int** Bmp2GameState(Pixel** &bitmap, int ScreenX, int ScreenY);

// use virtual mouse click to output
void virtualMouseClick(int fromX, int fromY, int toX, int toY);
#endif // CANDYLIB_H_INCLUDED
