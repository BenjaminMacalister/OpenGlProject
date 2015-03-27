#ifndef _PROCEDURAL_H_
#define _PROCEDURAL_H_

#include "Application.h"
#include "CameraClass.h"
#include "Vertex.h"

class ProceduralGeneration : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	void buildGrid(vec2 realDim, glm::ivec2 dims);
	void buildPerlinTexture(glm::ivec2 dims, int octaves, float persistance);


	openGLData m_planeMesh;
	unsigned int m_programID;
	unsigned int m_perlinTexture;
	float* m_perlinData;
	FlyCamera m_Camera;
	float dims;
	float size;
	int octaves;
	float persistance;
};



#endif