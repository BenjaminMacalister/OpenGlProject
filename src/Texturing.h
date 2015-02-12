#ifndef _TEXTURING_H_
#define _TEXTURING_H_
#include "Application.h"
#include "CameraClass.h"


class Texturing : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	void LoadTexture(const char* filename);
	void generateQuad(float size);


	FlyCamera m_Camera;
	unsigned int m_Texture;
	unsigned int m_ProgramID;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_VAO;
	float m_time;
	bool forward;

};
#endif