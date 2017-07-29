#include "StackFrame.hpp"

#include "Function.hpp"

namespace svm
{
	StackFrame::StackFrame(const Function& function, std::uint64_t functionIndex, std::uint64_t argsIdx)
		: function(function),
		functionIndex(functionIndex),
		argsIdx(argsIdx),
		currentInstruction(function.begin())
	{}

	StackFrame::StackFrame(StackFrame&& other)
		: function(other.function),
		functionIndex(other.functionIndex),
		argsIdx(other.argsIdx),
		currentInstruction(function.begin() + std::distance(other.function.begin(), other.currentInstruction))
	{
		other.currentInstruction = other.function.begin();
	}

    StackFrame& StackFrame::operator=(StackFrame&& other)
    {
        return *this;
    }

	Bytecode::const_iterator StackFrame::next()
	{
		return currentInstruction++;
	}

	bool StackFrame::complete() const
	{
		return currentInstruction == function.end();
	}

	void StackFrame::jump(std::uint64_t instIdx)
	{
		if (instIdx < function.length())
			currentInstruction = function.begin() + instIdx;
		else
			throw std::out_of_range("Attempt to jump out of bounds");
	}

	void StackFrame::rjump(std::int64_t instOff)
	{
		auto next = static_cast<std::uint64_t>(std::distance(function.begin(), currentInstruction) + instOff);

		if (next < function.length())
			currentInstruction += instOff;
		else
			throw std::out_of_range("Attempt to relative jump out of bounds");
	}

	Bytecode::const_iterator StackFrame::begin() const
	{
		return function.begin();
	}

	Bytecode::const_iterator StackFrame::end() const
	{
		return function.end();
	}

	std::uint64_t StackFrame::length() const
	{
		return function.length();
	}
}