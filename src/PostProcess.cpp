#include "PostProcess.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"

void PostProcess::generateFrameBuffer()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
	unsigned int indexData[] = {
		0, 1, 2, 0, 2, 3
	};
	glGenVertexArrays(1, &m_quad.m_VAO);

	glGenBuffers(1, &m_quad.m_VBO);
	glGenBuffers(1, &m_quad.m_IBO);

	glBindVertexArray(m_quad.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	//create frame buffer
	generateFrameBuffer();
	//create quad mesh
	generateScreenSpaceQuad();
	//load post effect shader
	LoadShaders("./shaders/postVertex.glsl", nullptr, "./shaders/postFragment.glsl", &m_postProgramID);
	m_time = 0;
	return true;
}

bool PostProcess::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	m_time += dt;
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
	Gizmos::addSphere(vec3(0,0,0), 0.5f, 100, 100, vec4(1, 1, 0, 1));
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
	
	//bind frameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//render everything
	Gizmos::draw(m_Camera.getProjectionView());
	//bind backbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind post effect shader
	glUseProgram(m_postProgramID);

	//bind fboTexture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	int texLocation = glGetUniformLocation(m_postProgramID, "inputTexture");
	glUniform1f(texLocation, 0);
	int timeLocation = glGetUniformLocation(m_postProgramID, "time");
	glUniform1f(timeLocation, m_time);
	//render our quad with texture
	glBindVertexArray(m_quad.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Gizmos::draw(m_Camera.getProjectionView());
	TwDraw();
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}