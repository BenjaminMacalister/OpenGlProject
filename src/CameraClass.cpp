#include "CameraClass.h"
#include "GLFW/glfw3.h"

Camera::Camera()
{
	m_viewTransform = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_projectionTransform = glm::perspective(glm::radians(60.0f),16 / 9.f, 0.1f, 1000.f);	m_WorldTransform = glm::inverse(m_viewTransform);	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}

bool Camera::update()
{
	this->updateProjectionViewTransform();
	return true;
}

void Camera::setPosition(vec3 a_Position)
{
	m_viewTransform = glm::lookAt(a_Position, vec3(0), vec3(0, 1, 0));
}

bool Camera::updateProjectionViewTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
	return true;
}

void Camera::SetPerspective(float a_FOV, float a_AR, float a_N, float a_f)
{
	m_projectionTransform = glm::perspective(a_FOV, a_AR, a_N, a_f);
}
void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, up);
	m_WorldTransform = glm::inverse(m_viewTransform);
	updateProjectionViewTransform();
}

mat4 Camera::getProjection()
{
	return m_projectionTransform;
}

mat4 Camera::getProjectionView()
{
	return m_projectionViewTransform;
}

mat4 Camera::getView()
{
	return m_viewTransform;
}

mat4 Camera::getWorldTransform()
{
	return m_WorldTransform;
}

Camera::~Camera()
{

}



bool FlyCamera::update(float a_DT)
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		m_WorldTransform[3] -= m_WorldTransform[2] * m_speed * a_DT;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		m_WorldTransform[3] += m_WorldTransform[2] * m_speed * a_DT;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		m_WorldTransform[3] -= m_WorldTransform[0] * m_speed * a_DT;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		m_WorldTransform[3] += m_WorldTransform[0] * m_speed * a_DT;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_WorldTransform[3] -= m_WorldTransform[1] * m_speed * a_DT;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
	{
		m_WorldTransform[3] += m_WorldTransform[1] * m_speed * a_DT;
	}
	double x_delta, y_delta;
	GLFWwindow* m_window = glfwGetCurrentContext();
	glfwGetCursorPos(m_window, &x_delta, &y_delta);

	x_delta -= (1280.0f / 2.0f);
	y_delta -= (720.0f / 2.0f);

	x_delta /= (1280.0f / 2.0f);
	y_delta /= (720.0f / 2.0f);

	x_delta *= -100*a_DT;
	y_delta *= -100*a_DT;

	if (glfwGetMouseButton(m_window, 0))
	{
		glfwSetCursorPos(m_window, 1280.0f / 2.0f, 720.0f / 2.0f);
		vec3 camera_Right = (vec3)m_WorldTransform[0];

		mat4 yaw = glm::rotate((float)x_delta, vec3(0, 1, 0));
		mat4 pitch = glm::rotate((float)y_delta, camera_Right);

		mat4 rotation = yaw * pitch;

		m_WorldTransform[0] = rotation * m_WorldTransform[0];
		m_WorldTransform[1] = rotation * m_WorldTransform[1];
		m_WorldTransform[2] = rotation * m_WorldTransform[2];
	}
	//m_WorldTransform[3][3] = 1;
	m_viewTransform = glm::inverse(m_WorldTransform);
	this->updateProjectionViewTransform();
	return true;
}

void FlyCamera::SetSpeed(float a_Speed)
{
	this->m_speed = a_Speed;
}