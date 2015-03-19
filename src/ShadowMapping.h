#ifndef _ShadowMapping_H_
#define _ShadowMapping_H_

#include "Application.h"
#include "CameraClass.h"
#include "Vertex.h"

class Shadows : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	FlyCamera m_Camera;

	openGLData m_bunney;
	openGLData m_plane;

	unsigned int m_fbo;
	unsigned int m_fboDepth;

	unsigned int m_shadowmapProgram;
	unsigned int m_diffuseShadowedProgram;
	void buildMeshes();
	void buildShadowsMap();
	vec3 m_lightDirection;
	mat4 m_lightMatrix;
};



#endif