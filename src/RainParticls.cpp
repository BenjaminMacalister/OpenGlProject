#include "RainParticls.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"
#include "stb_image.h"
bool RainParticles::StartUp()
{

	m_time = 0;
	LoadShaders("./Shaders/gpuParticleVert.glsl", "./Shaders/gpuParticleGeo.glsl", "./Shaders/ParticleFragment.glsl", &m_rainID);
	m_emitter1.Init(10000, vec3(0, 1000, 0), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter2.Init(10000, vec3(500, 1000, 0), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter3.Init(10000, vec3(-500, 1000, 0), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter4.Init(10000, vec3(0, 1000, 500), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter5.Init(10000, vec3(500, 1000, 500), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter6.Init(10000, vec3(-500, 1000, 500), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter7.Init(10000, vec3(0, 1000, -500), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter8.Init(10000, vec3(500, 1000, -500), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);
	m_emitter9.Init(10000, vec3(-500, 1000, -500), 10, 3.0f, 5.0f, 600.0f, 700.6f, 1.0f, 0.0f, vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), 100);


	return true;
}

bool RainParticles::Update()
{

	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	m_time += dt;
	m_emitter1.m_updateShader;
	m_emitter2.m_updateShader;
	m_emitter3.m_updateShader;
	m_emitter4.m_updateShader;
	m_emitter5.m_updateShader;
	m_emitter6.m_updateShader;
	m_emitter7.m_updateShader;
	m_emitter8.m_updateShader;
	m_emitter9.m_updateShader;

	return true;
}
void RainParticles::ShutDown()
{

}
void RainParticles::Draw(FlyCamera a_camera)
{

	m_emitter1.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter2.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter3.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter4.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter5.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter6.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter7.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter8.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
	m_emitter9.Draw(m_time, a_camera.m_WorldTransform, a_camera.getProjectionView());
}