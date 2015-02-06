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


Application::Application()
{

}

Application::~Application()
{

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
	//this is where the cool code will be
	glfwMakeContextCurrent(this->m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(this->m_window);
		glfwTerminate();
		return false;
	}
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
	return true;
}

void Application::Draw(){}