#pragma once
#include "EngineCore.h"

class SoundCore
{
public:
	 sound::IEmitter* CreateMonoSound(const std::string& filePath, float volumeNormedPercent = 1, bool bLoop = false)
	{
		return Core.CreateMonoSound(filePath, volumeNormedPercent, bLoop);
	}
};

extern SoundCore Sound;