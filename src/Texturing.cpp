#include "Texturing.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Vertex.h"
#include "Utility.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
bool Texturing::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	LoadTexture("./textures/crate.png");
	generateQuad(5.0f);
	LoadShaders("./Shaders/Texture_Vertex.glsl", "./Shaders/Texture_Fragmant.glsl", &m_ProgramID);
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);
	forward = false;
	return true;
}

bool Texturing::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	if (forward == true)
	{
		m_time += 5 * dt;
	}
	else
	{
		m_time -= 5 * dt;
	}
	if (m_time > 10)
	{
		forward = false;
	}
	if (m_time < 0)
	{
		forward = true;
	}
	glfwSetTime(0);
	m_Camera.setPosition(vec3(2, 2, 2));

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
void Texturing::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void Texturing::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	int view_proj_uniform = glGetUniformLocation(m_ProgramID, "ProjectionVeiw");
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());
	//bind texture to texturew spot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	int diffuse_location = glGetUniformLocation(m_ProgramID, "diffuse");

	glUniform1i(diffuse_location, 0);
	GLuint _time = glGetUniformLocation(m_ProgramID, "time");
	glUniform1f(_time, m_time);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	Gizmos::draw(m_Camera.getProjectionView());

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Texturing::LoadTexture(const char* fileName)
{
	int width,height;

	int channels;
	unsigned char* data = stbi_load(fileName, &width, &height, &channels, STBI_default);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

void Texturing::generateQuad(float size)
{
	VertexTexCoord vertex_data[4];

	vertex_data[0].position = vec4(-size, 0, -size, 1);
	vertex_data[1].position = vec4(-size, 0, size, 1);
	vertex_data[2].position = vec4(size, 0, size, 1);
	vertex_data[3].position = vec4(size, 0, -size, 1);

	vertex_data[0].tex_coord = vec2(0, 0);
	vertex_data[1].tex_coord = vec2(0, 1);
	vertex_data[2].tex_coord = vec2(1, 1);
	vertex_data[3].tex_coord = vec2(1, 0);

	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };
	glGenVertexArrays(1, &m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexCoord)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); //texture coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexTexCoord), 0); //position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexCoord), (void*)sizeof(vec4)); //texture coord

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




}