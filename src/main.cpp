#include <windows.h>
#include <stdio.h>
#include "CandyLib.h"

int main()
{
    // Set the hook
    SetHook();

    // Don't mind this, it is a meaningless loop to keep a console application running.
    // I used this to test the keyboard hook functionality. If you want to test it, keep it in ;)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {

    }
}
