#pragma once

/*
applicationcallbacks.h

stores callback function for the main window of the application
*/

#include <Windows.h>
#include <CommCtrl.h>

// window proc for the application's main window to handle menu items
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);