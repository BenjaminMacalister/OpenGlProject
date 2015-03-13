#include "RenderTargets.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"
bool RenderTarget::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	GenerateFrameBuffer();
	GeneratePlane();
	LoadShaders("./shaders/Texture_Vertex.glsl", nullptr, "./shaders/Texture_Fragmant.glsl", &m_programID);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);
	m_mirrorCamera = Camera();
	m_mirrorCamera.setPosition(vec3(0, 5, -5));
	m_mirrorCamera.setLookAt(m_mirrorCamera.m_WorldTransform[3].xyz, m_Camera.m_WorldTransform[3].xyz, vec3(0, 1, 0));
	return true;
}

bool RenderTarget::Update()
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
	Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 100, 10, vec4(1, 1, 0, 1));
	m_Camera.update(dt);

	return true;
}
void RenderTarget::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void RenderTarget::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, 512, 512);
	glClearColor(0.7f, 0.7f, 0.7f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 tempWorld = m_Camera.m_WorldTransform;
	vec4 plane = vec4(0, 0, 1, -5);
	vec3 reflected = glm::reflect(-tempWorld[2].xyz(), plane.xyz());
	reflected = glm::normalize(reflected);

	float dist = (glm::dot((plane.xyz()* plane.w) - tempWorld[3].xyz(), plane.xyz())) / (glm::dot(plane.xyz(), -tempWorld[2].xyz()));

	vec3 inter = tempWorld[3].xyz() - tempWorld[2].xyz() * dist;

	tempWorld[3].xyz = inter - reflected * dist;

	vec3 up = vec3(0, 1, 0);
	tempWorld[2].xyz = -reflected;
	tempWorld[0].xyz = glm::normalize(glm::cross(tempWorld[2].xyz(), up));
	tempWorld[1].xyz = glm::normalize(glm::cross(tempWorld[0].xyz(), tempWorld[2].xyz()));

	mat4 View = glm::inverse(tempWorld);
	mat4 projView = m_Camera.m_projectionTransform * View;

	glUseProgram(m_programID);
		int ProjViewUnif = glGetUniformLocation(m_programID, "ProjectionVeiw");
		glUniformMatrix4fv(ProjViewUnif, 1, GL_FALSE, (float*)&projView);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glBindVertexArray(m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	
	glDepthFunc(GL_GREATER);
	Gizmos::draw(projView);
	glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	glClearColor(0.3f, 0.3f, 0.3f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);

	

	int reflectProjViewUnif = glGetUniformLocation(m_programID, "reflectedUniformProj");
	glUniformMatrix4fv(reflectProjViewUnif, 1, GL_FALSE, (float*)&projView);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);

	int diffuseUniform = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(diffuseUniform, 0);

	glBindVertexArray(m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	Gizmos::draw(m_Camera.getProjectionView());
	TwDraw();
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void RenderTarget::GenerateFrameBuffer()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 512, 512);
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

void RenderTarget::GeneratePlane()
{
	float VertexData[] = {
		-5, 0, -5, 1, 0, 0,
		5, 0, -5, 1, 1, 0,
		5, 10, -5, 1, 1, 1,
		-5, 10, -5, 1, 0, 1,
	};
	unsigned int IndexData[] = {
		0, 1, 2, 0, 2, 3
	};
	m_plane.m_index_count = 6;
	glGenVertexArrays(1, &m_plane.m_VAO);
	glBindVertexArray(m_plane.m_VAO);
	glGenBuffers(1, &m_plane.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_plane.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData),
		VertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_plane.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData),
		IndexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(float)* 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(float)* 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}