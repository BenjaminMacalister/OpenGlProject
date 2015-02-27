#ifndef _GPUPARTICLE_H_
#define _GPUPARTICLE_H_

#include "Application.h"
#include "CameraClass.h"
#include "GPUEmitter.h"


class GPUParticles : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
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
	FlyCamera m_Camera;
	GPUPointEmitter m_emitter;
	float m_time;
};



#endif