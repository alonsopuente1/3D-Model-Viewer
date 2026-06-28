#include "ToolTip.h"

// specifies to use the newer version of the common controls library
// necessary for the tooltips to work
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

ToolTip::ToolTip():
	mhTIWindow(NULL),
	mtiInfo({ 0 }),
	mId(0)
{
}

void ToolTip::Delete()
{
	if (!DestroyWindow(mhTIWindow))
		std::cout << "failed to delete tooltip window" << std::endl;
}

void PrintRect(RECT rc)
{
	std::cout << "Topleft corner x: " << rc.left << " y: " << rc.top << std::endl;
	std::cout << "Bottomright corner x: " << rc.right << " y: " << rc.bottom << std::endl << std::endl;
}

// creates a tooltip for a menu item for a window with the text 
// the creation of the tooltip was taken from https://stackoverflow.com/questions/41731311/how-to-add-tooltip-to-menu-item
// with some help of the MSDN forum too
void ToolTip::Create(HWND pMainWindow, int pItemID, wchar_t* pText)
{
	mhTIWindow = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		pMainWindow, NULL, NULL,
		NULL);
	
	mtiInfo.cbSize = sizeof(TOOLINFO);
	mtiInfo.hwnd = pMainWindow; // main window handle
	mtiInfo.uId = (UINT)pItemID;   // distinguish it from eventual other tooltips
	mtiInfo.uFlags = TTF_SUBCLASS;
	mtiInfo.lpszText = (char*)pText;

	mId = pItemID;	

	SendMessage(mhTIWindow, TTM_ADDTOOL, 0, (LPARAM)&mtiInfo);
	SendMessage(mhTIWindow, TTM_ACTIVATE, TRUE, 0);

	Update(pMainWindow);
}

// should be called when the window is resized to get new rect of menu item
// this function was taken from https://stackoverflow.com/questions/41731311/how-to-add-tooltip-to-menu-item
// however it was changed slightly to support different menu items
void ToolTip::Update(HWND pMainWindow)
{
	mtiInfo.cbSize = sizeof(TOOLINFO);
	mtiInfo.uId = mId;
	GetMenuItemRect(pMainWindow, GetMenu(pMainWindow), mId, &mtiInfo.rect);
	MapWindowPoints(NULL, pMainWindow, (LPPOINT)&mtiInfo.rect, 2);
	SendMessage(mhTIWindow, TTM_NEWTOOLRECT, 0, (LPARAM)&mtiInfo);
}
