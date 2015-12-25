#include "VM.hpp"

#include <cctype>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "Assembler.hpp"

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
		Program VM::loadBinary(const std::string& file)
		{
			std::ifstream fin(file, std::ios::binary);

			if(!fin)
				throw std::runtime_error("Unable to open input file");

			std::string identifier(4, 0);
			fin.read(&identifier[0], strLen(BINARY_ID));

			if(identifier != BINARY_ID)
				throw std::runtime_error("Input file is not a valid svm binary");

			std::uint32_t version = 0;
			fin.read(reinterpret_cast<char*>(&version), sizeof(version));

			// check version...
			/*
			if(version != thisVersion)
				throw std::runtime_error("Incompatible version!");
			*/

			Program out;

			// constants
			std::uint32_t numConstants = 0;
			fin.read(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

			for(std::uint32_t i = 0; i < numConstants; ++i)
			{
				Value::Type type;
				fin.read(reinterpret_cast<char*>(&type), sizeof(type));

				switch(type)
				{
					case Value::Type::Nil:
						out.constants.emplace_back();
						break;

					case Value::Type::Bool:
					{
						bool val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						out.constants.emplace_back(val);
						break;
					}

					case Value::Type::Int:
					{
						int val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						out.constants.emplace_back(val);
						break;
					}

					case Value::Type::Float:
					{
						float val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						out.constants.emplace_back(val);
						break;
					}

					case Value::Type::String:
					{
						std::size_t len = 0;
						fin.read(reinterpret_cast<char*>(&len), sizeof(len));

						string val(len, 0);
						fin.read(reinterpret_cast<char*>(&val[0]), len);

						out.constants.emplace_back(val);
						break;
					}

					default:
					{
						std::string msg = "Unknown type: ";
						msg += static_cast<byte>(type);

						throw std::runtime_error(msg);
					}
				}
			}

			// functions
			std::uint32_t numFunctions = 0;
			fin.read(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

			for(std::uint32_t i = 0; i < numFunctions; ++i)
			{
				std::size_t bounds[2];
				fin.read(reinterpret_cast<char*>(bounds), sizeof(bounds));

				out.functions.emplace_back(bounds[0], bounds[1]);
			}

			// bytecode
			auto curr = fin.tellg();
			fin.seekg(0, std::ios::end);
			auto end = fin.tellg();
			fin.seekg(curr);

			auto numBytes = end - curr;

			out.bytecode.resize(numBytes / sizeof(Instruction));
			
			fin.read(reinterpret_cast<char*>(&out.bytecode[0]), numBytes);

			return out;
		}

		void VM::writeBinary(const Program& program, const std::string& file)
		{
			std::ofstream fout(file, std::ios::binary);

			if(!fout)
				throw std::runtime_error("Unable to open output file");

			// file type identifier
			fout.write(BINARY_ID, strLen(BINARY_ID));

			// version
			fout.write("0000", 4);	// heh. need to figure out version stuff.

			// constants
			std::uint32_t numConstants = program.constants.size();
			fout.write(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

			for(auto& c : program.constants)
			{
				auto type = c.type();

				fout.write(reinterpret_cast<char*>(&type), sizeof(type));

				if(type != Value::Type::String)
				{
					Bytes bytes = c;
					fout.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
				}
				else
				{
					string str = c;
					std::size_t len = str.length();

					fout.write(reinterpret_cast<char*>(&len), sizeof(len));
					fout.write(str.c_str(), len);
				}
			}

			// functions
			std::uint32_t numFunctions = program.functions.size();
			fout.write(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

			for(auto& f : program.functions)
			{
				fout.write(reinterpret_cast<const char*>(&f), sizeof(f));
			}

			// bytecode
			fout.write(reinterpret_cast<const char*>(program.bytecode.data()), program.bytecode.size() * sizeof(Instruction));
		}

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

		void VM::repl(std::istream& in, std::ostream& out)
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
					il::Assembler::run(iss, out, program);
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
