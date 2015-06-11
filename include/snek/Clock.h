#pragma once

#include "Duration.h"

namespace snek
{
	class Clock
	{
	private:
		Duration start_;
	public:
        explicit Clock();
		Duration Reset();
		Duration Elapsed() const;

		static Duration CurrentTime();
	};
}
