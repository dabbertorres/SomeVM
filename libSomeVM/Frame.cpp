#include "Frame.hpp"

#include "Function.hpp"

namespace svm
{
	Frame::Frame(const Function& function, std::uint64_t functionIndex, std::uint64_t argsIdx)
		: function(function),
		functionIndex(functionIndex),
		argsIdx(argsIdx),
		currentInstruction(function.begin())
	{}

	Frame::Frame(Frame&& other)
		: function(other.function),
		functionIndex(other.functionIndex),
		argsIdx(other.argsIdx),
		currentInstruction(function.begin() + std::distance(other.function.begin(), other.currentInstruction))
	{
		other.currentInstruction = other.function.begin();
	}

    Frame& Frame::operator=(Frame&&/* other*/)
    {
        // TODO
        return *this;
    }

	Bytecode::const_iterator Frame::next()
	{
		return currentInstruction++;
	}

	bool Frame::complete() const
	{
		return currentInstruction == function.end();
	}

	void Frame::jump(std::uint64_t instIdx)
	{
		if (instIdx < function.length())
			currentInstruction = function.begin() + instIdx;
		else
			throw std::out_of_range("Attempt to jump out of bounds");
	}

	void Frame::rjump(std::int64_t instOff)
	{
		auto next = static_cast<std::uint64_t>(std::distance(function.begin(), currentInstruction) + instOff);

		if (next < function.length())
			currentInstruction += instOff;
		else
			throw std::out_of_range("Attempt to relative jump out of bounds");
	}

	Bytecode::const_iterator Frame::begin() const
	{
		return function.begin();
	}

	Bytecode::const_iterator Frame::end() const
	{
		return function.end();
	}

	std::uint64_t Frame::length() const
	{
		return function.length();
	}
}