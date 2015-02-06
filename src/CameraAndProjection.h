#ifndef _CAMPROJ_H_
#define CAMPROJ_H_

#include "Application.h"
#include "CameraClass.h"

class CameraAndProjection : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	FlyCamera m_Camera;
};



#endif