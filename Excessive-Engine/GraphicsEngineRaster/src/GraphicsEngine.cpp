#include "GraphicsEngine.h"




GraphicsEngine::GraphicsEngine() {
	// l����fasz rizzsel
}

GraphicsEngine::~GraphicsEngine() {
	// l�fasz rizzsel
}


void GraphicsEngine::release() {
	delete this;
}


ge::IScene* GraphicsEngine::createScene() {
	return nullptr;
}