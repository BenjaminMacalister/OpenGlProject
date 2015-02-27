#include "Particle.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"
#include "stb_image.h"
bool Particle::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(50.0f);
	SpawnMaxLimit = 1000;
	minSpeed = 10;
	maxSpeed = 50;
	startSize = 1;
	endSize = 0.01f;
	SpawnAmount = 100;
	minLife = 5.75f;
	maxLife = 15.5f;
	m_gravity = 50;
	StartPatritcleColour = vec4(1, 0, 0, 0.1);
	endParticleColour = vec4(0.3, 0, 0, 1);
	myEmiter.Init(SpawnMaxLimit, vec3(0, 10, 0), SpawnAmount, minLife, maxLife, minSpeed, maxSpeed, startSize, endSize, StartPatritcleColour, endParticleColour, m_gravity);
	TwAddVarRW(m_bar, "SpawnLimit", TW_TYPE_FLOAT, &SpawnMaxLimit, "min=1");
	TwAddVarRW(m_bar, "MinSpeed", TW_TYPE_FLOAT, &minSpeed, "");
	TwAddVarRW(m_bar, "MaxSpeed", TW_TYPE_FLOAT, &maxSpeed, "");
	TwAddVarRW(m_bar, "StartSize", TW_TYPE_FLOAT, &startSize, "");
	TwAddVarRW(m_bar, "Gravity", TW_TYPE_FLOAT, &m_gravity, "");
	TwAddVarRW(m_bar, "EndSize", TW_TYPE_FLOAT, &endSize, "");
	TwAddVarRW(m_bar, "MinLifeSpan", TW_TYPE_FLOAT, &minLife, "");
	TwAddVarRW(m_bar, "MaxLifeSpan", TW_TYPE_FLOAT, &maxLife, "");
	TwAddVarRW(m_bar, "SpawnAmount", TW_TYPE_INT32, &SpawnAmount, "min=1");
	TwAddVarRW(m_bar, "MinLifeSpan", TW_TYPE_FLOAT, &minLife, "");
	TwAddVarRW(m_bar, "StartColour", TW_TYPE_COLOR4F, &StartPatritcleColour, "");
	TwAddVarRW(m_bar, "EndColour", TW_TYPE_COLOR4F, &endParticleColour, "");
	LoadTexture();
	LoadShaders("./Shaders/ParticleVertex.glsl",nullptr, "./Shaders/ParticleFragment.glsl", &m_ProgramID);
	
	return true;
}

bool Particle::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	if (glfwGetMouseButton(m_window, 0))
	{
		myEmiter.UpdateInit(SpawnMaxLimit, vec3(0, 10, 0), SpawnAmount, minLife, maxLife, minSpeed, maxSpeed, startSize, endSize, StartPatritcleColour, endParticleColour, m_gravity);
	}
	m_Camera.setPosition(vec3(2, 2, 2));
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);

	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 blue(0, 0, 1, 1);
	vec4 red(1, 0, 0, 1);
	vec4 yellow(1, 1, 0, 1);
	vec4 green(0, 1, 0, 1);
	int x = 20;
	for (int i = 0; i <= x; i++)
	{
		Gizmos::addLine(vec3(-x / 2 + i, 0, -x / 2), vec3(-x / 2 + i, 0, x / 2), i == x / 2 ? blue : black);
		Gizmos::addLine(vec3(-x / 2, 0, -x / 2 + i), vec3(x / 2, 0, -x / 2 + i), i == x / 2 ? blue : black);
	}
	m_Camera.update(dt);
	myEmiter.Update(dt);
	myEmiter.UpdateVertexData(m_Camera.m_WorldTransform[3].xyz, m_Camera.m_WorldTransform[1].xyz);


	return true;
}
void Particle::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void Particle::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	int loc = glGetUniformLocation(m_ProgramID, "ProjectionVeiw");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
		&m_Camera.getProjectionView()[0][0]);
	Gizmos::draw(m_Camera.getProjectionView());
	myEmiter.Render(m_ProgramID, m_texture);
	TwDraw();
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Particle::LoadTexture()
{
	int width, height;

	int channels;
	unsigned char* data = stbi_load("./textures/Pokeball.png", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
}