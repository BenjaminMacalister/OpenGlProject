#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Application.h"
#include "CameraClass.h"
#include "Emitter.h"

class Particle : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	void LoadTexture();
	FlyCamera m_Camera;
	Emitter myEmiter;
	unsigned int m_ProgramID;
	unsigned int m_texture;

	float SpawnMaxLimit;
	vec4 StartPatritcleColour;
	vec4 endParticleColour;
	float minSpeed;
	float maxSpeed;
	float startSize;
	float endSize;
	float minLife;
	float maxLife;
	float m_gravity;
	int SpawnAmount;
};



#endif