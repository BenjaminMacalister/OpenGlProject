#ifndef _ASSIGNMENT1_H_
#define _ASSIGNMENT1_H_

#include "Application.h"
#include "CameraClass.h"
#include "AssignmentLighting.h"
#include "Vertex.h"
#include "RainParticls.h"
#include "AssignmentAnim.h"

class Assignment1 : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	void buildGrid(vec2 realDim, glm::ivec2 dims);
	void buildPerlinTexture(glm::ivec2 dims, int octaves, float persistance);
	Animation m_character;
	RainParticles m_rainParticle;
	AssignLighting m_lighting;
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