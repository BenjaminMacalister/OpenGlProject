#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "glm_header.h"
#include "Vertex.h"
struct Particles
{
	vec4 position;
	vec4 velocity;
	vec4 colour;
	float size;
	float lifeSpan;//how long it will live
	float lifeTime;//how long its been alive
};

//an Emitter contains particles it controls
//it spawns new particles
//it updates existing particles
//it rederis its particles
class Emitter
{
public:
	Emitter();
	~Emitter();
	void Init(unsigned int maxParticles, vec3 a_position, float a_emitRate,
		float a_minLifeSpan, float a_maxLifeSpan, float a_minVelocity, float a_maxVelocity,
		float a_startSize, float a_endSize, vec4 a_startColour, vec4 a_endColour, float gravity);
	void UpdateInit(unsigned int maxParticles, vec3 a_position, float a_emitRate,
		float a_minLifeSpan, float a_maxLifeSpan, float a_minVelocity, float a_maxVelocity,
		float a_startSize, float a_endSize, vec4 a_startColour, vec4 a_endColour, float gravity);

	void EmitParticles();
	void Update(float a_dt);
	void Render(unsigned int program, unsigned int texure);
	void UpdateVertexData(vec3 camPos, vec3 camUp);
	//Particle Data
	Particles* m_particles;
	unsigned int m_maxParticles;
	unsigned int m_particleCount;
	//openGL Data
	openGLData m_buffers;
	VertexParticle* m_vertexData;
	unsigned int* m_indexData;
	//Emitter Data
	vec4  m_Position;
	float m_emitRate;
	float m_emitTimer;

	float m_LifeSpanMin;
	float m_LifeSpanMax;
	float m_gravity;
	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	vec4 m_startColour;
	vec4 m_endColour;

};
#endif