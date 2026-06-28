#pragma once

/*
window.h

class for creating a window and 
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();
	bool Create(const char* strTitle, int pWidth, int pHeight);
	GLFWwindow* GetWindow();
	HWND GetWindowHandle();
	void MakeContextCurrent();
	void Destroy();
	void SwapBuffers();

	int GetWidth();
	int GetHeight();

private:

	GLFWwindow* mwndWindow;
	HWND mwndWindowHandle;
};