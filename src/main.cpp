#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/shaders.hpp"
#include "draw_scene.hpp"
#include "grid.hpp"

#include <iostream>
#include <fstream>


using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1600;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "Railroad and Train";
static float aspectRatio = 2.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
static const float GL_VIEW_SIZE = 200.;


/// Camera parameters
float angle_phy {30.0};      // Angle between z axis and viewpoint
float camera_x {10.0};
float camera_y {10.0};
float camera_z {10.0};

bool move_up = false;
bool move_down = false;
bool move_front = false;
bool move_back = false;
bool move_right = false;
bool move_left = false;
bool turn_left = false;
bool turn_right = false;


/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int /*width*/, int /*height*/)
{
	myEngine.set3DProjection(90.0, aspectRatio, Z_NEAR, Z_FAR);
}

void onKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_L:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLFW_KEY_P:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GLFW_KEY_F:
			move_up = true;
			break;
		case GLFW_KEY_D:
			move_down = true;
			break;
		case GLFW_KEY_UP:
			move_front = true;
			break;
		case GLFW_KEY_DOWN:
			move_back = true;
			break;
		case GLFW_KEY_RIGHT:
			move_right = true;
			break;
		case GLFW_KEY_LEFT:
			move_left = true;
			break;
		case GLFW_KEY_C:
			turn_left = true;
			break;
		case GLFW_KEY_V:
			turn_right = true;
			break;
		}
	} 

	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_F:
			move_up = false;
			break;
		case GLFW_KEY_D:
			move_down = false;
			break;
		case GLFW_KEY_UP:
			move_front = false;
			break;
		case GLFW_KEY_DOWN:
			move_back = false;
			break;
		case GLFW_KEY_RIGHT:
			move_right = false;
			break;
		case GLFW_KEY_LEFT:
			move_left = false;
			break;
		case GLFW_KEY_C:
			turn_left = false;
			break;
		case GLFW_KEY_V:
			turn_right = false;
			break;
		}
	}
}

int main(int /*argc*/, char ** /*argv*/)
{

    std::ifstream i("../grids/grid1.json");

    auto j = json::parse(i);
    auto grid = j.template get<Grid>();
	

	/* GLFW initialisation */
	GLFWwindow *window;
	if (!glfwInit())
		return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout << "Loading GL extension" << std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);

	std::cout << "Engine init" << std::endl;
	myEngine.mode2D = false; // Set engine to 3D mode
	myEngine.initGL();
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	CHECK_GL;

	initScene();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		/* Fix camera position */
		myEngine.mvMatrixStack.loadIdentity();

		if (move_up)
			camera_z += 0.5;
		if (move_down)
			camera_z -= 0.5;
		if (move_front){
			camera_x += cos(deg2rad(angle_phy));
			camera_y += sin(deg2rad(angle_phy));
		}
		if (move_back){
			camera_x -= cos(deg2rad(angle_phy));
			camera_y -= sin(deg2rad(angle_phy));
		}
		if (move_right){
			camera_x += cos(deg2rad(angle_phy - 90));
			camera_y += sin(deg2rad(angle_phy - 90));
		}
		if (move_left){
			camera_x -= cos(deg2rad(angle_phy - 90));
			camera_y -= sin(deg2rad(angle_phy - 90));
		}
		if (turn_left)
			angle_phy += 2.0;
		if (turn_right)
			angle_phy -= 2.0;
	
		Vector3D pos_camera =
			Vector3D(camera_x, camera_y, camera_z);
		Vector3D viewed_point = Vector3D(camera_x + cos(deg2rad(angle_phy)), camera_y + sin(deg2rad(angle_phy)), camera_z);
		Vector3D up_vector = Vector3D(0.0, 0.0, 1.0);
		Matrix4D viewMatrix = Matrix4D::lookAt(pos_camera, viewed_point, up_vector);
		myEngine.setViewMatrix(viewMatrix);
		myEngine.updateMvMatrix();

		drawScene(grid);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if (elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
			elapsedTime = glfwGetTime() - startTime;
		}
	}

	glfwTerminate();
	return 0;
}
