#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <iostream>
/*
ToolTip.h

class for tooltips to contain all the code necessary.
only works for menu items and no other controls
*/

class ToolTip
{
public:
	// initialises members 
	ToolTip();

	// deallocate the memory used by the tooltip
	void Delete();

	// creates the tooltip window on the specified menu item position in the
	// pMainWindow
	void Create(HWND pMainWindow, int pItemID, wchar_t* pText);
	void Update(HWND pMainWindow);

private:

	// tooltip
	HWND mhTIWindow;
	TOOLINFOA mtiInfo;

	// the menu item position the tooltip is attached too
	int mId;
};

