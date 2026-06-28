#include "application.h"

// include headers
#include "error.h"
#include "applicationcallbacks.h"
#include "globals.h"
#include "ToolTip.h"

// include libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#define _WIN32_IE 0x0300
#include <Windows.h>
#include <commctrl.h>
#include <glm/ext.hpp>

// stores how much the scroll wheel has scrolled
static float fltScrollMag = 0.0f;
static std::string strFileDragged = "";
static bool blnMessageBoxOpen = false;

std::vector<ToolTip> arrToolTips;
Application gApp;

// Clamps paramter num within range of min-max
void clamp(float& num, float min, float max)
{
	if (num < min)
		num = min;
	else if (num > max)
		num = max;
}

Application::Application() :
	// Initialise private members
	mfltPitch(0.f),
	mfltYaw(0.f),
	mstrTitle("3D Model Viewer"),
	mViewNormals(false),
	mViewWireframe(false),
	mfltZoom(5),
	mvec2MousePressedPos(0.0f),
	mvec3BackgroundColour(0.f, 0.5f, 0.5f),
	mvec3ModelColour(0.5, 0.0f, 0.0f)
{}

Application::~Application()
{
	// Delete all allocated resources before program termination
	for (ToolTip& tt : arrToolTips)
		tt.Delete();
	mModel.Delete();
	mShader.Delete();
	mNormalShader.Delete();
	mwndMainWindow.Destroy();
	glfwTerminate();
}

void Application::Init()
{
	// Sets running to false as default so you just have to return in constructor if something goes wrong
	mblnRunning = false;

	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "Failed to initialise glfw3!" << std::endl;
		return;
	}

	// Use OpenGL Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	mwndMainWindow.Create(mstrTitle.c_str(), 1280, 720);
	mwndMainWindow.MakeContextCurrent();

	// Set the WndProc of the window created by GLFW to a new function to add functionality to the menu items i add. Keep old one
	// for unhandled messages in the new WndProc i created
	gwndprocOrigWndProc = (WNDPROC)SetWindowLongPtr(mwndMainWindow.GetWindowHandle(), GWLP_WNDPROC, (LONG_PTR)MainWindowProc);

	// to be able to use tooltips
	INITCOMMONCONTROLSEX initCommonControls = { 0 };
	initCommonControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	initCommonControls.dwICC = ICC_BAR_CLASSES;
	if (InitCommonControlsEx(&initCommonControls) == FALSE)
	{
		std::cerr << "Common controls failed to load." << std::endl;
		return;
	}

	// Sets up the menu items of the main window
	SetupMenus();
	SetupTooltips();

	// Loads all of the OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialise OpenGL" << std::endl;
		return;
	}

	glEnable(GL_DEPTH_TEST);

	// When window is resized, the dimensions of viewport are resized to keep model in view of window
	glfwSetWindowSizeCallback(mwndMainWindow.GetWindow(), [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});

	// When scroll wheel event occurs, store the y offset in a static variable
	glfwSetScrollCallback(mwndMainWindow.GetWindow(), [](GLFWwindow* window, double xoffset, double yoffset)
		{
			fltScrollMag = yoffset;
		});

	// When a file(s) is drag and dropped, it checks if only one is dragged and dropped and if so, it opens the file
	glfwSetDropCallback(mwndMainWindow.GetWindow(), [](GLFWwindow*, int count, const char** paths)
		{
			if ((count >= 2 || count <= 0) && !blnMessageBoxOpen)
			{
				blnMessageBoxOpen = true;
				MessageBox(NULL, __TEXT("Cannot open more than one model file at once!"), NULL, MB_OK | MB_ICONSTOP);
				blnMessageBoxOpen = false;
				return;
			}
			strFileDragged = paths[0];
		});

	// If the shaders fail to create, the program cannot run correctly so it quits
	if (!mShader.Create())
	{
		std::cerr << "Failed to create MainShader." << std::endl;
		return;
	}

	if (!mNormalShader.Create())
	{
		std::cerr << "Failed to create LineShader" << std::endl;
		return;
	}

	// Load the default model 
	mModel.LoadModel("res/models/suzanne.obj");

	// Set running to true to indicate the constructor worked correctly
	mblnRunning = true;
}

void Application::Run(float pTargetFPS)
{
	// Initialise variables
	float fltNewTimeS = 0.0;
	float fltDeltaTimeMS = 0.0;
	float fltOldTimeS = 0.0;

	float fltMSPerFrame = 1000.f / pTargetFPS;

	while (mblnRunning)
	{		
		// Calculates time passed from last frame 
		fltNewTimeS = (float)glfwGetTime();
		fltDeltaTimeMS = (fltNewTimeS - fltOldTimeS) * 1000.0f;
		fltOldTimeS = fltNewTimeS;
		
		// Run main loop funtions
		HandleEvents();
		Update(fltDeltaTimeMS);
		Draw();
	}
}


void Application::ToggleWireframe()
{
	mViewWireframe = !mViewWireframe;
	ToggleMenuItemCheckmark(MenuItems::WireframeToggle);
}

void Application::ToggleNormal()
{
	mViewNormals = !mViewNormals;
	ToggleMenuItemCheckmark(MenuItems::NormalToggle);
}


void Application::ToggleMenuItemCheckmark(MenuItems pMenuItemID)
{
	// Get the menubar of the window
	HMENU hMenu = GetMenu(mwndMainWindow.GetWindowHandle());

	// Check if the desired menuitem is checked or not
	MENUITEMINFO menuItemInfo = { 0 };
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask = MIIM_STATE;

	GetMenuItemInfo(hMenu, pMenuItemID, FALSE, &menuItemInfo);

	// If so, set to unchecked and vice versa
	if (menuItemInfo.fState == MFS_CHECKED)
		menuItemInfo.fState = MFS_UNCHECKED;
	else
		menuItemInfo.fState = MFS_CHECKED;

	// Update the desired menu item
	SetMenuItemInfo(hMenu, pMenuItemID, FALSE, &menuItemInfo);
}

bool Application::OpenNewModelFile()
{
	// Use OPENFILENAMEA structure over OPENFILENAMEW as the first uses normal C-Style strings instead of wide character styles
	OPENFILENAMEA ofnOpenFile = { 0 };

	char cstrFileName[256] = { 0 };

	// set member variables of structure
	ofnOpenFile.lStructSize = sizeof(OPENFILENAMEA);
	ofnOpenFile.hwndOwner = NULL;
	ofnOpenFile.lpstrFilter = "All Files\0*.*\0\0";
	ofnOpenFile.Flags = OFN_FILEMUSTEXIST;
	ofnOpenFile.nMaxFile = 256;
	ofnOpenFile.lpstrFile = cstrFileName;

	// do api call
	if (GetOpenFileNameA(&ofnOpenFile))
	{
		return mModel.LoadModel(cstrFileName);
	}
	return false;
}

void Application::HandleEvents()
{
	glfwPollEvents();
	
	// This is used to not call the method over and over again;
	GLFWwindow* wndpMainWindow = mwndMainWindow.GetWindow();

	// process keyboard+mouse inputs
	ProcessInputs();

	// Check if the window should close
	if (glfwWindowShouldClose(wndpMainWindow) || gblnWindowClosed)
	{
		mblnRunning = false;
	}
}

void Application::Update(float pDeltaTimeMs)
{
	// avoids dividing by 0 error
	if (pDeltaTimeMs <= 0)
		pDeltaTimeMs = 1.0f;

	float fps = ceil(1000.f / pDeltaTimeMs);

	// zooms the camera based on how much the scroll wheel has scrolled and clamps the zoom between a set distance
	mfltZoom -= fltScrollMag;
	fltScrollMag = 0.0f;
	clamp(mfltZoom, 3.0f, 10000.f);

	// checks if a file has been dragged and if so, load the model
	if (strFileDragged != "")
	{
		mModel.LoadModel(strFileDragged.c_str());
		strFileDragged = "";
	}

	// change the window title to show how much fps the program is on
	std::string strWindowTitle = mstrTitle + " FPS: " + std::to_string((int)fps);
	glfwSetWindowTitle(mwndMainWindow.GetWindow(), strWindowTitle.c_str());
}

void Application::Draw()
{
	// Clear the window the the greenish colour
	mwndMainWindow.MakeContextCurrent();
	glClearColor(mvec3BackgroundColour.r, mvec3BackgroundColour.g, mvec3BackgroundColour.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float fltAspectRatio = (float)mwndMainWindow.GetWidth() / (float)mwndMainWindow.GetHeight();

	// used for projection of all models
	glm::mat4 matMVP = glm::perspective(90.0f, fltAspectRatio, 0.01f, 100000.0f) * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -mfltZoom)) * glm::rotate(glm::mat4(1), mfltPitch, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1), mfltYaw, glm::vec3(0.0f, 1.0f, 0.0f));
	// used for lighting calculations
	glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -mfltZoom)) * glm::rotate(glm::mat4(1), mfltPitch, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1), mfltYaw, glm::vec3(0.0f, 1.0f, 0.0f));
	
	// if wireframe is not on
	if (!mViewWireframe)
	{
		// bind the main shader, set the uniforms and draw the model
		mShader.Bind();
		mShader.SetMVPUniform(matMVP);
		mShader.SetModelMatUniform(matModel);
		mShader.SetModelColour(mvec3ModelColour);
		mModel.Draw(mShader);
		mShader.Unbind();
	}
	else
	{
		// otherwise bind the line shader, set uniforms and draw model
		mNormalShader.Bind();
		mNormalShader.SetMVPUniform(matMVP);
		mModel.DrawWireframe(mNormalShader);
		mNormalShader.Unbind();
	}

	// if normals are on
	if (mViewNormals)
	{
		// bind line shader, set uniforms and draw model
		mNormalShader.Bind();
		mNormalShader.SetMVPUniform(matMVP);
		mModel.DrawNormals(mNormalShader);
		mNormalShader.Unbind();
	}

	// swap buffer to present new screen to user
	mwndMainWindow.SwapBuffers();
}

void Application::SetupMenus()
{
	// create the main menu bar and the file+view menu popups
	HMENU hMenubar = CreateMenu();
	HMENU hFileMenuItem = CreateMenu();
	HMENU hViewMenuItem = CreateMenu();

	// create the file menu dropdown
	AppendMenuA(hFileMenuItem, MF_STRING, (UINT_PTR)MenuItems::FileOpen, "Open Model File\tCtrl+O");
	AppendMenuA(hMenubar, MF_POPUP, (UINT_PTR)hFileMenuItem, "File");

	// create the view menu dropdown
	AppendMenuA(hViewMenuItem, MF_STRING, (UINT_PTR)MenuItems::WireframeToggle, "View Wireframe\tW");
	AppendMenuA(hViewMenuItem, MF_STRING, (UINT_PTR)MenuItems::NormalToggle, "View Normals\tN");
	AppendMenuA(hViewMenuItem, MF_STRING, (UINT_PTR)MenuItems::PickBackgroundColour, "Change Background Colour\tB");
	AppendMenuA(hViewMenuItem, MF_STRING, (UINT_PTR)MenuItems::PickModelColour, "Change Model Colour\tM");
	AppendMenuA(hMenubar, MF_POPUP, (UINT_PTR)hViewMenuItem, "View");

	// create the help button
	AppendMenuA(hMenubar, MF_STRING, (UINT_PTR)MenuItems::Help, "Help");

	// create the quit button
	AppendMenuA(hMenubar, MF_STRING, (UINT_PTR)MenuItems::Quit, "Quit");

	// set the new menu to the menu of window
	SetMenu(mwndMainWindow.GetWindowHandle(), hMenubar);
}

// setup the tooltips
void Application::SetupTooltips()
{
	// temp variable to store the main window handle
	HWND hMainWindow = mwndMainWindow.GetWindowHandle();

	// push back 4 empty tooltips for 4 menu items
	arrToolTips.push_back(ToolTip());
	arrToolTips.push_back(ToolTip());
	arrToolTips.push_back(ToolTip());
	arrToolTips.push_back(ToolTip());
	
	// create the tooltips
	arrToolTips[0].Create(hMainWindow, 0, (wchar_t*)L"Option to open a custom model file");
	arrToolTips[1].Create(hMainWindow, 1, (wchar_t*)L"Options to change colours/toggle features");
	arrToolTips[2].Create(hMainWindow, 2, (wchar_t*)L"Click to learn how to use the 3D model viewer");
	arrToolTips[3].Create(hMainWindow, 3, (wchar_t*)L"Quit the program");
}

glm::vec3 Application::ChooseColour()
{
	// output colour, set to current colour just in case the user cancels the dialog
	glm::vec3 vecColourOutput = mvec3BackgroundColour;
	// needed for creating the choose colour dialog
	COLORREF acrCustClr[16] = { 0 };
	// struct to give the information for the choose colour dialog
	CHOOSECOLOR ccChooseColour = { 0 };

	// filling struct data
	ZeroMemory(&ccChooseColour, sizeof(CHOOSECOLOR));
	ccChooseColour.lStructSize = sizeof(CHOOSECOLOR);
	ccChooseColour.hwndOwner = mwndMainWindow.GetWindowHandle();
	ccChooseColour.Flags = CC_FULLOPEN | CC_RGBINIT;
	ccChooseColour.lpCustColors = (LPDWORD)acrCustClr;

	// run the choose colour dialog
	if (ChooseColor(&ccChooseColour))
	{
		vecColourOutput = glm::vec3((float)GetRValue(ccChooseColour.rgbResult), (float)GetGValue(ccChooseColour.rgbResult), (float)GetBValue(ccChooseColour.rgbResult));
		vecColourOutput /= 255.f;
	}
	
	// return the colour the user chose
	return vecColourOutput;
}

void Application::ProcessInputs()
{
	// saves from calling getter repeatedly
	GLFWwindow* wndpMainWindow = mwndMainWindow.GetWindow();

	// Gets current state of the mouse pressed
	bool blnMousePressed = glfwGetMouseButton(wndpMainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	// Stores the pitch and yaw before the mouse was pressed
	static float fltPrevYaw = 0.0f, fltPrevPitch = 0.0f;
	static bool blnMousePressedLastFrame = false, 
		blnWPressedLast = false, 
		blnNPressedLast = false;

	// This checks if the mouse was just pressed
	if (blnMousePressed && !blnMousePressedLastFrame)
	{
		// Gets where the mouse was clicked and stores in member variable
		double dblMouseX, dblMouseY;
		glfwGetCursorPos(wndpMainWindow, &dblMouseX, &dblMouseY);
		mvec2MousePressedPos = glm::vec2((float)dblMouseX, (float)dblMouseY);

		// stores how the model is rotated on mouse click
		fltPrevYaw = mfltYaw; fltPrevPitch = mfltPitch;
	}
	// This checks if the mouse is being held down
	else if (blnMousePressed && blnMousePressedLastFrame)
	{
		// Get where the mouse currently is
		double dblMouseX, dblMouseY;
		glfwGetCursorPos(wndpMainWindow, &dblMouseX, &dblMouseY);
		glm::vec2 currentPos((float)dblMouseX, (float)dblMouseY);

		// Gets difference in x and y of the current mouse pos to where it was pressed and adds it to pitch and yaw of model
		glm::vec2 deltaPos = currentPos - mvec2MousePressedPos;
		mfltYaw = fltPrevYaw + deltaPos.x / 100.0f;
		mfltPitch = fltPrevPitch + deltaPos.y / 100.0f;
	}




	/*-------- KEYBOARD SHORTCUTS --------*/

	// Check if the escape button was pressed
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(mwndMainWindow.GetWindow(), 1);
	}

	// Check if N key is pressed or normal menu item was clicked
	if ((glfwGetKey(wndpMainWindow, GLFW_KEY_N) == GLFW_PRESS && !blnNPressedLast) || gblnNormalToggled)
	{
		gblnNormalToggled = false;
		ToggleNormal();
	}

	// Check if W key is pressed or wireframe menu item was clicked
	if ((glfwGetKey(wndpMainWindow, GLFW_KEY_W) == GLFW_PRESS && !blnWPressedLast) || gblnWireframeToggled)
	{
		gblnWireframeToggled = false;
		ToggleWireframe();
	}

	// Check if B key is pressed for changing the background colour
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_B) == GLFW_PRESS) 
	{
		ChangeBackgroundColour();
	}
	
	// Check if N is pressed to change model colour
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_M) == GLFW_PRESS)
	{
		ChangeModelColour();
	}

	// Check if CTRL+O are pressed to open a new file
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_O) == GLFW_PRESS && glfwGetKey(wndpMainWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		SendMessage(mwndMainWindow.GetWindowHandle(), WM_COMMAND, MAKEWPARAM(Application::MenuItems::FileOpen, 0), 0);
	}

	/* Check if directional arrow keys are being pressed to rotate model */
	// Left
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		mfltYaw -= 0.01f;
	}

	// Right
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		mfltYaw += 0.01f;
	}

	// Up
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mfltPitch -= 0.01f;
	}

	// Down
	if (glfwGetKey(wndpMainWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mfltPitch += 0.01f;
	}


	// Set the flags 
	blnMousePressedLastFrame = blnMousePressed;
	blnNPressedLast = glfwGetKey(wndpMainWindow, GLFW_KEY_N);
	blnWPressedLast = glfwGetKey(wndpMainWindow, GLFW_KEY_W);
}
