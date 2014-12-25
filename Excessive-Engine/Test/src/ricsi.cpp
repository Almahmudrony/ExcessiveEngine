﻿#include "EngineCore.h"
#include "Factory.h"
#include "ICamera.h"

// basic util
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include "Sys.h"


#define CAM_MOVE_SPEED 10
float gCamSpeedMultiplier = 1;

IGapi* gGapi;

int Ricsi() {

	// Init window
	rWindow d;
		d.clientW = 800;
		d.clientH = 600;
		d.capText = "Excessive-Engine -> Ricsi teszt";
	IWindow* window = Factory::createWindow(d);
	
	// Init core (graphics.... etc)
	rGraphicsEngineRaster gDesc;
		gDesc.type = eGapiType::OPENGL_4_5;
	
	EngineCore core;
		graphics::IGraphicsEngine* gEngine = core.initGraphicsEngineRaster(gDesc);
	
	// Create scene and camera
	graphics::IScene* scene = gEngine->createScene();
		graphics::ICamera* cam = gEngine->createCam();
		cam->setFOV(3.14f / 2);
		cam->setAspectRatio(window->getClientAspectRatio());
		cam->setNearPlane(0.05f);
		cam->setFarPlane(3000);
		cam->setPos(mm::vec3(0, -3, 1));
	scene->setCam(cam);


	Entity* simpleEntity = core.createEntity(scene, Sys::getWorkDir() + L"../Assets/teapot.dae");

	// So that I can read init messages
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	
	// Run the main loop
	rWindowEvent ev;
	double elapsed;
	std::chrono::time_point<std::chrono::high_resolution_clock> last_frame;

	// Timer for dT frame calc
	//ITimer* t = Factory::createTimer();

	bool bWDown = false;
	bool bSDown = false;
	bool bADown = false;
	bool bDDown = false;

	bool bRMBDown = false;

	while (window->isOpen()) {
		//t->reset();

		// keep 60 fps
		elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_frame).count() / 1.0e6;
		int fps = 1 / elapsed + 0.5;
		std::wstringstream title_ss;
		static float camAngleX = 0; // upwards/downwards looking
		static float camAngleZ = 0; // orientation
		title_ss << L"Excessive-Engine - Ricsi teszt | FPS=" << fps << L" | Pitch=" << camAngleX * 180 / 3.141592653f << L"° Facing=" << camAngleZ * 180 / 3.141592653f << L"°";
		window->setText(title_ss.str().c_str());
		last_frame = std::chrono::high_resolution_clock::now();

		while (window->popEvent(&ev))
			switch (ev.msg)
			{
				case eWindowMsg::MOUSE_PRESS:
					if (ev.mouseBtn == eMouseBtn::RIGHT)
						bRMBDown = true;
					break;
				case eWindowMsg::MOUSE_RELEASE:
					if (ev.mouseBtn == eMouseBtn::RIGHT)
						bRMBDown = false;
					break;
				case eWindowMsg::MOUSE_MOVE: {
					if (bRMBDown)
					{
						float angleChangeZ = (float)(ev.mouseDeltaX) * 0.009;
						float angleChangeX = (float)(-ev.mouseDeltaY) * 0.009;

						mm::vec3 viewDir = mm::normalize(cam->getTarget() - cam->getPos());
						float lenXY = mm::length(viewDir.xy);
						static float angleX = acos(lenXY)*(viewDir.z > 0 ? 1 : -1);
						angleX += angleChangeX;
						angleX = std::max(-85.f / 180 * 3.141592653f, std::min(angleX, 85.f / 180 * 3.141592653f));
						static float angleZ = atan2(viewDir.y / lenXY, viewDir.z / lenXY) - 3.141592653f/2;
						angleZ += angleChangeZ;
						if (angleZ > 3.141592653f) {
							angleZ -= floor(angleZ / 3.141592653f) * 2*3.141592653f;
						}
						else if (angleZ < -3.141592653f) {
							angleZ -= ceil(angleZ / 3.141592653f) * 2 * 3.141592653f;
						}

						mm::vec3 newViewDir(0, 1, 0);

						mm::mat3 rotAroundX(
							1, 0, 0,
							0, cos(angleX), -sin(angleX),
							0, sin(angleX), cos(angleX)
							);
						mm::mat3 rotAroundZ(
							cos(angleZ), -sin(angleZ), 0,
							sin(angleZ), cos(angleZ), 0,
							0, 0, 1
							);

						newViewDir *= rotAroundX;
						newViewDir *= rotAroundZ;

						camAngleX = angleX;
						camAngleZ = angleZ;

						cam->setTarget(cam->getPos() + newViewDir);
					}
				} break;

				case eWindowMsg::KEY_PRESS:
					switch (ev.key)
					{
						case eKey::ESCAPE: window->close(); break;
						case eKey::W: bWDown = true; break;
						case eKey::S: bSDown = true; break;
						case eKey::A: bADown = true; break;
						case eKey::D: bDDown = true; break;
						case eKey::LSHIFT: gCamSpeedMultiplier = 5; break;
					} break;
				case eWindowMsg::KEY_RELEASE:
					switch (ev.key)
					{
						case eKey::W: bWDown = false; break;
						case eKey::S: bSDown = false; break;
						case eKey::A: bADown = false; break;
						case eKey::D: bDDown = false; break;
						case eKey::LSHIFT: gCamSpeedMultiplier = 1; break;
					} break;
			}

		// Camera move
		if (bWDown) // W
			cam->setPos(cam->getPos() + cam->getDirFront() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
		if (bSDown) // S									 
			cam->setPos(cam->getPos() + cam->getDirBack()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
		if (bADown) // A									 
			cam->setPos(cam->getPos() + cam->getDirLeft()  * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);
		if (bDDown) // D									 
			cam->setPos(cam->getPos() + cam->getDirRight() * CAM_MOVE_SPEED * elapsed * gCamSpeedMultiplier);

		// Update everything

		//float deltaT = t->getElapsedSinceReset();

		// Update graphics engine
		gEngine->update();

		// Call that after OpenGL "finish" all of it's rendering
		window->displayClientRect();

		// keep 60 fps
		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds sleep_time = std::chrono::microseconds(16667) - std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame);
		//this_thread::sleep_for(sleep_time);
	}
	std::cout << std::endl;

	return 0;
}
