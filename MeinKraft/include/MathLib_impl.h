#include <algorithm>
#include <cstdarg>
namespace mathlib
{
	template <typename T>
	T Clamp(const T& min, const T& max, const T& value)
	{
		return std::max(min, std::min(max, value));
	}

	template<typename T>
	const T Max(int nb, ...)
	{
		va_list args;

		va_start(args, nb);

		T Max = -INFINITY;

		T arg;

		for (int idx = 0; idx < nb; ++idx)
		{
			arg = va_arg(args, T);
			if (arg > Max)
				Max = arg;
		}

		va_end(args);
		return Max;
	}

	template<typename T>
	const T Min(int nb, ...)
	{
		va_list args;

		va_start(args, nb);

		T Min = INFINITY;

		T arg;

		for (int idx = 0; idx < nb; ++idx)
		{
			arg = va_arg(args, T);
			if (arg < Min)
				Min = arg;
		}

		va_end(args);
		return Min;
	}
}