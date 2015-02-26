#ifndef __ADVTEXTURE_H__
#define __ADVTEXTURE_H__
#include "Application.h"
#include "CameraClass.h"
#include "Vertex.h"


class AdvancedTexture : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	void LoadTexture();
	void generateQuad(float size);

	openGLData m_quad;

	unsigned int m_diffuse_texture;
	unsigned int m_normal_texture;
	unsigned int m_specular_texture;

	vec3 m_light_dir;
	vec3 m_light_colour;
	vec3 m_ambient_light;
	float m_spec_power;

	bool m_draw_gizmos;
	float m_fps;
	FlyCamera m_Camera;
	//unsigned int m_Texture;
	unsigned int m_ProgramID;
	float m_time;
	bool forward;
	//TwBar* m_bar;
	vec4 m_newColour;

};
#endif