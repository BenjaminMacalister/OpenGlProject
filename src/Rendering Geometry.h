#ifndef _RENDERINGGEO_H_
#define _RENDERINGGEO_H_
#include "Application.h"
#include "CameraClass.h"


class RenderingGeometry : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	void generateGrid(unsigned int Rows, unsigned int Cols);
	void generateShader();
	unsigned int m_ProgramID;
	unsigned int m_IndexCount;

	FlyCamera m_camera;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	float time;
};


#endif