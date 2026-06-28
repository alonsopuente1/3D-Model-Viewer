#include "globals.h"

// stores the original window procedure of the GLFW3 created window
WNDPROC gwndprocOrigWndProc = nullptr;

// a flag to indicate to exit the program
bool gblnWindowClosed = false;

// menu items events
bool gblnOpenModelFile = false;
bool gblnWireframeToggled = false;
bool gblnNormalToggled = false;
bool gblnPickBackgroundColour = false;
bool gblnPickModelColour = false;
