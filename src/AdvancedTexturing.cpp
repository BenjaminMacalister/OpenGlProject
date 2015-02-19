#include "AdvancedTexturing.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Vertex.h"
#include "Utility.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void OnMouseButton(GLFWwindow* window, int button, int press, int mod_keys)
{
	TwEventMouseButtonGLFW(button, press);
}
void OnMousePosition(GLFWwindow* window, double x, double y)
{
	TwEventMousePosGLFW((int)x, (int)y);
}

void OnMouseScroll(GLFWwindow* window, double x, double y)
{
	TwEventMouseWheelGLFW((int)y);
}

void OnKey(GLFWwindow* window, int key, int scancode, int press, int mod_keys)
{
	TwEventKeyGLFW(key, press);
}

void OnChar(GLFWwindow* window, unsigned int c)
{
	TwEventCharGLFW(c, GLFW_PRESS);
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
	TwWindowSize(width, height);
	glViewport(0, 0, width, height);
}


bool AdvancedTexture::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glfwSetMouseButtonCallback(m_window, OnMouseButton);
	glfwSetCursorPosCallback(m_window, OnMousePosition);
	glfwSetScrollCallback(m_window, OnMouseScroll);
	glfwSetKeyCallback(m_window, OnKey);
	glfwSetCharCallback(m_window, OnChar);
	glfwSetWindowSizeCallback(m_window, OnWindowResize);
	m_newColour = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	generateQuad(5.0f);
	LoadTexture();
	LoadShaders("./Shaders/Normal_Map_Vertex.glsl", "./Shaders/Normal_Map_Fragment.glsl", &m_ProgramID);

	m_light_dir = glm::normalize(vec3(-1,-1,0));
	m_light_colour = vec3(1);
	m_ambient_light = vec3(0.1f);
	m_spec_power =15;

	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);
	forward = false;

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	m_bar = TwNewBar("My new Bar");

	TwAddVarRW(m_bar, "Clear Colour", TW_TYPE_COLOR4F, &m_newColour, "");
	TwAddVarRW(m_bar, "Light Colour", TW_TYPE_COLOR3F, &m_light_colour, "group = Light");
	TwAddVarRW(m_bar, "Light Direction", TW_TYPE_DIR3F, &m_light_dir, "group = Light");
	TwAddVarRW(m_bar, "Specular Power", TW_TYPE_FLOAT, &m_spec_power, "group = Light min=0 max=100 step=0.05");

	TwAddVarRW(m_bar, "Draw Gizmos", TW_TYPE_BOOL8, &m_draw_gizmos, "precision=5");

	TwAddVarRO(m_bar, "FPS", TW_TYPE_FLOAT, &m_fps, "");
	m_draw_gizmos = true;


	return true;
}

bool AdvancedTexture::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	glClearColor(m_newColour.x, m_newColour.y, m_newColour.z, m_newColour.w);
	float dt = (float)glfwGetTime();
	m_fps = 1 / dt;
	glfwSetTime(0);
	m_light_dir = (glm::rotate(dt, vec3(0, 1, 0))* vec4(m_light_dir, 0)).xyz;
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
	Gizmos::addLine(m_light_dir, m_light_dir*m_spec_power, vec4(1, 0, 0, 1));
	m_Camera.update(dt);

	return true;
}
void AdvancedTexture::ShutDown()
{
	Gizmos::destroy;
	TwDeleteAllBars();
	TwTerminate();
	Application::ShutDown();
}
void AdvancedTexture::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);

	int proj_veiw_uniform = glGetUniformLocation(m_ProgramID, "projection_view");
	glUniformMatrix4fv(proj_veiw_uniform, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());

	int ambiant_Uniform = glGetUniformLocation(m_ProgramID, "ambient_light");
	int LightDir_Uniform = glGetUniformLocation(m_ProgramID, "light_dir");
	int LightColour_Uniform = glGetUniformLocation(m_ProgramID, "light_colour");
	int cameraPost_Uniform = glGetUniformLocation(m_ProgramID, "eye_pos");
	int specPower_Uniform = glGetUniformLocation(m_ProgramID, "spec_power");



	glUniform3fv(ambiant_Uniform, 1, (float*)&m_ambient_light);
	glUniform3fv(LightDir_Uniform, 1, (float*)&m_light_dir);
	glUniform3fv(LightColour_Uniform, 1, (float*)&m_light_colour);

	vec3 camera_pos = m_Camera.m_WorldTransform[3].xyz;
	glUniform3fv(cameraPost_Uniform, 1, (float*)&camera_pos);
	glUniform1f(specPower_Uniform, m_spec_power);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	glBindVertexArray(m_quad.m_VAO);
	glDrawElements(GL_TRIANGLES, m_quad.m_index_count, GL_UNSIGNED_INT, 0);

	int diffuse_location = glGetUniformLocation(m_ProgramID, "diffuse_tex");
	int normal_tex_location = glGetUniformLocation(m_ProgramID, "normal_tex");
	int spec_tex_location = glGetUniformLocation(m_ProgramID, "specular_tex");

	glUniform1i(diffuse_location, 0);
	glUniform1i(normal_tex_location, 1);
	glUniform1i(spec_tex_location, 2);
	if (m_draw_gizmos == true)
	{
		Gizmos::draw(m_Camera.getProjectionView());
	}
	TwDraw();
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
	Gizmos::clear();
}

void AdvancedTexture::LoadTexture()
{
	int width, height;

	int channels;
	unsigned char* data = stbi_load("./textures/rock_diffuse.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_diffuse_texture);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load("./textures/four_normal.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_normal_texture);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load("./textures/four_specular.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_specular_texture);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
}

void AdvancedTexture::generateQuad(float size)
{
	VertexNormal vertex_data[4];

	vertex_data[0].position = vec4(-size, 0, -size, 1);
	vertex_data[1].position = vec4(-size, 0, size, 1);
	vertex_data[2].position = vec4(size, 0, size, 1);
	vertex_data[3].position = vec4(size, 0, -size, 1);

	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[1].normal = vec4(0, 1, 0, 0);
	vertex_data[2].normal = vec4(0, 1, 0, 0);
	vertex_data[3].normal = vec4(0, 1, 0, 0);

	vertex_data[0].tangent = vec4(1,0,0,0);
	vertex_data[1].tangent = vec4(1,0,0,0);
	vertex_data[2].tangent = vec4(1,0,0,0);
	vertex_data[3].tangent = vec4(1,0,0,0);
	

	vertex_data[0].tex_coord = vec2(0, 0);
	vertex_data[1].tex_coord = vec2(0, 1);
	vertex_data[2].tex_coord = vec2(1, 1);
	vertex_data[3].tex_coord = vec2(1, 0);

	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };
	m_quad.m_index_count = 6;
	glGenVertexArrays(1, &m_quad.m_VAO);

	glGenBuffers(1, &m_quad.m_VBO);
	glGenBuffers(1, &m_quad.m_IBO);

	glBindVertexArray(m_quad.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); //normal
	glEnableVertexAttribArray(2); //tangent
	glEnableVertexAttribArray(3); //texture coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0); //position

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VertexNormal), (void*)sizeof(vec4)); //normal

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(VertexNormal), (void*)(sizeof(vec4)* 2));//tangent

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)(sizeof(vec4)* 3));//tex_coord

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




}