#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Muelsyse/Core/PlatformDetection.h"

#ifdef MUL_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <chrono>
#include <fstream>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>

#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Core/Buffer.h"
#include "Muelsyse/Core/Logger.h"
#include "Muelsyse/Debug/Instrumentor.h"