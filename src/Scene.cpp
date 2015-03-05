#include "Scene.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
AABB::AABB()
{
	reset();
}

AABB::~AABB()
{

}

void AABB::reset()
{
	min.x = min.y = min.z = 1e37f;
	max.x = max.y = max.z = -1e37f;
}

void AABB::fit(const std::vector<vec3>& points)
{
	for (auto& p : points)
	{
		if (p.x < min.y) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.y) min.x = p.x;
		if (p.y > max.y) min.y = p.y;
		if (p.z > max.z) min.z = p.z;
	}
}

void boundingSphere::fit(const std::vector<glm::vec3>& points)
{
	vec3 min(1e37f), max(-1e37f);

	for (auto& p : points)
	{
		if (p.x < min.y) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.y) min.x = p.x;
		if (p.y > max.y) min.y = p.y;
		if (p.z > max.z) min.z = p.z;
	}
	centre = (min + max) * 0.5f;
	radius = glm::distance(min, centre);
}
bool Scene::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);
	m_timer = -6;
	return true;
}

bool Scene::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}
	m_Camera.setPosition(vec3(2, 2, 2));
	float dt = (float)glfwGetTime();
	m_timer += dt;
	glfwSetTime(0.0);

	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 blue(0, 0, 1, 1);
	vec4 red(1, 0, 0, 1);
	vec4 yellow(1, 1, 0, 1);
	vec4 green(0, 1, 0, 1);

	for (int i = 0; i <= 20; i++)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	}
	boundingSphere sphere;
	vec4 plane(0, 1, 0, -1);
	vec4 planeColour(1, 1, 1, 1);
	sphere.centre = vec3(0, cosf((float)m_timer) + 1, 0);
	sphere.radius = 0.5;
	vec4 planes[6];
	getFrustumPlanes(m_Camera.getProjectionView(), planes);
	for (int i = 0; i<6; i++)
	{
		float dot = glm::dot(vec3(planes[i]), sphere.centre) + planes[i].w;
		if (dot > sphere.radius)
		{
			printf("front");
			//still render
			Gizmos::addSphere(sphere.centre, sphere.radius, 10, 10, vec4(1, 0, 0, 1));
		}
		else if (dot < -sphere.radius)
		{
			//dont render
			printf("back");
			//Gizmos::addSphere(sphere.centre, sphere.radius, 10, 10, vec4(0, 1, 0, 1));
		}
		else
		{
			//render
			printf("On");
			Gizmos::addSphere(sphere.centre, sphere.radius, 10, 10, vec4(0, 0, 1, 1));
		}
	}
	//Gizmos::addAABBFilled(vec3(0, 0, 0), plane.xyz, vec4(1, 1, 1, 1));
	Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4), vec3(-4, 1, 4), planeColour);
	Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4), vec3(-4, 1, -4), planeColour);
	//mat4 transform = glm::translate()
	mat4 moveSphere = glm::rotate(m_timer, vec3(0, 1, 0));
	mat4 moveSphere2 = glm::translate(vec3(0, m_timer, 0));
	m_Camera.update(dt);

	return true;
}
void Scene::ShutDown()
{
	Gizmos::destroy();
	Application::ShutDown();
}
void Scene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::draw(m_Camera.getProjectionView());

	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
	Gizmos::clear();
}

void Scene::getFrustumPlanes(const mat4& transform, vec4* planes)
{
	//rightside
	planes[0] = vec4(transform[0][3] - transform[0][0],
					 transform[1][3] - transform[1][0],
					 transform[2][3] - transform[2][0],
					 transform[3][3] - transform[3][0]);
	//left side
	planes[1] = vec4(transform[0][3] + transform[0][0],
					 transform[1][3] + transform[1][0],
					 transform[2][3] + transform[2][0],
					 transform[3][3] + transform[3][0]);
	//top
	planes[2] = vec4(transform[0][3] - transform[0][1],
					 transform[1][3] - transform[1][1],
					 transform[2][3] - transform[2][1],
					 transform[3][3] - transform[3][1]);
	//bottom
	planes[3] = vec4(transform[0][3] + transform[0][1],
					 transform[1][3] + transform[1][1],
					 transform[2][3] + transform[2][1],
					 transform[3][3] + transform[3][1]);
	//far
	planes[4] = vec4(transform[0][3] - transform[0][2],
					 transform[1][3] - transform[1][2],
					 transform[2][3] - transform[2][2],
					 transform[3][3] - transform[3][2]);
	//near
	planes[5] = vec4(transform[0][3] + transform[1][2],
					 transform[1][3] + transform[1][2],
					 transform[2][3] + transform[2][2],
					 transform[3][3] + transform[3][2]);

	for (int i = 0; i < 6; i++)
	{
		planes[i] = glm::normalize(planes[i]);
	}
}