#pragma once

#include "libSomeVM.hpp"

#include <cstdint>

namespace svm
{
	enum class SVM_API SysCall : std::int64_t
	{
		Print = 0,
	};
}
