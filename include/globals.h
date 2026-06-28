#pragma once

/*
globals.h

all global variables used in program are stored here
mostly used for flags to indicate a menu item has been pressed
*/

#include <Windows.h>

// used to store the original GLFW3 created window procedure.
extern WNDPROC gwndprocOrigWndProc;

// flags for window procedure events that the application class needs to handle
extern bool gblnWindowClosed;
extern bool gblnOpenModelFile;
extern bool gblnWireframeToggled;
extern bool gblnNormalToggled;
extern bool gblnPickBackgroundColour;
extern bool gblnPickModelColour;
