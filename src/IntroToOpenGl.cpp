#include "IntroToOpenGl.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

mat4 BuildOrbitMatrix(float local_rotation, float speed, float radius, mat4 transform)
{
	mat4 result = glm::rotate(local_rotation*speed, vec3(0, 1, 0)) * transform * glm::translate(vec3(radius, 0, 0));

	return result;
}

bool IntroToOpenGl::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);
	m_projection = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	m_camera_x = -10.0f;
	m_camera_z = -10.0f;
	m_timer = 0.0f;
	return true;


}

void IntroToOpenGl::ShutDown()
{
	Gizmos::destroy();
}
bool IntroToOpenGl::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
		float dt = (float)glfwGetTime();
		glfwSetTime(0.0);
		m_timer += dt;
		m_camera_x = sinf(m_timer) * 10;
		m_camera_z = cosf(m_timer) * 10;
		Gizmos::clear();
		//mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
		Gizmos::addTransform(mat4(1), 10);

		vec4 white(1);
		vec4 black(0, 0, 0, 1);
		vec4 blue(0, 0, 1, 1);
		vec4 red(1, 0, 0, 1);
		vec4 yellow(1, 1, 0, 1);
		vec4 green(0, 1, 0, 1);
		m_Camera.update(dt);
		for (int i = 0; i <= 20; i++)
		{
			Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
			Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
		}

		mat4 suntransform = glm::rotate(m_timer, vec3(0, 1, 0));

		mat4 child_one_transform = glm::rotate(m_timer, vec3(0, 1, 0)) * suntransform* glm::translate(vec3(5, 0, 0));
		mat4 child_one_two_transform = BuildOrbitMatrix(m_timer, 0.5, 10, suntransform);

		mat4 MoonOne = child_one_transform * glm::rotate(m_timer * 10, vec3(0, 1, 1))* glm::translate(vec3(1.5, 0, 0));

		//vec4 center = transform * vec4(0, 0, 0, 1);

		/*Gizmos::addAABB(vec3(0, 0, 0), vec3(10, 1, 1), white);
		Gizmos::addAABBFilled(vec3(0, 0, 0), vec3(10, 1, 1), blue);*/
		Gizmos::addSphere(suntransform[3].xyz, 3.0f, 50, 50, yellow, &suntransform);
		Gizmos::addSphere(child_one_transform[3].xyz, 1.0f, 20, 20, blue, &child_one_transform);
		Gizmos::addSphere(MoonOne[3].xyz, 0.25f, 20, 20, green, &MoonOne);
		Gizmos::addSphere(child_one_two_transform[3].xyz, 1.50f, 20, 20, black, &child_one_two_transform);
	return true;
}

void IntroToOpenGl::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
	//Gizmos::draw(m_projection, view);
	Gizmos::draw(m_Camera.getProjectionView());

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}