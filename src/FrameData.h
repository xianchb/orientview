// Copyright � 2014 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: GPLv3, see the LICENSE file.

#pragma once

#include <cstdint>

namespace OrientView
{
	struct FrameData
	{
		uint8_t* data = nullptr;
		int dataLength = 0;
		int rowLength = 0;
		int width = 0;
		int height = 0;
		int duration = 0; // ms
		int number = 0;
	};
}
