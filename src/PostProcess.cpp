#include "PostProcess.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

void PostProcess::generateFrameBuffer()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_FBOTexture, 0);

	glGenRenderbuffers(1, &m_FBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_FBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_FBODepth);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FRAMEBUFFER DID NOT GENERATE PROPERLY");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::generateScreenSpaceQuad()
{
	vec2 halfTexel = 1.0f / (vec2(1280, 720) * 0.5f);
	float vertexData[] =
	{
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,
	};
	glGenVertexArrays(1, &m_quad.m_VAO);

	glGenBuffers(1, &m_quad.m_VBO);
	glGenBuffers(1, &m_quad.m_IBO);

	glBindVertexArray(m_quad.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

}
bool PostProcess::StartUp()
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
	return true;
}

bool PostProcess::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);

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
void PostProcess::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void PostProcess::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_Camera.getProjectionView());
	TwDraw();
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}