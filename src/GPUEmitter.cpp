#include "GPUEmitter.h"
#include "gl_core_4_4.h"
#include "Utility.h"

GPUPointEmitter::GPUPointEmitter():  m_particles(nullptr), m_maxParticles(0), m_position(0),
				m_drawShader(0), m_updateShader(0)
{
	m_VAO[0] = 0;
	m_VAO[1] = 0;
	m_VBO[0] = 0;
	m_VBO[1] = 0;
}
GPUPointEmitter::~GPUPointEmitter()
{
	delete[] m_particles;
	glDeleteVertexArrays(2, m_VAO);
	glDeleteBuffers(2, m_VBO);

	glDeleteProgram(m_updateShader);
	glDeleteProgram(m_drawShader);
}

void GPUPointEmitter::Init(unsigned int maxParticles, vec3 a_position, float a_emitRate,
	float a_minLifeSpan, float a_maxLifeSpan, float a_minVelocity, float a_maxVelocity,
	float a_startSize, float a_endSize, vec4 a_startColour, vec4 a_endColour, float gravity)
{
	m_position = a_position;
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_velocityMin = a_minVelocity;
	m_velocityMax = a_maxVelocity;
	m_LifeSpanMin = a_minLifeSpan;
	m_LifeSpanMax = a_maxLifeSpan;
	m_emitRate = a_emitRate;

	m_maxParticles = maxParticles;

	m_particles = new GPUParticle[maxParticles];
	m_activeBuffer = 0;

	CreateBuffers();
	CreateUpdateShader();
	CreateDrawShader();
}
void GPUPointEmitter::CreateBuffers()
{
	glGenVertexArrays(2, m_VAO);
	glGenBuffers(2, m_VBO);
	for (unsigned int bufferIndex = 0; bufferIndex < 2; bufferIndex++)
	{
		glBindVertexArray(m_VAO[bufferIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[bufferIndex]);
		glBufferData(GL_ARRAY_BUFFER, m_maxParticles*sizeof(GPUParticle), m_particles, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);//position
		glEnableVertexAttribArray(1);//velocity
		glEnableVertexAttribArray(2);//lifetime
		glEnableVertexAttribArray(3);//lifespan

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), (void*)(12));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), (void*)(24));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), (void*)(28));
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void GPUPointEmitter::CreateUpdateShader()
{
	unsigned int vertexShader;
	LoadShaderType("./Shaders/ParticleUpdate.glsl", GL_VERTEX_SHADER, &vertexShader);
	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, vertexShader);
	const char* outputs[4] = { "updatedPosition", "updatedVelocity", "updatedLifetime", "updatedLifespan"};
	glTransformFeedbackVaryings(m_updateShader, 4, outputs, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_updateShader);
	glDeleteShader(vertexShader);
	GLint uniforms;
	glGetProgramiv(m_updateShader, GL_ACTIVE_UNIFORMS, &uniforms);

}
void GPUPointEmitter::CreateDrawShader()
{

	m_drawShader = glCreateProgram();
	LoadShaders("./Shaders/gpuParticleVert.glsl", "./Shaders/gpuParticleGeo.glsl", "./Shaders/gpuParticleFrag.glsl", &m_drawShader);
	GLint uniforms;
	glGetProgramiv(m_drawShader, GL_ACTIVE_UNIFORMS, &uniforms);
}
void GPUPointEmitter::Draw(float a_currentTime, mat4 a_cameraTransform, mat4 a_projectionView)
{
	glUseProgram(m_updateShader);
	GLint uniforms;
	glGetProgramiv(m_updateShader, GL_ACTIVE_UNIFORMS, &uniforms);
	int deltaUniform = glGetUniformLocation(m_updateShader, "deltaTime");
	int emitterUniform = glGetUniformLocation(m_updateShader, "emitterPosition");
	int minVelUniform = glGetUniformLocation(m_updateShader, "minVelocity");
	int maxVelUniform = glGetUniformLocation(m_updateShader, "maxVelocity");
	int minLifeUniform = glGetUniformLocation(m_updateShader, "minLifespan");
	int maxLifeUniform = glGetUniformLocation(m_updateShader, "maxLifespan");
	int timeUniform = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(deltaUniform, a_currentTime - m_lastDrawTime);
	glUniform3fv(emitterUniform, 1, (float*)&m_position);
	glUniform1f(minVelUniform, m_velocityMin);
	glUniform1f(maxVelUniform, m_velocityMax);
	glUniform1f(minLifeUniform, m_LifeSpanMin);
	glUniform1f(maxLifeUniform, m_LifeSpanMax);
	glUniform1f(timeUniform, a_currentTime);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(m_VAO[m_activeBuffer]);
	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_VBO[otherBuffer]);

	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	//render pass
	glUseProgram(m_drawShader);
	int projViewUniform = glGetUniformLocation(m_drawShader, "projectionView");
	int cameraWorldUniform = glGetUniformLocation(m_drawShader, "cameraWorld");
	int startSizeUniform = glGetUniformLocation(m_drawShader, "startSize");
	int endSizeUniform = glGetUniformLocation(m_drawShader, "endSize");
	int startColourUniform = glGetUniformLocation(m_drawShader, "startColour");
	int endColourUniform = glGetUniformLocation(m_drawShader, "endColour");
	glUniformMatrix4fv(projViewUniform, 1, GL_FALSE, (float*)&a_projectionView);
	glUniformMatrix4fv(cameraWorldUniform, 1, GL_FALSE, (float*)&a_cameraTransform);
	glUniform1f(startSizeUniform, m_startSize);
	glUniform1f(endSizeUniform, m_endSize);
	glUniform4fv(startColourUniform, 1, (float*)&m_startColour);
	glUniform4fv(endColourUniform, 1, (float*)&m_endColour);

	glBindVertexArray(m_VAO[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	m_activeBuffer = otherBuffer;
	m_lastDrawTime = a_currentTime;
	

}