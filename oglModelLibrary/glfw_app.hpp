#ifndef glfw_app_INC
#define glfw_app_INC

#include "glfw_window.hpp"

namespace carl
{

	struct App
	{
		Window mWindow;
		Window& window() {return mWindow;}

		App(int w=640, int h=480)
		{
			// Initialize GLFW
			if( !glfwInit() ) exit(EXIT_FAILURE);
			printf("glfw initalized! \n");

			// Create the window, passing this application to it
			mWindow.create(this,w,h);
			printf("glfw window created! \n");

			// Initialize GLEW
			glewExperimental = true;
			GLenum glewError = glewInit();
			if(glewError != GLEW_OK)
			{
				printf("glew init error\n%s\n", glewGetErrorString( glewError) );
			}

			if(GLEW_APPLE_vertex_array_object)
			{
				printf("genVertexArrayAPPLE supported\n");
			} else if (GLEW_ARB_vertex_array_object) {
				printf("genVertexArrays supported\n");
			}

			//OpenGl starting render queue values: Alpha blending and depth testing
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glLineWidth(3);
		}

		// start the draw loop
		void start()
		{
			printf("starting app\n");
			while (!mWindow.shouldClose() )
			{
				mWindow.setViewport();

				//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				onDraw();

				mWindow.swapBuffers();
				glfwPollEvents();
			}
		}

		~App()
		{
			glfwTerminate();
		}

		virtual void onDraw() {}

		virtual void onMouseMove(double x, double y) {}

		virtual void onMouseDown(int button, int action) {}

		virtual void onKeyDown(int key, int action) {}

		virtual void onMouseScroll(double scrollA, double scrollB) {}
	};
}
#endif