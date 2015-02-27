#version 410

layout(location=0) in vec3 position;
layout(location=1) in vec3 velocity;
layout(location=2) in float lifetime;
layout(location=3) in float lifespan;

out vec3 updatedPosition;
out vec3 updatedVelocity;
out float updatedLifetime;
out float updatedLifespan;
uniform float deltaTime;
uniform vec3 emitterPosition;


const float INVERSE_MAX_UNIT = 1.0f / 4294967295.0f;
float rand(uint seed, float range)
{
	uint i= (seed ^ 12345391u) * 2654435769u;
	i^= ( i << 6u) ^ (i >> 26u);
	i*= 2654435769u;
	i += (i<<5u) ^ ( i >> 12u);
	return float(range * i) * INVERSE_MAX_UNIT;
}
uniform float minVelocity;
uniform float maxVelocity;
uniform float minLifespan;
uniform float maxLifespan;
uniform float time;
void main()
{
	//move particles based on velocity
	updatedPosition = position + velocity * deltaTime;
	updatedVelocity = velocity;
	updatedLifespan = lifespan;
	//update the lifetime based on delta time
	updatedLifetime = lifetime+ deltaTime;

	//spawn new particles if we need to
	if(updatedLifetime > lifespan)
	{
		updatedPosition = emitterPosition;
		//set velocity
		float velRange = maxVelocity - minVelocity;

		uint seed = uint(gl_VertexID +(time * 1000));
		float velLen = rand(seed++, velRange) + minVelocity;

		updatedVelocity.x = rand(seed++, 2) -1;
		updatedVelocity.y = rand(seed++, 2) -1;
		updatedVelocity.z = rand(seed++, 2) -1;
		updatedVelocity = normalize(updatedVelocity) * velLen;

		//set new lifespan
		updatedLifetime = 0;
		float lifespanRange = maxLifespan - minLifespan;
		updatedLifespan = rand(seed++, lifespanRange) + minLifespan;
	}
}
