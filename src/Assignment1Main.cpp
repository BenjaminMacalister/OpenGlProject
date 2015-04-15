#include "Assignment1Main.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"
#include "stb_image.h"
bool Assignment1::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	//srand(time(0));
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	m_Bar2 = TwNewBar("RainBar");
	Gizmos::create();
	m_rotateLight = false;
	m_raining = true;
	dims = 128;
	size = 2000;
	m_speedCamera = 500.0f;
	rainColour = vec4(0, 0, 1, 1);
	rainSize = 1;
	octaves = 5;
	persistance = 0.2f;
	buildGrid(vec2(size, size), glm::ivec2(dims, dims));
	buildPerlinTexture(glm::ivec2(dims, dims), octaves, persistance);
	m_Camera = FlyCamera();
	m_rainParticle = RainParticles();
	m_lighting = AssignLighting();
	m_character = Animation();
	m_character2 = Animation();
	m_Camera.SetSpeed(m_speedCamera);
	m_rainParticle.StartUp();
	m_lighting.StartUp();
	m_character.StartUp("./models/characters/Enemyelite/EnemyElite.fbx", vec2(2.7,4.8));
	m_character2.StartUp("./models/characters/Marksman/Marksman.fbx", vec2(18.3,22));
	LoadShaders("./Shaders/perlinVertex.glsl", nullptr, "./Shaders/perlinFragmentTextured.glsl", &m_programID);
	m_lightDirection = vec3(-3, -2.5, 0);
	LoadTexture();
	TwAddVarRW(m_bar, "Perlin", TW_TYPE_FLOAT, &dims, "group=Plain max=400");
	TwAddVarRW(m_bar, "Size", TW_TYPE_FLOAT, &size, "group=Plain");
	TwAddVarRW(m_bar, "Octaves", TW_TYPE_INT32, &octaves, "group=Plain");
	TwAddVarRW(m_bar, "Persistance", TW_TYPE_FLOAT, &persistance, "group=Plain");
	TwAddVarRW(m_bar, "CameraSpeed", TW_TYPE_FLOAT, &m_speedCamera, "");
	TwAddVarRW(m_bar, "lightDirection", TW_TYPE_DIR3F, &m_lightDirection, "");
	TwAddVarRW(m_bar, "LightRotation", TW_TYPE_BOOL32, &m_rotateLight, "");
	TwAddVarRW(m_Bar2, "Raining", TW_TYPE_BOOL32, &m_raining, "group=Rain");
	TwAddVarRW(m_Bar2, "RainingColour", TW_TYPE_COLOR4F, &rainColour, "");
	TwAddVarRW(m_Bar2, "RainSize", TW_TYPE_FLOAT, &rainSize, "");

	glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON_MODE);
	return true;
}

bool Assignment1::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	if (m_rotateLight == true)
	{
		m_lightDirection = (glm::rotate(dt, vec3(0, 1, 0))* vec4(m_lightDirection, 0)).xyz;
	}
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
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		LoadShaders("./Shaders/perlinVertex.glsl", nullptr, "./Shaders/perlinFragmentTextured.glsl", &m_programID);
		buildPerlinTexture(glm::ivec2(dims, dims), octaves, persistance);
		buildGrid(vec2(size, size), glm::ivec2(dims, dims));
		m_Camera.SetSpeed(m_speedCamera);
	}
	m_lighting.Update();
	m_Camera.update(dt);
	m_character.Update(dt);
	m_character2.Update(dt);
	m_rainParticle.Update();
	return true;
}
void Assignment1::ShutDown()
{
	m_rainParticle.ShutDown();
	m_character.ShutDown();
	m_character2.ShutDown();
	Gizmos::destroy();
	Application::ShutDown();
}
void Assignment1::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);

	int viewProjUnif = glGetUniformLocation(m_programID, "viewProj");

	glUniformMatrix4fv(viewProjUnif, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());

	int worldProjUnif = glGetUniformLocation(m_programID, "worldProj");

	glUniformMatrix4fv(worldProjUnif, 1, GL_FALSE, (float*)&m_Camera.m_WorldTransform);
	int texUniform = glGetUniformLocation(m_programID, "perlinTexture");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_RockTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_WaterTexture);
	int rock_location = glGetUniformLocation(m_programID, "RockTexture");
	int grass_location = glGetUniformLocation(m_programID, "GrassTexture");
	int water_location = glGetUniformLocation(m_programID, "WaterTexture");
	int m_lightDirectionUniform = glGetUniformLocation(m_programID, "m_lightDirection");
	glUniform3fv(m_lightDirectionUniform, 1, (float*)&m_lightDirection);
	glUniform1i(rock_location, 1);
	glUniform1i(grass_location, 2);
	glUniform1i(water_location, 3);
	glUniform1i(texUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glBindVertexArray(m_planeMesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_planeMesh.m_index_count, GL_UNSIGNED_INT, 0);
	m_character.Draw(m_Camera, m_lightDirection, vec3(30000, 16000, -40000));
	m_character2.Draw(m_Camera, m_lightDirection, vec3(-3000, 30000, -30000));
	if (m_raining == true)
	{
		m_rainParticle.Draw(m_Camera, rainColour, rainSize);
	}
	m_lighting.Draw(m_Camera);
	Gizmos::draw(m_Camera.getProjectionView());
	TwDraw();

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Assignment1::buildGrid(vec2 realDims, glm::ivec2 dims)
{
	//compute how many verticies we need
	unsigned int vertexCount = (dims.x + 1) * (dims.y + 1);
	//allocate vetex data
	VertexTexCoord* vertexData = new VertexTexCoord[vertexCount];

	//compute how many indicies we need
	unsigned int indexCount = dims.x * dims.y * 6;
	//allocate index data
	unsigned int* indexData = new unsigned int[indexCount];


	float currY = 0;
	//two nested for loops
	for (unsigned int y = 0; y < dims.y + 1; ++y)
	{
		float currX = 0;
		for (unsigned int x = 0; x < dims.x + 1; x++)
		{
			//inside create points
			vertexData[y*(dims.x + 1) + x].position = vec4(currX - (realDims.x / 2), 0, currY - (realDims.y / 2), 1);
			vertexData[y*(dims.x + 1) + x].tex_coord = vec2((float)x / dims.x, (float)y / dims.y);
			currX += realDims.x / (float)dims.x;
		}
		currY += realDims.y / (float)dims.y;
	}
	//generate indexData
	int curreIndex = 0;
	for (int y = 0; y < dims.y; y++)
	{
		for (int x = 0; x < dims.x; x++)
		{
			//create our 6 indicies here
			indexData[curreIndex++] = y*(dims.x + 1) + x;
			indexData[curreIndex++] = (y + 1)*(dims.x + 1) + x;
			indexData[curreIndex++] = (y + 1)*(dims.x + 1) + x + 1;

			indexData[curreIndex++] = (y + 1)*(dims.x + 1) + x + 1;
			indexData[curreIndex++] = y*(dims.x + 1) + x + 1;;
			indexData[curreIndex++] = y*(dims.x + 1) + x;
		}
	}
	//create vertexArrayObject, buffers, etc
	m_planeMesh.m_index_count = indexCount;
	glGenVertexArrays(1, &m_planeMesh.m_VAO);
	glGenBuffers(1, &m_planeMesh.m_VBO);
	glGenBuffers(1, &m_planeMesh.m_IBO);
	//bind and fill buffers
	glBindVertexArray(m_planeMesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_planeMesh.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeMesh.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexCoord)*vertexCount, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indexCount, indexData, GL_STATIC_DRAW);
	//tell openGL about vertex structure
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexTexCoord), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexCoord), (void*)sizeof(vec4));

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] vertexData;
	delete[] indexData;

}

void Assignment1::buildPerlinTexture(glm::ivec2 dims, int octaves, float persistance)
{

	//set scale
	float scale = (1.0f / dims.x) * 3.0f;
	//allocate memory for perlin data
	m_perlinData = new float[dims.x * dims.y];
	vec2 offSet;
	offSet.x = 10;
	offSet.y = 10;
	//loop through the pixles
	for (int y = 0; y < dims.y; y++)
	{
		for (int x = 0; x < dims.x; x++)
		{
			//call glm::perlinFunction
			float amplitude = 1;
			float freq = 1;
			m_perlinData[y*dims.x + x] = 0;
			for (int o = 0; o < octaves; o++)
			{
				float perlinSample = glm::perlin(vec2(x + offSet.x, y + offSet.y)*scale * freq) * 0.5 + 0.5;
				perlinSample *= amplitude;
				m_perlinData[y*dims.x + x] += perlinSample;
				amplitude *= persistance;
				freq *= 2;
			}
		}
	}

	//generate openGL texture handles
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	//pass perlin data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims.x, dims.y, 0, GL_RED, GL_FLOAT, m_perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Assignment1::LoadTexture()
{
	int width, height;

	int channels;
	unsigned char* data = stbi_load("./textures/Land/RockTexture.jpg", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_RockTexture);
	glBindTexture(GL_TEXTURE_2D, m_RockTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load("./textures/Land/GrassTexture.jpg", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_GrassTexture);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load("./textures/Land/WaterTexture.jpg", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_WaterTexture);
	glBindTexture(GL_TEXTURE_2D, m_WaterTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
}