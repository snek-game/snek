#include <snek-pch.h>

#include <snek/Clock.h>
#include <snek/snek-config.h>

#if SN_PLATFORM == SN_WIN
#	include <windows.h>

namespace
{
	LARGE_INTEGER get_frequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency;
	}
}
#endif

#include <chrono>

namespace snek
{
	Clock::Clock() :
        start_(Clock::CurrentTime())
	{
	}

	Duration Clock::Elapsed() const
	{
        return Clock::CurrentTime() - this->start_;
	}

	Duration Clock::Reset()
	{
		Duration now = Clock::CurrentTime();
		Duration e = now - this->start_;
		this->start_ = now;
		return e;
	}

    Duration Clock::CurrentTime()
	{
#if SN_PLATFORM == SN_WIN
        /*
		HANDLE currentThread = GetCurrentThread();
		DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);
		static LARGE_INTEGER frequency = get_frequency();
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		SetThreadAffinityMask(currentThread, previousMask);
		return sl::microseconds(1000000 * time.QuadPart / frequency.QuadPart);
        */
        return snek::Microseconds(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
#elif SL_PLATFORM == SL_MAC
		static mach_timebase_info_data_t frequency = {0, 0};
		if (frequency.denom == 0)
			mach_timebase_info(&frequency);
		uint64_t nanoseconds = mach_absolute_time() * frequency.numer / frequency.denom;
		return sl::microseconds(nanoseconds / 1000);
#else
		timespec time;
		Clock_gettime(Clock_MONOTONIC, &time);
		return sl::microseconds(static_cast<uint64_t>(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
#endif
		
	}

}
