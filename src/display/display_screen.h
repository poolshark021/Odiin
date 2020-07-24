#pragma once

#include <cstdint>
#include "ili9341.h"

namespace Display
{
	using color_t = uint32_t;
	using coord_t = uint16_t;

	class Screen
	{
	public:
		Screen();
		virtual ~Screen();
		Screen(const Screen &) = delete;
		Screen &operator=(const Screen &) = delete;

		void Update();
		inline bool IsInitialized() { return initialized; }
		static constexpr uint32_t GraphicsTickMs = 1;
	private:
		static bool initialized;
		static void Tick(void* context);
	};
} // namespace Display
