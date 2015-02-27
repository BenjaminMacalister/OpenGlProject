#include "Rendering Geometry.h"
#include "glm_header.h"
#include "Vertex.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include "Utility.h"
bool RenderingGeometry::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	generateShader();
	generateGrid(100, 100);
	Gizmos::create();
	m_camera = FlyCamera();
	m_camera.SetSpeed(5.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	time = 0;
	return true;
}


void RenderingGeometry::ShutDown()
{

}

void RenderingGeometry::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	int proj_veiw_handle = glGetUniformLocation(m_ProgramID, "ProjectionVeiw");
	if (proj_veiw_handle >= 0)
	{
		glUniformMatrix4fv(proj_veiw_handle, 1, false, (float*)&m_camera.getProjectionView());
	}
	GLuint _time = glGetUniformLocation(m_ProgramID, "time");
	glUniform1f(_time, time);
	GLuint _hieghtScale = glGetUniformLocation(m_ProgramID, "heightScale");
	glUniform1f(_hieghtScale, 0.5);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	//Gizmos::draw(m_camera.getProjectionView());
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	Application::Draw();
}

bool RenderingGeometry::Update()
{
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
	if (Application::Update() == false)
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	time += 5*dt;
	glfwSetTime(0);

	m_camera.update(dt);

	return true;
}

void RenderingGeometry::generateGrid(unsigned int rows, unsigned int cols)
{

	Vertex* vertex_array = new Vertex[(rows + 1)* (cols + 1)];
	for (unsigned int r = 0; r < rows + 1; r++)
	{
		for (unsigned int c = 0; c < cols + 1; c++)
		{
			vertex_array[c + r*(cols + 1)].position = vec4((float)c, 0, (float)r, 1);
			vertex_array[c + r*(cols + 1)].colour = vec4((float)c / (cols+1), 0, (float)r / (rows+1), 1);

		}
	}

	m_IndexCount = rows * cols * 6;

	unsigned int *index_array = new unsigned int[m_IndexCount];
	int index_location = 0;
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			index_array[index_location + 0] = c + r*(cols+1);
			index_array[index_location + 1] = c + (r + 1)*(cols + 1);
			index_array[index_location + 2] = (c + 1) + r*(cols + 1);
			
			index_array[index_location + 3] = (c + 1) + r*(cols + 1);
			index_array[index_location + 4] = c + (r + 1)*(cols + 1);
			index_array[index_location + 5] = (c + 1) + (r + 1)*(cols + 1);
			index_location += 6;
		}
	}

	//binds the buffers
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);
	
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//adds the data into the allocated slots
	glBufferData(GL_ARRAY_BUFFER, (cols + 1)*(rows + 1)*sizeof(Vertex), vertex_array, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FLOAT, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FLOAT, sizeof(Vertex), (void*)sizeof(vec4));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(unsigned int), index_array, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	delete[] vertex_array;
	delete[] index_array;
}

void RenderingGeometry::generateShader()
{
	LoadShaders("./src/basic_vertex.vs",nullptr,  "./src/basic_fragmant.glsl", &m_ProgramID);
	/*const char * vs_source = "#version 410\n"
		"layout(location=0) in vec4 Position; \n"
		"layout(location=1) in vec4 Colour; \n"
		"out vec4 vColour;\n"
		"uniform mat4 ProjectionVeiw;\n"
		"uniform float time;"
		"uniform float heightScale;"
		"void main() { vec4 nColour;\
		nColour.x = 0;\
		nColour.y = 0;\
		nColour.z = Colour.x +sin(time + Position.x) * heightScale;\
		vColour = Colour;\
						vec4 P = Position;\
		P.y += sin(time + Position.x) * heightScale;\
		P.y += cos(time + Position.z) * heightScale;\
		 gl_Position = ProjectionVeiw * P;
		 }";

	const char * fs_source = "#version 410\n"
		"in vec4 vColour;\n"
		"out vec4 FragColour;\n"
		"void main() {FragColour = vColour;}";

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmant_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertex_shader, 1, &vs_source, 0);
	glCompileShader(vertex_shader);
	glShaderSource(fragmant_shader, 1, &fs_source, 0);
	glCompileShader(fragmant_shader);

	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, vertex_shader);
	glAttachShader(m_ProgramID, fragmant_shader);
	glLinkProgram(m_ProgramID);

	//error checking
	int success = GL_FALSE;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_ProgramID, infoLogLength, 0, infoLog);
		printf("ERROR: STUFFED UP\n\n");
		printf("%s", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmant_shader);
	glDeleteShader(vertex_shader);*/



}