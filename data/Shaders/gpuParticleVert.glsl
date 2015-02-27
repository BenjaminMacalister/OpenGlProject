#version 410
layout(location=0) in vec3 position;
layout(location=1) in vec3 velocity;
layout(location=2) in float lifetime;
layout(location=3) in float lifespan;
out vec3 geoPosition;
//out vec3 geoVelocity;
out float geoLifetime;
out float geoLifespan;

void main()
{
	geoPosition = position;
	//geoVelocity = velocity;
	geoLifetime = lifetime;
	geoLifespan = lifespan;
}