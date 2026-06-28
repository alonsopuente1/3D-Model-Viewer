#pragma once

/*
application.h

contains the class definition for the Application class which contains the full program
*/

#include <iostream>
#include <Windows.h>
#include <CommCtrl.h>

#include "window.h"
#include "Model.h"
#include "MainShader.h"
#include "LineShader.h"
#include "ToolTip.h"

// tooltips in program
extern std::vector<ToolTip> arrToolTips;



class Application
{
public:
	
	// Menu item IDs
	enum MenuItems : int {
		FileOpen = 0,
		WireframeToggle,
		NormalToggle,
		PickBackgroundColour,
		PickModelColour,
		Help,
		Quit
	};

	/*--------Public Methods--------*/

	// initialises all members
	Application();

	// deallocates all memory used
	~Application();

	// initialises all components to the application
	void Init();

	// this starts the main loop of the program
	void Run(float pTargetFPS = 30.f);

	// opens an open file dialog and loads the model user opens
	bool OpenNewModelFile();

	// Toggles the respective feature and changes the checkbox status of respective menu item
	void ToggleWireframe();
	void ToggleNormal();

	// changes the colour of the background chosen by user
	inline void ChangeBackgroundColour()
	{
		mvec3BackgroundColour = ChooseColour();
	}

	// changes the colour of the model chosen by user
	inline void ChangeModelColour()
	{
		mvec3ModelColour = ChooseColour();
	}

	// stops the program
	inline void Stop()
	{
		mblnRunning = false;
	}
private:
	/*--------Private Methods--------*/
	
	// Main loop functions of program
	void HandleEvents();
	void Update(float pDeltaTimeMs);
	void Draw();

	// Toggles the checkbox of any menu item that is passed through
	void ToggleMenuItemCheckmark(MenuItems pMenuItemID);

	// Sets up the menubar for usability
	void SetupMenus();

	void SetupTooltips();

	// Opens the choose colour common dialog by windows and returns the colour the user selected.
	// RGB values of vec returned in range of 0-1
	glm::vec3 ChooseColour();

	// Processes the inputs from keyboard and mouse
	void ProcessInputs();


	/*--------Private members--------*/

	// Main window of the program
	Window mwndMainWindow;

	// Rotation of model
	float mfltPitch;
	float mfltYaw;

	// True if the program is running
	bool mblnRunning;

	// Stores title of this program
	const std::string mstrTitle;

	// Model object itself
	Model mModel;

	// Flags to know whether normals/wireframe should be rendered
	bool mViewNormals;
	bool mViewWireframe;

	// Controls the zoom magnitude. The larger the number the further away the model and vice versa
	float mfltZoom;

	// RGB value of respective colour. RGB Values are stored in range 0-1
	glm::vec3 mvec3BackgroundColour;
	glm::vec3 mvec3ModelColour;

	// Stores where the mouse was initially pressed
	glm::vec2 mvec2MousePressedPos;

	// Different shaders for rendering
	MainShader mShader;
	LineShader mNormalShader;
};

// main application variable. global so that menu item responses take effect
extern Application gApp;