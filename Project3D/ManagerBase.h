#pragma once
#include "Camera.h"
#include "Terrain.h"
#include <list>
#include <memory>

struct AppBlock {
	Graphics& gfx;
	Camera& cam;
	int width;
	int height;
};

template<class T>
class ManagerBase {
public:
	ManagerBase(AppBlock& appBlock) 
		: appBlock(appBlock) {};
	//virtual void Update();
	
protected:
	AppBlock& appBlock;
	std::list<std::unique_ptr<T>> classList;
};