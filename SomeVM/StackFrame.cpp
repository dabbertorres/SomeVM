#include "StackFrame.hpp"

#include "Function.hpp"

namespace dbr
{
	namespace svm
	{
		StackFrame::StackFrame(const Function& function, std::size_t argsIdx)
		:	function(function),
			argsIdx(argsIdx),
			currentInstruction(function.begin())
		{}

		StackFrame::StackFrame(const StackFrame& other)
		:	function(other.function),
			argsIdx(other.argsIdx),
			currentInstruction(function.begin() + std::distance(other.function.begin(), other.currentInstruction))
		{}

		StackFrame::StackFrame(StackFrame&& other)
		:	function(other.function),
			argsIdx(other.argsIdx),
			currentInstruction(function.begin() + std::distance(other.function.begin(), other.currentInstruction))
		{
			other.currentInstruction = other.function.begin();
		}

		Bytecode::const_iterator StackFrame::next()
		{
			return currentInstruction++;
		}

		bool StackFrame::complete() const
		{
			return currentInstruction == function.end();
		}

		void StackFrame::jump(Int instIdx)
		{
			if(0 <= instIdx && instIdx < function.length())
				currentInstruction = function.begin() + instIdx;
			else
				throw std::out_of_range("Attempt to jump out of bounds");
		}

		void StackFrame::rjump(Int instOff)
		{
			auto next = std::distance(function.begin(), currentInstruction) + instOff;

			if(0 <= next && next < function.length())
				currentInstruction += instOff;
			else
				throw std::out_of_range("Attempt to jump out of bounds");
		}

		Bytecode::const_iterator StackFrame::begin() const
		{
			return function.begin();
		}

		Bytecode::const_iterator StackFrame::end() const
		{
			return function.end();
		}

		std::size_t StackFrame::length() const
		{
			return function.length();
		}
	}
}