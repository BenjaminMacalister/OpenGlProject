#include "Application.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include <cstdio>
#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

void OnMouseButton(GLFWwindow* window, int button, int press, int mod_keys)
{
	TwEventMouseButtonGLFW(button, press);
}
void OnMousePosition(GLFWwindow* window, double x, double y)
{
	TwEventMousePosGLFW((int)x, (int)y);
}

void OnMouseScroll(GLFWwindow* window, double x, double y)
{
	TwEventMouseWheelGLFW((int)y);
}

void OnKey(GLFWwindow* window, int key, int scancode, int press, int mod_keys)
{
	TwEventKeyGLFW(key, press);
}

void OnChar(GLFWwindow* window, unsigned int c)
{
	TwEventCharGLFW(c, GLFW_PRESS);
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
	TwWindowSize(width, height);
	glViewport(0, 0, width, height);
}
Application::Application()
{

}

Application::~Application()
{
	TwDeleteAllBars();
	TwTerminate();
}

bool Application::StartUp()
{
	this->m_window = glfwCreateWindow(1280, 720, "OpenGlApplication", nullptr, nullptr);
	if (glfwInit() == 0)
	{
		return false;
	}
	if (this->m_window == nullptr)
	{
		return false;
	}
	glfwSetMouseButtonCallback(m_window, OnMouseButton);
	glfwSetCursorPosCallback(m_window, OnMousePosition);
	glfwSetScrollCallback(m_window, OnMouseScroll);
	glfwSetKeyCallback(m_window, OnKey);
	glfwSetCharCallback(m_window, OnChar);
	glfwSetWindowSizeCallback(m_window, OnWindowResize);
	//this is where the cool code will be
	glfwMakeContextCurrent(this->m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(this->m_window);
		glfwTerminate();
		return false;
	}
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);
	m_bar = TwNewBar("My new Bar");
	TwAddVarRO(m_bar, "FPS", TW_TYPE_FLOAT, &m_fps, "");
	int major_version = ogl_GetMajorVersion();
	int minor_version = ogl_GetMinorVersion();
	printf("successfully loaded OpenGl version %d.%d\n", major_version, minor_version);
	return true;
}

void Application::ShutDown()
{
	glfwDestroyWindow(this->m_window);
	glfwTerminate();
}

bool Application::Update()
{
	if(glfwWindowShouldClose(m_window))
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	m_fps = 1 / dt;
	return true;
}

void Application::Draw(){ TwDraw(); }