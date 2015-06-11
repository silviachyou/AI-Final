#define _WIN32_WINNT 0x0500 //RIGHT
#include <windows.h>
#include "CandyLib.h"

void MouseMove (int x, int y )
{
  double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1;
  double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1;
  double fx = x*(65535.0f/fScreenWidth);
  double fy = y*(65535.0f/fScreenHeight);
  INPUT  Input={0};
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
  Input.mi.dx = fx;
  Input.mi.dy = fy;
  ::SendInput(1,&Input,sizeof(INPUT));
}

void LeftClick ( )
{
  INPUT    Input={0};
  // left down
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));

  // left up
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));
}

void virtualMouseClick(int fromX, int fromY, int toX, int toY) {
    MouseMove(fromX, fromY);
    LeftClick();
    MouseMove(toX, toY);
    LeftClick();
}
