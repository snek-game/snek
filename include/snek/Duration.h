#pragma once
#include <cstdint>

namespace snek
{
	class Duration
	{
	private:
		int64_t micro_;

		friend Duration Seconds(float);
		friend Duration Microseconds(int64_t);
		friend Duration Milliseconds(int32_t);

		Duration();
	public:
		explicit Duration(int64_t);
		Duration(const Duration& other);
		Duration& operator=(const Duration& other);

		int64_t Us() const;
		int32_t Ms() const;
		float S() const;
	};

	Duration Seconds(float amount);
	Duration Milliseconds(int32_t amount);
	Duration Microseconds(int64_t amount);

	bool operator ==(Duration left, Duration right);
	bool operator !=(Duration left, Duration right);
	bool operator <(Duration left, Duration right);
	bool operator >(Duration left, Duration right);
	bool operator <=(Duration left, Duration right);
	bool operator >=(Duration left, Duration right);
	Duration operator -(Duration right);
	Duration operator +(Duration left, Duration right);
	Duration& operator +=(Duration& left, Duration right);
	Duration operator -(Duration left, Duration right);
	Duration& operator -=(Duration& left, Duration right);
	Duration operator *(Duration left, float right);
	Duration operator *(Duration left, int64_t right);
	Duration operator *(float left, Duration right);
	Duration operator *(int64_t left, Duration right);
	Duration& operator *=(Duration& left, float right);
	Duration& operator *=(Duration& left, int64_t right);
	Duration operator /(Duration left, float right);
	Duration operator /(Duration left, int64_t right);
	Duration& operator /=(Duration& left, float right);
	Duration& operator /=(Duration& left, int64_t right);
}
