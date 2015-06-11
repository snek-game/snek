#include <snek-pch.h>

#include <snek/Duration.h>

namespace snek
{
	Duration::Duration() :
		micro_(0)
	{
	}

	float Duration::S() const
	{
		return micro_ / 1000000.f;
	}

	int32_t Duration::Ms() const
	{
		return static_cast<int32_t>(micro_ / 1000);
	}

	int64_t Duration::Us() const
	{
		return micro_;
	}

	Duration::Duration(int64_t Microseconds) :
		micro_(Microseconds)
	{
	}

	Duration::Duration(const Duration& other) :
		micro_(other.micro_)
	{
	}

	Duration& Duration::operator=(const Duration& other)
	{
		this->micro_ = other.micro_;
		return *this;
	}

	Duration Seconds(float amount)
	{
		return Duration(static_cast<int64_t>(amount * 1000000));
	}

	Duration Milliseconds(int32_t amount)
	{
		return Duration(static_cast<int64_t>(amount)* 1000);
	}

	Duration Microseconds(int64_t amount)
	{
		return Duration(amount);
	}

	bool operator ==(Duration left, Duration right)
	{
		return left.Us() == right.Us();
	}

	bool operator !=(Duration left, Duration right)
	{
		return left.Us() != right.Us();
	}

	bool operator <(Duration left, Duration right)
	{
		return left.Us() < right.Us();
	}

	bool operator >(Duration left, Duration right)
	{
		return left.Us() > right.Us();
	}

	bool operator <=(Duration left, Duration right)
	{
		return left.Us() <= right.Us();
	}

	bool operator >=(Duration left, Duration right)
	{
		return left.Us() >= right.Us();
	}

	Duration operator -(Duration right)
	{
		return Microseconds(-right.Us());
	}

	Duration operator +(Duration left, Duration right)
	{
		return Microseconds(left.Us() + right.Us());
	}

	Duration& operator +=(Duration& left, Duration right)
	{
		return left = left + right;
	}

	Duration operator -(Duration left, Duration right)
	{
		return Microseconds(left.Us() - right.Us());
	}

	Duration& operator -=(Duration& left, Duration right)
	{
		return left = left - right;
	}

	Duration operator *(Duration left, float right)
	{
		return Seconds(left.S() * right);
	}

	Duration operator *(Duration left, int64_t right)
	{
		return Microseconds(left.Us() * right);
	}

	Duration operator *(float left, Duration right)
	{
		return right * left;
	}

	Duration operator *(int64_t left, Duration right)
	{
		return right * left;
	}

	Duration& operator *=(Duration& left, float right)
	{
		return left = left * right;
	}

	Duration& operator *=(Duration& left, int64_t right)
	{
		return left = left * right;
	}

	Duration operator /(Duration left, float right)
	{
		return Seconds(left.S() / right);
	}

	Duration operator /(Duration left, int64_t right)
	{
		return Microseconds(left.Us() / right);
	}

	Duration& operator /=(Duration& left, float right)
	{
		return left = left / right;
	}

	Duration& operator /=(Duration& left, int64_t right)
	{
		return left = left / right;
	}
}