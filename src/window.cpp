#include "window.h"

#include <iostream>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

Window::Window()
{
	mwndWindow = nullptr;
	mwndWindowHandle = nullptr;
}

Window::~Window()
{
	Destroy();
}

bool Window::Create(const char* strTitle, int pWidth, int pHeight)
{
	if (pWidth <= 0 || pHeight <= 0)
	{
		std::cerr << "Invalid width or height parameter when creating window " << strTitle << std::endl;
		return false;
	}

	mwndWindow = glfwCreateWindow(pWidth, pHeight, strTitle, NULL, NULL);

	if (!mwndWindow)
	{
		const char* strErrorInfo;
		glfwGetError(&strErrorInfo);
		std::cerr << "Failed to create window with GLFW. Error: " << strErrorInfo << std::endl;
		return false;
	}

	mwndWindowHandle = glfwGetWin32Window(mwndWindow);

	return true;
}

GLFWwindow* Window::GetWindow()
{
	return mwndWindow;
}

HWND Window::GetWindowHandle()
{
	return mwndWindowHandle;
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(mwndWindow);
}

void Window::Destroy()
{
	glfwDestroyWindow(mwndWindow);
	mwndWindowHandle = nullptr;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(mwndWindow);
}

int Window::GetWidth()
{
	int width, junk;
	glfwGetWindowSize(mwndWindow, &width, &junk);
	return width;
}

int Window::GetHeight()
{
	int height, junk;
	glfwGetWindowSize(mwndWindow, &junk, &height);
	return height;
}
