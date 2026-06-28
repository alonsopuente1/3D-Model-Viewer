#include "applicationcallbacks.h"

#include "globals.h"
#include "application.h"

#include <windowsx.h> // GET_X_LPARAM() GET_Y_LPARAM()

#define CallGLFWWndProc() CallWindowProc(gwndprocOrigWndProc, hWnd, uMsg, wParam, lParam)

static bool blnQuitMessageBoxOpen = false;
static bool blnHelpMsgBoxOpen = false;

enum TutorialStage : int {
	None = 0,
	ClickFile,
	ToggleWireframe,
	ToggleNormal,
	ChangeBackgroundColour,
	ChangeModelColour
};

static TutorialStage tsCurrentStep = None;

void HandleTutorialStage(Application::MenuItems pMenuItemClicked)
{
	// check if user pressed the help button while in tutorial
	if (pMenuItemClicked == Application::MenuItems::Help)
	{
		// if so ask if they would like to quit the tutorial
		std::string strCurrentStep = "You are currently on the";
		switch (tsCurrentStep)
		{
		case ClickFile:
			strCurrentStep += " open model file step";
			break;
		case ToggleWireframe:
			strCurrentStep += " toggle wireframe step";
			break;
		case ToggleNormal:
			strCurrentStep += " toggle normal step";
			break;
		case ChangeBackgroundColour:
			strCurrentStep += " change background colour step";
			break;
		case ChangeModelColour:
			strCurrentStep += " change model colour step";
			break;
		}

		strCurrentStep += ". Would you like to exit the tutorial?";
		if (MessageBoxA(NULL, strCurrentStep.c_str(), "Tutorial", MB_ICONINFORMATION | MB_YESNO) == IDYES)
			tsCurrentStep = None;
		return;
	}


	// check which step the user is on
	switch (tsCurrentStep)
	{
		// if on the 'open model' step of tutorial
	case ClickFile:
	{
		// check if any other menu item was pressed, if so redirect user
		if (pMenuItemClicked != Application::MenuItems::FileOpen)
		{
			MessageBoxA(NULL, "That's the wrong option, try File->Open Model File", "Whoops!", MB_ICONHAND | MB_OK);
			return;
		}

		// display message to user
		MessageBoxA(NULL, "Great! Now choose a valid model file to load it.", "Tutorial", MB_ICONINFORMATION | MB_OK);

		// enter loop to try again if model fails to load first try
		while (true)
		{
			if (!gApp.OpenNewModelFile())
			{
				// inform user model file didn't load. ask to try again
				if (MessageBoxA(NULL, "The model file unsuccessfully loaded!\nIf you don't have a model file, you can exit the tutorial until you have one\n\nWould you like to try again?", "Whoops!", MB_ICONASTERISK | MB_YESNO) == IDNO)
				{
					// stop tutorial if user pressed no
					tsCurrentStep = None;
					return;
				}
			}
			else
				break;
		}

		// give user the next step in tutorial
		MessageBoxA(NULL, "Now you can see your cool model file in the program!\nNext, let's try viewing its wireframe. Click View->Toggle Wireframe to view it.", "Tutorial", MB_ICONINFORMATION | MB_OK);
		tsCurrentStep = ToggleWireframe;

		return;
	}

	// if on the toggle wireframe step of tutorial
	case ToggleWireframe:
		if (pMenuItemClicked != Application::MenuItems::WireframeToggle)
		{
			MessageBoxA(NULL, "Wrong one! Remember, go to View->Toggle Wireframe, it's right next to where you clicked File.", "Whoops!", MB_ICONHAND | MB_OK);
			return;
		}

		tsCurrentStep = ToggleNormal;
		MessageBoxA(NULL, "That's a lot of lines. Lets have a go at viewing the normals! Try View->Toggle Normals", "Tutorial", MB_ICONINFORMATION | MB_OK);
		gApp.ToggleWireframe();
		return;

		// if on the toggle normal step of tutorial
	case ToggleNormal:
		if (pMenuItemClicked != Application::MenuItems::NormalToggle)
		{
			MessageBoxA(NULL, "That's not it... Remember, View->Toggle Normal (just underneath where you toggle the wireframe!)", "Whoops!", MB_ICONHAND | MB_OK);
			return;
		}

		tsCurrentStep = ChangeBackgroundColour;
		MessageBoxA(NULL, "That's even more lines! Try changing the background colour! Go to View->Change Background Colour", "Tutorial", MB_ICONINFORMATION | MB_OK);
		gApp.ToggleNormal();
		return;

		// check if on the change background colour step of tutorial
	case ChangeBackgroundColour:
		if (pMenuItemClicked != Application::MenuItems::PickBackgroundColour)
		{
			MessageBoxA(NULL, "Try again! The change background button is in the View section", "Whoops!", MB_ICONHAND | MB_OK);
			return;
		}

		tsCurrentStep = ChangeModelColour;
		MessageBoxA(NULL, "Now pick a colour!", "Tutorial", MB_OK | MB_ICONINFORMATION);
		gApp.ChangeBackgroundColour();

		MessageBoxA(NULL, "You can also change the colour of the model, try it!", "Tutorial", MB_ICONINFORMATION | MB_OK);
		return;

		// check if on the change model step of tutorial
	case ChangeModelColour:
		if (pMenuItemClicked != Application::MenuItems::PickModelColour)
		{
			MessageBoxA(NULL, "That's the wrong option... Try looking at View->Change Model Colour", "Whoops!", MB_ICONHAND | MB_OK);
			return;
		}

		tsCurrentStep = None;
		MessageBoxA(NULL, "Pick another colour (preferrably a different colour)", "Tutorial", MB_OK | MB_ICONINFORMATION);
		gApp.ChangeModelColour();
		MessageBoxA(NULL, "Well done! Now you know how to use the program! Have fun viewing your models!", "Tutorial", MB_OK | MB_ICONASTERISK);
		return;
	}
}

// this handles any menu item clicks
void HandleMenuItem(WPARAM wParam)
{
	// firstly, check if in tutorial. if so handle the tutorial step
	if (tsCurrentStep)
	{
		HandleTutorialStage((Application::MenuItems)LOWORD(wParam));
		return;
	}

	switch (LOWORD(wParam))
	{
		// check if it was the 'File Open' menu item
	case Application::MenuItems::FileOpen:
		gApp.OpenNewModelFile();
		break;

		// check for 'Toggle Wireframe' menu item
	case Application::MenuItems::WireframeToggle:
		gApp.ToggleWireframe();
		break;

		// check for 'Toggle Normal' menu item
	case Application::MenuItems::NormalToggle:
		gApp.ToggleNormal();
		break;

		// check for 'Change Background Colour' menu item
	case Application::MenuItems::PickBackgroundColour:
		gApp.ChangeBackgroundColour();
		break;

		// check for 'Change Model Colour' menu item
	case Application::MenuItems::PickModelColour:
		gApp.ChangeModelColour();
		break;

		// check for help menu item
	case Application::MenuItems::Help:
	{
		if (blnHelpMsgBoxOpen)
			break;

		blnHelpMsgBoxOpen = true;

		// ask if user wants to enter tutorial
		int intResponse = MessageBoxA(NULL, "Welcome to the 3D Model Viewer!\nWould you like a tutorial to use the program?", "Help", MB_ICONINFORMATION | MB_YESNO);
		if (intResponse == IDNO)
		{
			blnHelpMsgBoxOpen = false;
			break;
		}
		tsCurrentStep = ClickFile;


		// if so, make sure that the user understands how to rotate/zoom model
		while (MessageBoxA(NULL, "Before beginning to look at the buttons at the top, you first need to know how to manipulate your model.\n\nTo rotate the model, click and drag the mouse.\nTo zoom in and out, scroll up and down.\nLastly, if you want to exit the tutorial, press the help button again. Got it?", "Tutorial", MB_YESNO | MB_ICONINFORMATION) == IDNO);

		// afterwards, initiate the first step of tutorial
		MessageBoxA(NULL, "Firstly, lets try opening a model file. Click File->Open Model File at the top left of the window", "Tutorial", MB_ICONINFORMATION | MB_OK);

		blnHelpMsgBoxOpen = false;

		break;
	}
	// check for 'Quit' menu item
	case Application::MenuItems::Quit:
		// if there isnt one already open
		if (!blnQuitMessageBoxOpen)
		{
			// set quit message box to open
			blnQuitMessageBoxOpen = true;

			// the MessageBox function wont return until it has finished
			int intReturnVal = MessageBox(NULL, __TEXT("Are you sure you want to quit?"), __TEXT("Exit"), MB_YESNO);

			// if the user clicked yes then close the window
			if (intReturnVal == IDYES)
			{
				gApp.Stop();
			}

			// execution can only reach here if MessageBox returns
			blnQuitMessageBoxOpen = false;
		}
		break;

	// anything else just get outta here
	default:
		break;
	}
}

// used to update the bounding rectangle when the window is resized
extern std::vector<ToolTip> arrToolTips;

// Used in the main application to check which 
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// windows message
	switch (uMsg)
	{
	// if a menu item has been clicked
	case WM_COMMAND:
	{
		HandleMenuItem(wParam);
		return CallGLFWWndProc();
	} 
	case WM_SIZE:
	{
		for (ToolTip& tt : arrToolTips)
			tt.Update(hWnd);

		// call original window proc to keep GLFW3 functionality
		return CallGLFWWndProc();
	}

	// for any other message than a menu item click, use the original GLFW3 created WindowProc
	default:
		return CallGLFWWndProc();
	}
}