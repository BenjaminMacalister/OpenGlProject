#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <cstdio>
#include "Gizmos.h"
#include "IntroToOpenGl.h"
#include "CameraAndProjection.h"




int main()
{
	IntroToOpenGl app;

	app.StartUp();
	if (app.StartUp() == false)
	{
		return -1;
	}

	while (app.Update() == true)
	{
		app.Draw();
	}
	

	return 0;

}