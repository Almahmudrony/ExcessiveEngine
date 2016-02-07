#pragma once

#include "Logger.hpp"
#include <BaseLibrary/Utility>

namespace exc {

/// <summary> 
/// System-wide logging facility.
/// See <see cref="Logger"> for more on how it works, it's just a singletonified Logger.
/// </summary>
using LogCentre = Singleton<Logger>;

}