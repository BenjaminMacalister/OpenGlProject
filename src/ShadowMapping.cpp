#include "ShadowMapping.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "tiny_obj_loader.h"
#include "Utility.h"
bool Shadows::StartUp()
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

	LoadShaders("./Shaders/ShadowVertex.glsl", nullptr, "./Shaders/ShadowFragment.glsl", &m_diffuseShadowedProgram);
	LoadShaders("./Shaders/ShadowMapVertex.glsl", nullptr, "./Shaders/ShadowMapFragment.glsl", &m_shadowmapProgram);
	buildMeshes();
	return true;
}

bool Shadows::Update()
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
void Shadows::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void Shadows::Draw()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_shadowmapProgram);
	vec3 lightDirection = vec3(0, -1, 0);
	int lightMatrixLocation = glGetUniformLocation(m_shadowmapProgram, "lightMatrix");
	mat4 lightProjMatrix = glm::ortho(-10, 10, -10, 10, -10, 100);
	mat4 lightView = glm::lookAt(-lightDirection, vec3(0), vec3(0, 1, 0));

	mat4 lightMatrix = lightProjMatrix * lightView;
	glUniformMatrix4fv(lightMatrixLocation, 1, GL_FALSE, (float*)&lightMatrix);
	glBindVertexArray(m_bunney.m_VAO);
	glDrawElements(GL_TRIANGLES, m_bunney.m_index_count, GL_UNSIGNED_INT, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_diffuseShadowedProgram);

	mat4 offSetScale = mat4(
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0.5f, 0.5f, 0.5f, 1);
	lightMatrix = offSetScale * lightMatrix;
	int viewProjUni = glGetUniformLocation(m_diffuseShadowedProgram, "viewProj");
	glUniformMatrix4fv(viewProjUni, 1, GL_FALSE, (float*)&m_Camera.m_projectionViewTransform);
	int lightDirUni = glGetUniformLocation(m_diffuseShadowedProgram, "lighDir");
	glUniform3fv(lightDirUni, 1, (float*)&lightDirection);
	int lightMatUni = glGetUniformLocation(m_diffuseShadowedProgram, "lightMatrix");
	glUniformMatrix4fv(lightMatUni, 1, GL_FALSE, (float*)&lightMatrix);
	int ShadowMapLocation = glGetUniformLocation(m_diffuseShadowedProgram, "shadowMap");
	glUniform1i(ShadowMapLocation, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);

	glBindVertexArray(m_bunney.m_VAO);
	glDrawElements(GL_TRIANGLES, m_bunney.m_index_count, GL_UNSIGNED_INT, 0);

	/*glBindVertexArray(m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, m_plane.m_index_count, GL_UNSIGNED_INT, 0);*/
	Gizmos::draw(m_Camera.getProjectionView());

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Shadows::buildMeshes()
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<float> vertex_data;
	tinyobj::LoadObj(shapes, materials, "./models/stanford_objs/bunny.obj");

	m_bunney.m_index_count = shapes[0].mesh.indices.size();

	tinyobj::mesh_t* mesh = &shapes[0].mesh;
	vertex_data.reserve(m_bunney.m_index_count);

	vertex_data.insert(vertex_data.end(), mesh->positions.begin(), mesh->positions.end());
	vertex_data.insert(vertex_data.end(), mesh->normals.begin(), mesh->normals.end());
	

	glGenVertexArrays(1, &m_bunney.m_VAO);
	glBindVertexArray(m_bunney.m_VAO);

	glGenBuffers(1, &m_bunney.m_VBO);
	glGenBuffers(1, &m_bunney.m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_bunney.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bunney.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_bunney.m_index_count, vertex_data.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size()*sizeof(unsigned int), shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //positions
	glEnableVertexAttribArray(1); //normals

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[0].mesh.positions.size()));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float PlaneVertexData[]
	{
		-10, 0, -10, 10,	0, 1, 0, 0,
		10, 0, -10, 10,		0, 1, 0, 0,
		10, 0,  10, 10,		0, 1, 0, 0,
		-10, 0, 10, 10,		0, 1, 0, 0,
	};

	float PlaneIndexData[]
	{
		0, 1, 2, 0, 2, 3
	};
	m_plane.m_index_count = 6;
	glGenVertexArrays(1, &m_plane.m_VAO);

	glGenBuffers(1, &m_plane.m_VBO);
	glGenBuffers(1, &m_plane.m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_plane.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertexData), PlaneVertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneIndexData), PlaneIndexData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0); //positions
	glEnableVertexAttribArray(1); //normals

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*8, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 8, (void*)(sizeof(float)* 4));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



}

void Shadows::buildShadowsMap()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fboDepth);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_fboDepth, 0);

	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}