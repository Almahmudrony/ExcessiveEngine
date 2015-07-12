#include "Factory.h"
#include <locale>
#include <codecvt>

#include <GraphicsApi/OpenGL/GapiGL.h>
#include "PlatformLibrary/Sys.h"


graphics::IEngine* Factory::CreateGraphicsEngineRaster(const rGraphicsEngineRasterData& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = graphics::IEngine*(*)(const rGraphicsEngineRasterData& d);
	auto module = Sys::LoadDLL(L"GraphicsEngineRaster");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::GetDLLProcAddress(module, "CreateGraphicsEngineRaster");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#else	
	return new GraphicsEngineRaster(d);
#endif
}

graphics::IEngine* Factory::CreateGraphicsEngineRT(const rGraphicsEngineRT& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = graphics::IEngine*(*)(const rGraphicsEngineRT& d);
	auto module = Sys::LoadDLL(L"GraphicsEngineRasterRT");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::GetDLLProcAddress(module, "CreateGraphicsEngineRT");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#else
	return new GraphicsEngineRT(d);
#endif
}

physics::IEngine* Factory::CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = physics::IEngine*(*)(const rPhysicsEngineBullet& d);
	auto module = Sys::LoadDLL(L"PhysicsEngineBullet");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreatePhysicsEngineBullet");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#else
	return new PhysicsEngineBullet(d);
#endif
}

network::IEngine* Factory::CreateNetworkEngine(const rNetworkEngine& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = network::IEngine*(*)(const rNetworkEngine& d);
	auto module = Sys::LoadDLL(L"NetworkEngineBoost");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreateNetworkEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#else
	return new NetworkEngineBoost(d);
#endif
}

sound::IEngine* Factory::CreateSoundEngine(const rSoundEngine& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = sound::IEngine*(*)(const rSoundEngine& d);
	auto module = Sys::LoadDLL(L"SoundEngineSFML");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreateSoundEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#else
	return new SoundEngineSFML(d);
#endif
}

IGapi* Factory::CreateGapiGL() 
{
#ifdef EXCESSIVE_DLL_MODULES
	return ((IGapi*(*)())Sys::GetDLLProcAddress(Sys::LoadDLL((Sys::GetWorkDir() + L"GraphicsApiGL").c_str()), "createGraphicsApi"))();
#else
	return (IGapi*)new GapiGL();
#endif
}