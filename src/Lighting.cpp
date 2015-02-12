/*#include "Lighting.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "FBXFile.h"
#include "Utility.h"
bool Lighting::StartUp()
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
	m_FBXFile = new FBXFile();
	m_FBXFile->load("./models/stanford/Bunny.fbx");
	CreateOpenGLBuffers(m_FBXFile);

	LoadShaders("./Shaders/Lighting_Vertex.glsl", "./Shaders/Lighting_Fragmant.glsl", &m_ProgramID);
	return true;
}

void Lighting::reloadShader()
{
	delete &m_ProgramID;
	LoadShaders("./Shaders/Lighting_Vertex.glsl", "./Shaders/Lighting_Fragmant.glsl", &m_ProgramID);
}

bool Lighting::Update()
{
	if (Application::Update() == false)
	{
		return false;
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

	for (int i = 0; i <= 20; i++)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	}
	m_Camera.update(dt);
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_KEY_DOWN)
	{
		reloadShader();
	}

	return true;
}
void Lighting::ShutDown()
{
	CleanUpOpenGLBuffers(m_FBXFile);
	delete m_FBXFile;
	Gizmos::destroy;

	Application::ShutDown();
}
void Lighting::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_Camera.getProjectionView());

	int proj_veiw_uniform = glGetUniformLocation(m_ProgramID, "projection_view");
	glUniformMatrix4fv(proj_veiw_uniform, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());
	for (unsigned int mesh_index = 0; mesh_index < m_FBXFile->getMeshCount(); mesh_index++)
	{
		FBXMeshNode* mesh = m_FBXFile->getMeshByIndex(mesh_index);
		unsigned int *gl_data = (unsigned int*)mesh->m_userData;
		glBindVertexArray(gl_data[0]);
		glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Lighting::CreateOpenGLBuffers(FBXFile* file)
{
	for (unsigned int mesh_index = 0; mesh_index < file->getMeshCount(); mesh_index++)
	{
		FBXMeshNode* mesh = file->getMeshByIndex(mesh_index);

		//contains [0] is VAO, [1] is VBO and [2] is IBO
		unsigned int* gl_data = new unsigned int[3];

		glGenVertexArrays(1, &gl_data[0]);
		glGenBuffers(2, &gl_data[1]);

		glBindVertexArray(gl_data[0]);
		glBindBuffer(GL_ARRAY_BUFFER, gl_data[1]);
		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size()* sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() *sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)(FBXVertex::NormalOffset));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = gl_data;
	}
}

void Lighting::CleanUpOpenGLBuffers(FBXFile* file)
{
	for (unsigned int mesh_index = 0; mesh_index < file->getMeshCount(); mesh_index++)
	{
		FBXMeshNode* mesh = file->getMeshByIndex(mesh_index);

		unsigned int* gl_data = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &gl_data[0]);
		glDeleteBuffers(2, &gl_data[1]);

		delete[] gl_data;

	}
}*/