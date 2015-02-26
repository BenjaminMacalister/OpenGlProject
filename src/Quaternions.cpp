#include "Quaternions.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
bool Quaternions::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);

	m_hipFrames[0].position = vec3(0, 5, 0);
	m_hipFrames[0].rotation = glm::quat(vec3(-0.5, 0, 0));

	m_kneeFrames[0].position = vec3(0, -2.5, 0);
	m_kneeFrames[0].rotation = glm::quat(vec3(-1, 0, 0));

	m_ankleFrames[0].position = vec3(0, -2.5, 0);
	m_ankleFrames[0].rotation = glm::quat(vec3(0, 0, 0));


	m_hipFrames[1].position = vec3(0, 5, 0);
	m_hipFrames[1].rotation = glm::quat(vec3(1, 0, 0));

	m_kneeFrames[1].position = vec3(0, -2.5, 0);
	m_kneeFrames[1].rotation = glm::quat(vec3(0, 0, 0));

	m_ankleFrames[1].position = vec3(0, -2.5, 0);
	m_ankleFrames[1].rotation = glm::quat(vec3(1, 0, 0));

	m_footFrame[0].position = vec3(0, -1, 0);
	m_footFrame[0].rotation = glm::quat(vec3(-1, 0, 0));
	m_footFrame[1].position = vec3(0, -1, 0);
	m_footFrame[1].rotation = glm::quat(vec3(1, 0, 0));


	return true;
}

mat4 EvaluateKeyFrames(KeyFrame start, KeyFrame end, float t)
{
	vec3 position = glm::mix(start.position, end.position, t);
	glm::quat rot = glm::slerp(start.rotation, end.rotation, t);

	mat4 result = glm::translate(position)*glm::toMat4(rot);

	return result;
}

bool Quaternions::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	Gizmos::clear();
	m_Camera.setPosition(vec3(2, 2, 2));
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	m_timer += dt*5;
	float sin_wave = sinf(m_timer)*0.5f + 0.5f;
	

	m_hip_bone = EvaluateKeyFrames(m_hipFrames[0], m_hipFrames[1], sin_wave);
	m_knee_bone = EvaluateKeyFrames(m_kneeFrames[0], m_kneeFrames[1], sin_wave);
	m_ankle_bone = EvaluateKeyFrames(m_ankleFrames[0], m_ankleFrames[1], sin_wave);
	m_foot_bone = EvaluateKeyFrames(m_footFrame[0], m_footFrame[1], sin_wave);

	mat4 globalHip = m_hip_bone;
	mat4 globalKnee = globalHip * m_knee_bone;
	mat4 globalAnkle = globalKnee * m_ankle_bone;
	mat4 globalFoot = globalAnkle * m_foot_bone;

	vec3 hipPos = globalHip[3].xyz;
	vec3 kneePos = globalKnee[3].xyz;
	vec3 anklePos = globalAnkle[3].xyz;
	vec3 footPos = globalFoot[3].xyz;

	Gizmos::addAABB(hipPos, vec3(0.5f), vec4(0, 0, 1, 1), &globalHip);
	Gizmos::addAABB(kneePos, vec3(0.5f), vec4(1, 0, 1, 1), &globalKnee);
	Gizmos::addAABB(anklePos, vec3(0.5f), vec4(1, 1, 0, 1), &globalAnkle);
	Gizmos::addAABB(footPos, vec3(0.5f), vec4(1, 1, 0, 1), &globalFoot);

	Gizmos::addLine(hipPos, kneePos, vec4(1, 0, 0,1));
	Gizmos::addLine(kneePos, anklePos, vec4(0, 0, 1, 1));
	Gizmos::addLine(anklePos, footPos, vec4(0, 1, 0, 1));


	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 blue(0, 0, 1, 1);
	vec4 red(1, 0, 0, 1);
	vec4 yellow(1, 1, 0, 1);
	vec4 green(0, 1, 0, 1);

	for (int i = 0; i <= 20; i++)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	}
	m_Camera.update(dt);

	return true;
}
void Quaternions::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void Quaternions::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_Camera.getProjectionView());

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}