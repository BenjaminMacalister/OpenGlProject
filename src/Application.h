#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct GLFWwindow;
class Application
{
public:
	Application();
	virtual ~Application();

	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	GLFWwindow* m_window;
};

#endif  //close application if it already exists