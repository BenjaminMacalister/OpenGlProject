#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include "Application.h"
#include "CameraClass.h"
#include "tiny_obj_loader.h"
#include <vector>

struct openGLData
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_index_count;
};

class Lighting : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	unsigned int m_ProgramID;
	void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
	void CleanUpOpenGLBuffers();
	void reloadShader();

	std::vector<openGLData> m_glData;

	FlyCamera m_Camera;
	vec3	m_light_dir;
	vec3	m_light_colour;
	vec3	m_material_colour;
	vec3	m_ambient_light;
	float	m_spec_power;
};



#endif