#ifndef _Differed_H_
#define _Differed_H_

#include "Application.h"
#include "CameraClass.h"
#include "Vertex.h"

class DifferedRendering : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();


	void buildMeshes();
	void buildGBuffer();
	void buildLightBuffer();
	void buildQuad();
	void buildCube();
	void RenderDirectionalLight(vec3 a_lightDirection, vec3 a_lightColour);
	void RenderPointLight(vec3 position, float radius, vec3 diffuse);
	//LoadMesh
	openGLData m_lightCube;
	openGLData m_bunny;
	openGLData m_screenSpaceQuad;


	//generate a gBuffer
	unsigned int m_gBufferFbo;
	unsigned int m_albedoTexture;
	unsigned int m_positionTexture;
	unsigned int m_normalTexture;
	unsigned int m_gBufferDepth;

	//render lights
	unsigned int m_lightFbo;
	unsigned int m_lightTexture;

	//Shaders
	unsigned int m_gBufferProgram;
	unsigned int m_dirrectionalLightProgram;
	unsigned int m_PointLightProgram;
	unsigned int m_SpotLightProgram;
	unsigned int m_compositeProgram;
	float m_time;

	FlyCamera m_Camera;

};



#endif