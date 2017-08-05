#include "Debugger.hpp"

namespace svm
{
	Debugger::Debugger()
	{}

	Debugger::~Debugger()
	{}

	void Debugger::run(const std::vector<Breakpoint>& breakpoints)
	{
		breakpoints;
		//callStack.emplace(functions.front(), 0, 0);

		//std::uint64_t nextBreakpoint = 0;

		//while(!callStack.empty())
		//{
		//	Frame& frame = callStack.top();

		//	if(!frame.complete())
		//	{
		//		auto next = frame.next();

		//		if(nextBreakpoint < breakpoints.size())
		//		{
		//			const Breakpoint& bp = breakpoints[nextBreakpoint++];

		//			if(bp.functionIndex == frame.functionIndex)
		//			{
		//				std::uint64_t instIdx = std::distance(frame.begin(), next);

		//				if(bp.instructionIndex == instIdx)
		//				{
		//					// May want a different design.
		//					// Debugger class, which takes a VM as an argument
		//					// and runs from there.

		//					std::string input;
		//					do
		//					{
		//						outStream << "> ";

		//						std::getline(inStream, input);
		//					}
		//					while(input != "c");
		//				}
		//			}
		//		}

		//		interpret(*next, frame);
		//	}
		//	else
		//	{
		//		callStack.pop();
		//	}
		//}
	}
}
