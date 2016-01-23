#include "StackFrame.hpp"

namespace dbr
{
	namespace svm
	{
		StackFrame::StackFrame()
		:	code(),
			currInstr(code.begin()),
			registry(registrySize),
			nextFree(registry.begin())
		{}

		StackFrame::StackFrame(Bytecode bc)
		:	code(bc),
			currInstr(code.begin()),
			registry(registrySize),
			nextFree(registry.begin())
		{}

		StackFrame::StackFrame(Bytecode bc, Registry::const_iterator begin, Registry::const_iterator end)
		:	StackFrame(bc)
		{
			nextFree = std::copy(begin, end, registry.begin());
		}

		StackFrame::StackFrame(const StackFrame& other)
		:	StackFrame()
		{
			*this = other;
		}

		StackFrame::StackFrame(StackFrame&& other)
		:	StackFrame()
		{
			*this = std::move(other);
		}

		StackFrame& StackFrame::operator=(const StackFrame& other)
		{
			code.resize(other.code.size());
			std::copy(other.code.begin(), other.code.end(), code.begin());

			currInstr = code.begin() + (other.currInstr - other.code.begin());

			std::copy(other.registry.begin(), other.registry.end(), registry.begin());

			nextFree = registry.begin() + (other.nextFree - other.registry.begin());

			return *this;
		}

		StackFrame& StackFrame::operator=(StackFrame&& other)
		{
			code = std::move(other.code);
			currInstr = std::move(other.currInstr);

			registry = std::move(other.registry);
			nextFree = std::move(other.nextFree);

			return *this;
		}

		Bytecode::const_iterator StackFrame::next()
		{
			// if we haven't reached the end, increment before returning.
			// otherwise, just return
			if(currInstr != code.end())
				return currInstr++;
			else
				return currInstr;
		}

		void StackFrame::jump(Int instOff)
		{
			// bounds checking - wrap to ends if would go out of bounds
			if(instOff > 0 && code.end() - currInstr < instOff)
				currInstr = code.end();
			else if(instOff < 0 && currInstr - code.begin() < std::abs(instOff))
				currInstr = code.begin();
			else
				currInstr += instOff;
		}

		void StackFrame::push(const Instruction& inst)
		{
			// just in case code.push_back causes a reallocation, store and restore the offset of currInstr
			auto dist = currInstr - code.begin();
			code.push_back(inst);
			currInstr = code.begin() + dist;
		}

		void StackFrame::write(Value val)
		{
			if(nextFree != registry.end())
				*nextFree++ = val;
			else
				throw std::runtime_error("Error: Registry full");
		}

		void StackFrame::write(std::size_t idx, Value val)
		{
			registry.at(idx) = val;
		}

		void StackFrame::writeAll(Registry::const_iterator begin, Registry::const_iterator end)
		{
			nextFree = std::copy(begin, end, nextFree);
		}

		void StackFrame::writeAll(std::size_t idx, Registry::const_iterator begin, Registry::const_iterator end)
		{
			auto endCopy = std::copy(begin, end, registry.begin() + idx);
			
			// move nextFree only if the copy went past it
			if(endCopy > nextFree)
				nextFree = endCopy;
		}

		Value StackFrame::read(std::size_t idx) const
		{
			return registry.at(idx);
		}

		const StackFrame::Registry& StackFrame::getRegistry() const
		{
			return registry;
		}

		const Bytecode& StackFrame::getBytecode() const
		{
			return code;
		}

		Bytecode::const_iterator StackFrame::codeBegin() const
		{
			return code.begin();
		}

		Bytecode::const_iterator StackFrame::codeEnd() const
		{
			return code.end();
		}

		StackFrame::Registry::const_iterator StackFrame::regBegin() const
		{
			return registry.begin();
		}

		StackFrame::Registry::const_iterator StackFrame::regEnd() const
		{
			return registry.end();
		}
	}
}