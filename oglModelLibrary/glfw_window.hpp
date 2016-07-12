#ifndef CARL_WINDOW_H_INCLUDED
#define CARL_WINDOW_H_INCLUDED

//this does not exist yet
#include "gl_lib.hpp"
#include <iostream>

namespace carl
{

	// main calbacks from our app... will be implemented in later
	struct Interface
	{
		static void *app; // <-- an unknown application. I will define this later

		template<class APPLICATION>
		static void OnKeyDown (GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			((APPLICATION*)app)->onKeyDown(key, action);
		}

		template<class APPLICATION>
		static void OnMouseMove (GLFWwindow *window, double x, double y)
		{
			((APPLICATION*)app)->onMouseMove(x,y);
		}

		template<class APPLICATION>
		static void OnMouseDown (GLFWwindow *window, int button, int action, int mods)
		{
			((APPLICATION*)app)->onMouseDown(button,action);
		}

		template<class APPLICATION>
		static void OnMouseScroll (GLFWwindow *window, double scrollA, double scrollB)
		{
			((APPLICATION*)app)->onMouseScroll(scrollA, scrollB);
		}
	};

	void *Interface::app;

	struct Window
	{
		GLFWwindow *window;
		Interface interface;

		int mWidth, mHeight;

		int width()   { return mWidth; }
		int height()  { return mHeight; }
		float ratio() { return (float)mWidth/mHeight; }

		Window() {}

		//Create a Window Context
		template<class APPLICATION>
		void create(APPLICATION *app, int w, int h, const char *name="demo")
		{
			interface.app = app;

			mWidth = w; mHeight = h;

			window = glfwCreateWindow(w,h,name,NULL,NULL);
			if (!window)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1); // force quit interval ( not guaranteed to work with all graphics drivers

			// register callbacks for keyboard and mouse
			glfwSetKeyCallback(window, Interface::OnKeyDown<APPLICATION>);
			glfwSetCursorPosCallback(window, Interface::OnMouseMove<APPLICATION>);
			glfwSetMouseButtonCallback(window, Interface::OnMouseDown<APPLICATION>);
			glfwSetScrollCallback(window, Interface::OnMouseScroll<APPLICATION>);
		}

		//Get the current framebuffer size in pixels and Set the viewport to it
		void setViewport()
		{
			glfwGetFramebufferSize(window, &mWidth, &mHeight);
			glViewport(0,0,mWidth,mHeight);
		}

		// Check whether the window should close
		bool shouldClose()
		{
			return (bool)glfwWindowShouldClose(window);
		}

		// Swap the front and back buffers
		void swapBuffers()
		{
			glfwSwapBuffers(window);
		}

		//Destroy the window
		void destroy()
		{
			glfwDestroyWindow(window);
		}

		~Window()
		{
			destroy();
		}
	};
}
#endif