#include "GPUParticle.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"
#include "stb_image.h"
bool GPUParticles::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(50.0f);
	m_time = 0;
	LoadShaders("./Shaders/ParticleVertex.glsl", "./Shaders/gpuParticleGeo.glsl", "./Shaders/ParticleFragment.glsl", &m_ProgramID);
	m_emitter.Init(10000000, vec3(0, 0, 0), 10, 1, 1.5f, 0.06f, 0.6f, 0.03f, 0.0f, vec4(0, 1, 0, 0), vec4(0, 0.5f , 1, 0), 10);
	return true;
}

bool GPUParticles::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	m_time += dt;
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
	m_emitter.m_updateShader;
	m_Camera.update(dt);

	return true;
}
void GPUParticles::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void GPUParticles::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_Camera.getProjectionView());
	m_emitter.Draw(m_time, m_Camera.m_WorldTransform, m_Camera.getProjectionView());
	TwDraw();
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}