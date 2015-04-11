#ifndef _RAINPARTICLES_H_
#define _RAINPARTICLES_H_

#include "Application.h"
#include "CameraClass.h"
#include "GPUEmitter.h"


class RainParticles
{
public:
	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw(FlyCamera a_camera);
	unsigned int m_rainID;
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
	GPUPointEmitter m_emitter1;
	GPUPointEmitter m_emitter2;
	GPUPointEmitter m_emitter3;
	GPUPointEmitter m_emitter4;
	GPUPointEmitter m_emitter5;
	GPUPointEmitter m_emitter6;
	GPUPointEmitter m_emitter7;
	GPUPointEmitter m_emitter8;
	GPUPointEmitter m_emitter9;
	float m_time;
};



#endif