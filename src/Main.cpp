#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <cstdio>
#include "Gizmos.h"
<<<<<<< HEAD
#include "RenderTargets.h"
=======
#include "DifferedRendering.h"
>>>>>>> d017ceca1a88e7a3cfa2ef798f47c4925d1c7f92



int main()
{
<<<<<<< HEAD
	RenderTarget app;
=======
	DifferedRendering app;
>>>>>>> d017ceca1a88e7a3cfa2ef798f47c4925d1c7f92

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