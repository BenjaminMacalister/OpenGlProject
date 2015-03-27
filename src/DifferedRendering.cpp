#include "DifferedRendering.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "tiny_obj_loader.h"
#include "Utility.h"
bool DifferedRendering::StartUp()
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
	buildMeshes();
	buildGBuffer();
	buildLightBuffer();
	buildQuad();
	buildCube();
	LoadShaders("./Shaders/gBufferVertex.glsl", nullptr, "./Shaders/gBufferFragment.glsl", &m_gBufferProgram);
	LoadShaders("./Shaders/CompositeVertex.glsl", nullptr, "./Shaders/CompositeFragment.glsl", &m_compositeProgram);
	LoadShaders("./Shaders/CompositeVertex.glsl", nullptr, "./Shaders/DirectionalLightFragment.glsl", &m_dirrectionalLightProgram);
	LoadShaders("./Shaders/pointLightVertex.glsl", nullptr, "./Shaders/pointLightFragment.glsl", &m_PointLightProgram);
	m_time = 0;
	glEnable(GL_CULL_FACE);

	
	return true;
}

bool DifferedRendering::Update()
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
void DifferedRendering::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void DifferedRendering::Draw()
{
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFbo);
	glClearColor(0, 0, 0, 0);

	glClear(GL_DEPTH_BUFFER_BIT);

	vec4 clearColour = vec4(0, 0, 0, 0);
	vec4 clearNormal = vec4(0.5f, 0.5f, 0.5f, 0.5f);

	glClearBufferfv(GL_COLOR, 0, (float*)&clearColour);
	glClearBufferfv(GL_COLOR, 1, (float*)&clearColour);
	glClearBufferfv(GL_COLOR, 2, (float*)&clearNormal);
	glUseProgram(m_gBufferProgram);

	int viewUnif = glGetUniformLocation(m_gBufferProgram, "View");
	int viewProjUnif = glGetUniformLocation(m_gBufferProgram, "ProjectionView");

	glUniformMatrix4fv(viewUnif, 1, GL_FALSE, (float*)&m_Camera.getView());
	glUniformMatrix4fv(viewProjUnif, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());

	glBindVertexArray(m_bunny.m_VAO);
	glDrawElements(GL_TRIANGLES, m_bunny.m_index_count, GL_UNSIGNED_INT, 0);


	/////Light accumulation
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glUseProgram(m_dirrectionalLightProgram);


	int positionTexUnif = glGetUniformLocation(m_dirrectionalLightProgram, "positionTex");
	int normalTexUnif = glGetUniformLocation(m_dirrectionalLightProgram, "normalTex");

	glUniform1i(positionTexUnif, 0);
	glUniform1i(normalTexUnif, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_positionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);

	RenderDirectionalLight(vec3(0,-1,0), vec3(1,0,1));
	RenderDirectionalLight(vec3(0, 1, 0), vec3(0, 1, 0));
	RenderDirectionalLight(vec3(1, 0, 0), vec3(0.5f, 0.2f, 0.2f));

	glUseProgram(m_PointLightProgram);
	viewProjUnif = glGetUniformLocation(m_PointLightProgram, "projView");
	positionTexUnif = glGetUniformLocation(m_PointLightProgram, "positionTexture");
	normalTexUnif = glGetUniformLocation(m_PointLightProgram, "normalTexture");

	glUniformMatrix4fv(viewProjUnif, 1, GL_FALSE, (float*)&m_Camera.m_projectionViewTransform);
	glUniform1i(positionTexUnif, 0);
	glUniform1i(normalTexUnif, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_positionTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
	////draw POintLights;
	float dt = (float)glfwGetTime();
	glfwSetTime(0);
	m_time += dt;
	RenderPointLight(vec3(0, 10, 0), 15, vec3(1, 1, 0));
	RenderPointLight(vec3(5, 5, 0), 15, vec3(0, 1, 0));
	RenderPointLight(vec3(-5, 5, 0), 15, vec3(0, 0, 1)); 
	RenderPointLight(vec3(-1, 0, 1), 5, vec3(1, 0, 0));
	RenderPointLight(vec3(0, 1, 5), 5, vec3(0, 1, 1));
	RenderPointLight(vec3(-1, 4,5), 5, vec3(0, 1, 1));
	RenderPointLight(vec3(sinf(m_time) * 5, 3, cosf(m_time) * 5), 5, vec3(1, 1, 1));

	glDisable(GL_BLEND);
	//composite pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_compositeProgram);

	int albedoTexUnif = glGetUniformLocation(m_compositeProgram, "albedoTex");
	/*normalTexUnif = glGetUniformLocation(m_compositeProgram, "NormalTex");
	positionTexUnif = glGetUniformLocation(m_compositeProgram, "PositionTex");*/
	int lightTextureUnif = glGetUniformLocation(m_compositeProgram, "lightTex");

	glUniform1i(albedoTexUnif, 0);
	glUniform1i(lightTextureUnif, 1);
	/*glUniform1i(normalTexUnif, 2);*/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_albedoTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_lightTexture);

	/*glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_lightTexture);*/


	glBindVertexArray(m_screenSpaceQuad.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//Gizmos::draw(m_Camera.getProjectionView());

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}


void DifferedRendering::RenderDirectionalLight(vec3 a_lightDirection, vec3 a_lightColour)
{

	
	vec4 viewSpaceLightDIr = m_Camera.getView() * vec4(a_lightDirection, 0);


	int lightDirectionUnif = glGetUniformLocation(m_dirrectionalLightProgram, "LightDir");
	int lightColourUnif = glGetUniformLocation(m_dirrectionalLightProgram, "lightColour");

	glUniform3fv(lightDirectionUnif, 1, (float*)&viewSpaceLightDIr);
	glUniform3fv(lightColourUnif, 1, (float*)&a_lightColour);

	glBindVertexArray(m_screenSpaceQuad.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void DifferedRendering::RenderPointLight(vec3 a_position, float a_radius, vec3 a_diffuse)
{
	vec4 viewSpacePos = m_Camera.m_viewTransform * vec4(a_position, 1);

	int posUniform = glGetUniformLocation(m_PointLightProgram, "lightPosition");
	int viewPosUniform = glGetUniformLocation(m_PointLightProgram, "lighViewPosition");
	int lightDiffuseUniform = glGetUniformLocation(m_PointLightProgram, "lightDiffuse");
	int lightRadiusUniform = glGetUniformLocation(m_PointLightProgram, "lightRadius");

	glUniform3fv(posUniform, 1, (float*)&a_position);
	glUniform3fv(viewPosUniform, 1, (float*)&viewSpacePos);
	glUniform3fv(lightDiffuseUniform, 1, (float*)&a_diffuse);
	glUniform1f(lightRadiusUniform, a_radius);

	glBindVertexArray(m_lightCube.m_VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void DifferedRendering::buildMeshes()
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<float> vertex_data;
	tinyobj::LoadObj(shapes, materials, "./models/stanford_objs/bunny.obj");

	m_bunny.m_index_count = shapes[0].mesh.indices.size();

	tinyobj::mesh_t* mesh = &shapes[0].mesh;
	vertex_data.reserve(m_bunny.m_index_count);

	vertex_data.insert(vertex_data.end(), mesh->positions.begin(), mesh->positions.end());
	vertex_data.insert(vertex_data.end(), mesh->normals.begin(), mesh->normals.end());


	glGenVertexArrays(1, &m_bunny.m_VAO);
	glBindVertexArray(m_bunny.m_VAO);

	glGenBuffers(1, &m_bunny.m_VBO);
	glGenBuffers(1, &m_bunny.m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_bunny.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bunny.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_bunny.m_index_count, vertex_data.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size()*sizeof(unsigned int), shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //positions
	glEnableVertexAttribArray(1); //normals

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[0].mesh.positions.size()));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DifferedRendering::buildGBuffer()
{
	//create frame buffer
	glGenFramebuffers(1, &m_gBufferFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFbo);
	//generate textures
	glGenTextures(1, &m_albedoTexture);
	glBindTexture(GL_TEXTURE_2D, m_albedoTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &m_positionTexture);
	glBindTexture(GL_TEXTURE_2D, m_positionTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &m_normalTexture);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//albedo position normal depth
	glGenRenderbuffers(1, &m_gBufferDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gBufferDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);

	//attatch our texture to framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_albedoTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_positionTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_normalTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gBufferDepth);

	GLenum targets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, targets);
	
	//check that it worked
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error in gBuffer");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DifferedRendering::buildLightBuffer()
{
	//create fbo
	glGenFramebuffers(1, &m_lightFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFbo);
	//create textures
	//lighttexture
	glGenTextures(1, &m_lightTexture);
	glBindTexture(GL_TEXTURE_2D, m_lightTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//attach textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightTexture, 0);

	GLenum lightTarget = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &lightTarget);

	//check for success
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("error light frameBuffer");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DifferedRendering::buildQuad()
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
	glGenVertexArrays(1, &m_screenSpaceQuad.m_VAO);

	glGenBuffers(1, &m_screenSpaceQuad.m_VBO);
	glGenBuffers(1, &m_screenSpaceQuad.m_IBO);

	glBindVertexArray(m_screenSpaceQuad.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenSpaceQuad.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_screenSpaceQuad.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void DifferedRendering::buildCube()
{
	float vertexData[] =
	{
		-1, -1, 1, 1,
		1, -1, 1, 1,
		1, -1, -1, 1,
		-1, -1, -1, 1,

		-1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, -1, 1,
		-1, 1, -1, 1,
	};
	unsigned int indexData[] = {
		4, 5, 0,
		5, 1, 0,
		5, 6, 1,
		6, 2, 1,
		6, 7, 2,
		7, 3, 2,
		7, 4, 3,
		4, 0 ,3,
		7, 6, 4, 
		6, 5, 4, 
		0, 1, 3, 
		1, 2, 3,
	};
	glGenVertexArrays(1, &m_lightCube.m_VAO);

	glGenBuffers(1, &m_lightCube.m_VBO);
	glGenBuffers(1, &m_lightCube.m_IBO);

	glBindVertexArray(m_lightCube.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lightCube.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lightCube.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 4, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}