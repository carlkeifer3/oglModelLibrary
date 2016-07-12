
#include "glfw_app.hpp"
#include "3dModels.h"

#define GLM_FORCE_RADIANS
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace carl;

struct Mouse{
	bool isDown;
	float lastX, lastY;
	float dx, dy;
	float x, y;
	float wheelPos;
};

struct MyApp : public App{

	MyApp() { init(); }
	float timer;

	Mouse mouse;
	glm::quat tmpRot;

	glm::quat camRot;

	meshModels modelA;
	//meshModels modelB;

	void init()
	{
		mouse.wheelPos = -15.0f;
		mouse.isDown = false;

		float vertsX[32];
		float vertsY[32];

	    const double M_PI = 3.14159265;
	    double phi = 2.0 * M_PI / 32;
		float radius = 3.0f;

		for ( int i=0;i<32;i++ )
		{
	        double a = i * phi;
			vertsX[i] = (float)(radius * sin(a));
			vertsY[i] = (float)(radius * cos(a));
		}
	
		modelA.clearModel();
		modelA.cacheCylinder( vertsX, vertsY, 1.0f);

		//modelB.clearModel();
		//modelB.cacheCylinder( vertsX, vertsY, 1.0f);
	}

	virtual void onDraw()
	{
		glm::mat4 camPos = glm::lookAt( glm::vec3(0,0, mouse.wheelPos), glm::vec3(0,0,0), glm::vec3(0,1,0) );

		glm::mat4 rotateCam = glm::toMat4( camRot );
		glm::mat4 view = camPos * rotateCam;

		glm::mat4 proj = glm::perspective( 3.14f / 3.f, (float)window().width()/window().height(), 0.1f,-10.f);

		modelA.DrawPolygon(view, proj);

		//modelB.DrawPolygon(view, proj);
	}

	virtual void onMouseMove(double x, double y)
	{
		//std::cout << x << " " << y << std::endl;
		if (mouse.isDown)
		{
			mouse.x = (float)x;
			mouse.y = (float)y;
			onMouseDrag(mouse.x, mouse.y);
		}
	}

	virtual void onMouseDown(int button, int action)
	{
		if(action == GLFW_PRESS)
		{
			mouse.isDown = true;
			double x,y;
			glfwGetCursorPos(window().window, &x, &y);
			mouse.lastX = (float)x;
			mouse.lastY = (float)y;
			tmpRot = camRot;
		}else{
			mouse.isDown = false;
		}
	}

	virtual void onMouseScroll(double scrollA, double scrollB)
	{
		mouse.wheelPos = mouse.wheelPos + scrollB;
	}

	void onMouseDrag(float x, float y)
	{
		mouse.dx = mouse.x - mouse.lastX;
		mouse.dy = mouse.y - mouse.lastY;

		float wx = mouse.dx / window().width();
		float wy = mouse.dy / window().height();

		glm::vec3 axis = glm::cross( glm::vec3(0,0,1), glm::vec3(wx * 4, wy * 4, 0));
		glm::quat q = glm::angleAxis( glm::length(axis), glm::normalize(axis));
		camRot = q * tmpRot;
	}

	virtual void onKeyDown(int key, int action)
	{
		std::cout <<(unsigned char)key << std::endl;
	}
};

int main()
{
	MyApp app;
	app.start();

	printf("goodbye window\n");

	return 0;
}