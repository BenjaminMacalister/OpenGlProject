#include "glm_header.h"
#include "Application.h"
#include "CameraClass.h"
#include <vector>
#ifndef _SCENE_H_
#define _SCENE_H_
class AABB
{
public:
	AABB();
	~AABB();

	void reset();
	void fit(const std::vector<vec3>& points);

	vec3 min, max;


};

class boundingSphere
{
public:
	boundingSphere() :centre(0), radius(0){};
	~boundingSphere() {};
	void fit(const std::vector<glm::vec3>& points);
	vec3 centre;
	float radius;
};

class Scene : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	FlyCamera m_Camera;
	float m_timer;
	void getFrustumPlanes(const mat4& transform, vec4* planes);
};


#endif