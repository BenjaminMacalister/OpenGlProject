#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <cstdio>
#include "Gizmos.h"
#include "IntroToOpenGl.h"
#include "CameraAndProjection.h"
#include "Rendering Geometry.h"
#include "Texturing.h"
#include "Lighting.h"
#include "AdvancedTexturing.h"




int main()
{
	AdvancedTexture app;

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