#include "ProceduralGeneration.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "Utility.h"
bool ProceduralGeneration::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	srand(time(0));
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	dims = 128;
	size = 20;
	octaves = 5;
	persistance = 0.2f;
	buildGrid(vec2(size, size), glm::ivec2(dims, dims));
	buildPerlinTexture(glm::ivec2(dims, dims), octaves, persistance);
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(50.0f);
	LoadShaders("./Shaders/perlinVertex.glsl", nullptr, "./Shaders/perlinFragment.glsl", &m_programID);
	TwAddVarRW(m_bar, "Perlin", TW_TYPE_FLOAT, &dims, "");
	TwAddVarRW(m_bar, "Size", TW_TYPE_FLOAT, &size, "");
	TwAddVarRW(m_bar, "Octaves", TW_TYPE_INT32, &octaves, "");
	TwAddVarRW(m_bar, "Persistance", TW_TYPE_FLOAT, &persistance, "");
	glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON_MODE);
	return true;
}

bool ProceduralGeneration::Update()
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
	if(glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		LoadShaders("./Shaders/perlinVertex.glsl", nullptr, "./Shaders/perlinFragment.glsl", &m_programID);
		buildPerlinTexture(glm::ivec2(dims, dims), octaves, persistance);
		buildGrid(vec2(size, size), glm::ivec2(dims, dims));
	}
	m_Camera.update(dt);

	return true;
}
void ProceduralGeneration::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void ProceduralGeneration::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	int viewProjUnif = glGetUniformLocation(m_programID, "viewProj");

	glUniformMatrix4fv(viewProjUnif, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());
	int texUniform = glGetUniformLocation(m_programID, "perlinTexture");
	glUniform1i(texUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glBindVertexArray(m_planeMesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_planeMesh.m_index_count, GL_UNSIGNED_INT, 0);

	Gizmos::draw(m_Camera.getProjectionView());
	TwDraw();

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void ProceduralGeneration::buildGrid(vec2 realDims, glm::ivec2 dims)
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
			indexData[curreIndex++] = (y+1)*(dims.x + 1) + x;
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

void ProceduralGeneration::buildPerlinTexture(glm::ivec2 dims, int octaves, float persistance)
{
	
	//set scale
	float scale = (1.0f / dims.x) * 3.0f;
	//allocate memory for perlin data
	m_perlinData = new float[dims.x * dims.y];
	vec2 offSet;
	offSet.x = rand();
	offSet.y = rand();
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
				float perlinSample = glm::perlin(vec2(x + offSet.x, y+offSet.y)*scale * freq) * 0.5 + 0.5;
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