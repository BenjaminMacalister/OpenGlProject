#ifndef _AssignmentLIGHTING_H_
#define _AssignmentLIGHTING_H_

#include "Application.h"
#include "CameraClass.h"
#include "tiny_obj_loader.h"
#include "Vertex.h"
#include <vector>

class AssignLighting
{
public:
	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw(FlyCamera a_camera);
	unsigned int m_lightingID;
	void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
	void CleanUpOpenGLBuffers();
	void reloadShader();

	std::vector<openGLData> m_glData;

	vec3	m_light_dir;
	vec3	m_light_colour;
	vec3	m_material_colour;
	vec3	m_ambient_light;
	float	m_spec_power;
	float m_timer;
};



#endif