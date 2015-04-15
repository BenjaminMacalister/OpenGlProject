#ifndef _GPUEMITTER_H_
#define _GPUEMITTER_H_

#include "glm_header.h"
//gpu particle struct
struct GPUParticle
{
	GPUParticle() :lifeSpan(0), lifeTime(1) {}
	vec3 position;
	vec3 velocity;
	float lifeTime;
	float lifeSpan;
};

//gpu emitter class
	//creating gpu buffers
	//setting up stuff for transform feedback
	//creating shaders
	//rendering them
class GPUPointEmitter
{
public:
	GPUPointEmitter();
	~GPUPointEmitter();

	void Init(unsigned int maxParticles, vec3 a_position, float a_emitRate,
		float a_minLifeSpan, float a_maxLifeSpan, float a_minVelocity, float a_maxVelocity,
		float a_startSize, float a_endSize, vec4 a_startColour, vec4 a_endColour, float gravity);
	void Draw(float a_currentTime, mat4 a_cameraTransform, mat4 a_prohectionView, vec4 colour, float size);
	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();

	GPUParticle* m_particles;
	unsigned int m_maxParticles;

	vec4 m_startColour;
	vec4 m_endColour;
	vec3 m_position;

	float m_emitRate;
	float m_emitTimer;

	float m_LifeSpanMin;
	float m_LifeSpanMax;
	float m_gravity;
	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;
	unsigned int m_activeBuffer;
	unsigned int m_VAO[2];
	unsigned int m_VBO[2];
	unsigned int m_updateShader;
	unsigned int m_drawShader;

	float m_lastDrawTime;
};

#endif