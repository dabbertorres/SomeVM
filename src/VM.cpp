#include "VM.hpp"

#include <cctype>
#include <algorithm>
#include <sstream>

#include "Parser.hpp"

namespace
{
	static void toLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	}
}

namespace dbr
{
	namespace svm
	{
		void VM::run(Program program, std::istream& in, std::ostream& out)
		{
			callStack.emplace(program.bytecode);

			StackFrame* currFrame = &callStack.top();

			bool done = false;

			while(!done)
			{
				done = currFrame->run(program.constants, program.functions, in, out);

				if(done)
				{
					callStack.pop();

					if(!callStack.empty())
					{
						currFrame = &callStack.top();
						done = false;
					}
				}
			}
		}

		void VM::interpreter(std::istream& in, std::ostream& out)
		{
			Program program;

			callStack.emplace();

			StackFrame* currFrame = &callStack.top();

			out << "SomeVM (C) Alec Iverson, 2015\n> ";

			std::string line;
			std::getline(in, line);

			while(line != "exit")
			{
				try
				{
					std::istringstream iss(line);

					auto oldSize = program.bytecode.size();
					il::Parser::run(iss, out, program);
					auto newSize = program.bytecode.size();

					if(newSize != oldSize)
					{
						currFrame->push(program.bytecode.back());
						bool state = currFrame->run(program.constants, program.functions, in, out);
					}
				}
				catch(const std::exception& e)
				{
					out << "\nError: " << e.what() << std::endl;
				}

				out << "\n> ";
				std::getline(in, line);
			}
		}

		std::size_t VM::callStackSize() const
		{
			return callStack.size();
		}
	}
}
