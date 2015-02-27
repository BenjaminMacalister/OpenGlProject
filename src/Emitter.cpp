#include "Emitter.h"
#include "gl_core_4_4.h"

Emitter::Emitter() : m_particles(nullptr), m_maxParticles(0), m_particleCount(0),
					 m_vertexData(nullptr), m_indexData(nullptr)
{
	m_buffers = {};
}
Emitter::~Emitter()
{
	delete[] m_particles;
	delete[] m_vertexData;
	delete[] m_indexData;
	glDeleteVertexArrays(1, &m_buffers.m_VAO);
	glDeleteBuffers(1, &m_buffers.m_VBO);
	glDeleteBuffers(1, &m_buffers.m_IBO);

	
}
void Emitter::Init(unsigned int maxParticles, vec3 a_position, float a_emitRate,
	float a_minLifeSpan, float a_maxLifeSpan, float a_minVelocity, float a_maxVelocity,
	float a_startSize, float a_endSize, vec4 a_startColour, vec4 a_endColour, float gravity)
{
	m_Position = vec4(a_position, 1);
	m_LifeSpanMin = a_minLifeSpan;
	m_LifeSpanMax = a_maxLifeSpan;
	m_velocityMin = a_minVelocity;
	m_velocityMax = a_maxVelocity;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_emitRate = 1.0f / a_emitRate;
	m_maxParticles = maxParticles;
	m_gravity = gravity;
	m_particleCount = 0;
	m_particles = new Particles[maxParticles];
	m_vertexData = new VertexParticle[m_maxParticles * 4];
	m_indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < maxParticles; i++)
	{
		unsigned int start = 4 * i;
		m_indexData[i * 6 + 0] = start + 0;
		m_indexData[i * 6 + 1] = start + 1;
		m_indexData[i * 6 + 2] = start + 2;

		m_indexData[i * 6 + 3] = start + 0;
		m_indexData[i * 6 + 4] = start + 2;
		m_indexData[i * 6 + 5] = start + 3;
	}
	for (unsigned int i = 0; i < maxParticles; i++)
	{

	}
	glGenVertexArrays(1, &m_buffers.m_VAO);
	glGenBuffers(1, &m_buffers.m_VBO);
	glGenBuffers(1, &m_buffers.m_IBO);
	glBindVertexArray(m_buffers.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(VertexParticle), m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), m_indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // posiiton
	glEnableVertexAttribArray(1); // colour
	glEnableVertexAttribArray(2); //texture coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), (void*)sizeof(vec4));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), (void*)(sizeof(vec4)*2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Emitter::UpdateInit(unsigned int maxParticles, vec3 a_position, float a_emitRate,
	float a_minLifeSpan, float a_maxLifeSpan, float a_minVelocity, float a_maxVelocity,
	float a_startSize, float a_endSize, vec4 a_startColour, vec4 a_endColour, float gravity)
{
	m_gravity = gravity;
	m_Position = vec4(a_position, 1);
	m_LifeSpanMin = a_minLifeSpan;
	m_LifeSpanMax = a_maxLifeSpan;
	m_velocityMin = a_minVelocity;
	m_velocityMax = a_maxVelocity;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_emitRate = 1.0f / a_emitRate;
	m_maxParticles = maxParticles;
	m_particleCount = 0;
	m_particles = new Particles[maxParticles];
	m_vertexData = new VertexParticle[m_maxParticles * 4];
	m_indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < maxParticles; i++)
	{
		unsigned int start = 4 * i;
		m_indexData[i * 6 + 0] = start + 0;
		m_indexData[i * 6 + 1] = start + 1;
		m_indexData[i * 6 + 2] = start + 2;

		m_indexData[i * 6 + 3] = start + 0;
		m_indexData[i * 6 + 4] = start + 2;
		m_indexData[i * 6 + 5] = start + 3;
	}
	glGenVertexArrays(1, &m_buffers.m_VAO);
	glGenBuffers(1, &m_buffers.m_VBO);
	glGenBuffers(1, &m_buffers.m_IBO);
	glBindVertexArray(m_buffers.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(VertexParticle), m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), m_indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // posiiton
	glEnableVertexAttribArray(1); // colour
	glEnableVertexAttribArray(2); //texture coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), (void*)sizeof(vec4));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), (void*)(sizeof(vec4)* 2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Emitter::EmitParticles()
{
	unsigned int particlesToEmit = (unsigned int)(m_emitTimer / m_emitRate);
	m_emitTimer -= particlesToEmit * m_emitRate;
	for (unsigned int i = 0; i < particlesToEmit && m_particleCount<m_maxParticles; i++)
	{
		m_particles[m_particleCount].position = m_Position;
		m_particles[m_particleCount].lifeTime = 0;
		m_particles[m_particleCount].lifeSpan = glm::linearRand(m_LifeSpanMin, m_LifeSpanMax);
		m_particles[m_particleCount].colour = m_startColour;
		m_particles[m_particleCount].size = m_startSize;
		float velocityLength = glm::linearRand(m_velocityMin, m_velocityMax);
		m_particles[m_particleCount].velocity.xyz = glm::sphericalRand(velocityLength);
		m_particles[m_particleCount].velocity.w = 0;

		m_particleCount++;
	}
}
void Emitter::Update(float a_dt)
{
	//clean up the dead particles
	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		m_particles[i].lifeTime += a_dt;
		if (m_particles[i].lifeTime >m_particles[i].lifeSpan)
		{
			--m_particleCount;
			m_particles[i] = m_particles[m_particleCount];
			--i;
		}
	}
	//emit new particles
	m_emitTimer += a_dt;
	EmitParticles();
	

	//move all the alive particels
	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		m_particles[i].position += a_dt*m_particles[i].velocity;
		m_particles[i].position.y -= (a_dt*m_gravity)*m_particles[i].lifeTime;
		float t = m_particles[i].lifeTime / m_particles[i].lifeSpan;
		m_particles[i].colour = glm::mix(m_startColour, m_endColour, t);
		m_particles[i].size = glm::mix(m_startSize, m_endSize, t);
	}
}

void Emitter::UpdateVertexData(vec3 camPos, vec3 camUp)
{
	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		mat4 particleTransform(1);

		vec3 to = camPos - m_particles[i].position.xyz;
		vec3 f = glm::normalize(to);
		vec3 r = glm::cross(camUp, f);
		vec3 u = glm::cross(f, r);

		f *= m_particles[i].size;
		r *= m_particles[i].size;
		u *= m_particles[i].size;

		particleTransform[0].xyz = r;
		particleTransform[1].xyz = u;
		particleTransform[2].xyz = f;
		particleTransform[3] = m_particles[i].position;

		m_vertexData[i * 4 + 0].Position = particleTransform * vec4(-1, 1, 0, 1);
		m_vertexData[i * 4 + 1].Position = particleTransform * vec4(-1, -1, 0, 1);
		m_vertexData[i * 4 + 2].Position = particleTransform * vec4(1, -1, 0, 1);
		m_vertexData[i * 4 + 3].Position = particleTransform * vec4(1, 1, 0, 1);

		m_vertexData[i * 4 + 0].Colour = m_particles[i].colour;
		m_vertexData[i * 4 + 1].Colour = m_particles[i].colour;
		m_vertexData[i * 4 + 2].Colour = m_particles[i].colour;
		m_vertexData[i * 4 + 3].Colour = m_particles[i].colour;
		m_vertexData[i*4+0].tex_coord = vec2(0, 0);
		m_vertexData[i*4+1].tex_coord = vec2(0, 1);
		m_vertexData[i*4+2].tex_coord = vec2(1, 1);
		m_vertexData[i*4+3].tex_coord = vec2(1, 0);

	}
}

void Emitter::Render(unsigned int program, unsigned int texure)
{
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texure);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleCount * 4 * sizeof(VertexParticle), m_vertexData);
	glBindVertexArray(m_buffers.m_VAO);
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texure);*/
	int diffuse_location = glGetUniformLocation(program, "diffuse_tex");
	glUniform1i(diffuse_location, 0);
	glDrawElements(GL_TRIANGLES, 6 * m_particleCount, GL_UNSIGNED_INT, 0);

}