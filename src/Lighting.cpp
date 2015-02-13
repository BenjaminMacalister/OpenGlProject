#include "Lighting.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

#include "Utility.h"
bool Lighting::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);

	

	LoadShaders("./Shaders/Lighting_Vertex.glsl", "./Shaders/Lighting_Fragmant.glsl", &m_ProgramID);
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, "./Models/stanford_objs/dragon.obj");
	if (err.size() != 0)
	{
		return false;
	}
	CreateOpenGLBuffers(shapes);
	m_ambient_light = vec3(0.1f);
	m_light_dir = vec3(0, -1, 0);
	m_light_colour = vec3(0.6, 0.3, 1);
	m_material_colour = vec3(1,1,1);
	m_spec_power = 15;

	return true;
}

void Lighting::reloadShader()
{
	glDeleteProgram(m_ProgramID);
	LoadShaders("./Shaders/Lighting_Vertex.glsl", "./Shaders/Lighting_Fragmant.glsl", &m_ProgramID);
}

bool Lighting::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	//m_Camera.setPosition(vec3(2, 2, 2));
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	m_timer += dt;
	//m_light_dir.x = 3 *cos(dt);
	m_light_dir.x = sinf(m_timer) * 10;
	m_light_dir.z = cosf(m_timer) * 10;
	//m_camera_z = cosf(m_timer) * 10;

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
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		reloadShader();
		m_spec_power += 10;
	}
	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)
	{
		m_spec_power -= 10;
	}

	return true;
}
void Lighting::ShutDown()
{
	CleanUpOpenGLBuffers();
	Gizmos::destroy;

	Application::ShutDown();
}
void Lighting::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	Gizmos::draw(m_Camera.getProjectionView());

	int proj_veiw_uniform = glGetUniformLocation(m_ProgramID, "projection_view");
	glUniformMatrix4fv(proj_veiw_uniform, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());

	int ambiant_Uniform = glGetUniformLocation(m_ProgramID, "ambient_light");
	int LightDir_Uniform = glGetUniformLocation(m_ProgramID, "light_dir");
	int LightColour_Uniform = glGetUniformLocation(m_ProgramID, "light_colour");
	int MatirialColour_Uniform = glGetUniformLocation(m_ProgramID, "material_colour");
	int cameraPost_Uniform = glGetUniformLocation(m_ProgramID, "eye_pos");
	int specPower_Uniform = glGetUniformLocation(m_ProgramID, "spec_power");
	
	glUniform3fv(ambiant_Uniform, 1, (float*)&m_ambient_light);
	glUniform3fv(LightDir_Uniform, 1, (float*)&m_light_dir);
	glUniform3fv(LightColour_Uniform, 1, (float*)&m_light_colour);
	glUniform3fv(MatirialColour_Uniform, 1, (float*)&m_material_colour);

	vec3 camera_pos = m_Camera.m_WorldTransform[3].xyz;
	glUniform3fv(cameraPost_Uniform, 1, (float*)&camera_pos);
	glUniform1f(specPower_Uniform, m_spec_power);

	for (unsigned int mesh_index = 0; mesh_index < m_glData.size(); mesh_index++)
	{
		/*FBXMeshNode* mesh = m_FBXFile->getMeshByIndex(mesh_index);
		unsigned int *gl_data = (unsigned int*)mesh->m_userData;*/
		glBindVertexArray(m_glData[mesh_index].m_VAO);
		glDrawElements(GL_TRIANGLES, m_glData[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Lighting::CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	m_glData.resize(shapes.size());

	for (unsigned int shape_index = 0; shape_index < shapes.size(); shape_index++)
	{
		std::vector<float> vertex_data;

		unsigned int float_count = shapes[shape_index].mesh.positions.size();
		float_count += shapes[shape_index].mesh.normals.size();

		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), shapes[shape_index].mesh.positions.begin(), shapes[shape_index].mesh.positions.end());

		vertex_data.insert(vertex_data.end(), shapes[shape_index].mesh.normals.begin(), shapes[shape_index].mesh.normals.end());

		m_glData[shape_index].m_index_count = shapes[shape_index].mesh.indices.size();

		glGenVertexArrays(1, &m_glData[shape_index].m_VAO);
		glGenBuffers(1, &m_glData[shape_index].m_VBO);
		glGenBuffers(1, &m_glData[shape_index].m_IBO);

		glBindVertexArray(m_glData[shape_index].m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER,m_glData[shape_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*float_count, vertex_data.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glData[shape_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[shape_index].mesh.indices.size()*sizeof(unsigned int), shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //positions
		glEnableVertexAttribArray(1); //normals

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[shape_index].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
}

void Lighting::CleanUpOpenGLBuffers()
{
	glDeleteProgram(m_ProgramID);
	for (unsigned int i = 0; i < m_glData.size(); i++)
	{
		glDeleteVertexArrays(1, &m_glData[i].m_VAO);
		glDeleteBuffers(1, &m_glData[i].m_VBO);
		glDeleteBuffers(1, &m_glData[i].m_IBO);

	}
}