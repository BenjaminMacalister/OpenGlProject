#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <cstdio>
#include "Gizmos.h"
#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;


mat4 BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation)
{
	mat4 result = glm::rotate(local_rotation, vec3(0, 1, 0)) *glm::translate(vec3(radius, 0, 0)) * glm::rotate(orbit_rotation, vec3(0, 1, 0));

	return result;
}

int main()
{
	if (glfwInit() == 0)
	{
		return -1;
	}
	vec3 myVec;
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics",
													nullptr, nullptr);
	if (window == nullptr)
	{
		return -2;
	}
	//this is where the cool code will be
	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	int major_version = ogl_GetMajorVersion();
	int minor_version = ogl_GetMinorVersion();
	printf("successfully loaded OpenGl version %d.%d\n", major_version, minor_version);
	//this is where the render code will be
	glClearColor(0.1f, 0.5f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();


	
	mat4 projection = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	float camera_x = -10.0f;
	float camera_z = -10.0f;
	float timer = 0.0f;
	glfwSetTime(0.0);
	while (glfwWindowShouldClose(window) == false)
	{
		float dt = (float)glfwGetTime();
		glfwSetTime(0.0);
		timer += dt;
		camera_x = sinf(timer) * 10;
		camera_z = cosf(timer) * 10;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Gizmos::clear();
		mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
		Gizmos::addTransform(mat4(1), 10);

		vec4 white(1);
		vec4 black(0, 0, 0, 1);
		vec4 blue(0, 0, 1, 1);
		vec4 red(1, 0, 0, 1);

		for (int i = 0; i <= 20; i++)
		{
			Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white :black);
			Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
		}

		mat4 suntransform = glm::rotate(timer, vec3(0, 1, 0));

		mat4 child_one_transform = suntransform* glm::translate(vec3(5, 0, 0)) * glm::rotate(timer, vec3(0,1,0));
		mat4 child_one_two_transform = BuildOrbitMatrix(timer, 10, 10);

		mat4 child_two_transform = child_one_transform * glm::translate(vec3(2, 0, 0))* glm::rotate(timer, vec3(1, 1, 1));

		//vec4 center = transform * vec4(0, 0, 0, 1);

		/*Gizmos::addAABB(vec3(0, 0, 0), vec3(10, 1, 1), white);
		Gizmos::addAABBFilled(vec3(0, 0, 0), vec3(10, 1, 1), blue);*/
		Gizmos::addSphere(suntransform[3].xyz, 3.0f, 20, 20, red, &suntransform);
		Gizmos::addSphere(child_one_transform[3].xyz, 1.0f, 20, 20, blue, &child_one_transform);
		Gizmos::addSphere(child_two_transform[3].xyz, 0.5f, 20, 20, blue, &child_two_transform);
		Gizmos::addSphere(child_one_two_transform[3].xyz, 1.50f, 20, 20, blue, &child_one_two_transform);
		
		Gizmos::draw(projection, view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Gizmos::destroy();



	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}