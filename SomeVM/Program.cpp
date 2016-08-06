#include "Program.hpp"

#include <sstream>
#include <fstream>

#include "Assembler.hpp"
#include "Util.hpp"

namespace dbr
{
	namespace svm
	{
		std::size_t Program::load(const std::string& file)
		{
			std::ifstream fin(file, std::ios::binary);

			if(!fin)
				throw std::runtime_error("Unable to open input file");

			auto startPos = fin.tellg();

			std::string identifier(4, 0);
			fin.read(&identifier[0], util::strlen(BINARY_ID));

			if(identifier != BINARY_ID)
				throw std::runtime_error("Input file is not a valid svm binary");

			// check version...
			std::uint32_t version = 0;
			fin.read(reinterpret_cast<char*>(&version), sizeof(version));

			if(version != VERSION)
				throw std::runtime_error("Incompatible version");

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
						constants.emplace_back();
						break;

					case Value::Type::Bool:
					{
						Bool val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						constants.emplace_back(val);
						break;
					}

					case Value::Type::Int:
					{
						Int val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						constants.emplace_back(val);
						break;
					}

					case Value::Type::Float:
					{
						Float val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						constants.emplace_back(val);
						break;
					}

					case Value::Type::String:
					{
						std::uint32_t len = 0;
						fin.read(reinterpret_cast<char*>(&len), sizeof(len));

						String val(len, 0);
						fin.read(reinterpret_cast<char*>(&val[0]), len);

						constants.emplace_back(val);
						break;
					}

					default:
					{
						std::string msg = "Unknown type: ";
						msg += static_cast<std::uint8_t>(type);

						throw std::runtime_error(msg);
					}
				}
			}

			// functions
			std::uint32_t numFunctions = 0;
			fin.read(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

			for(std::uint32_t i = 0; i < numFunctions; ++i)
			{
				std::uint8_t nrets;
				std::uint8_t nargs;

				fin.read(reinterpret_cast<char*>(&nrets), sizeof(nrets));
				fin.read(reinterpret_cast<char*>(&nargs), sizeof(nargs));

				std::uint32_t numInstrs;
				fin.read(reinterpret_cast<char*>(&numInstrs), sizeof(numInstrs));

				Bytecode code(numInstrs);

				fin.read(reinterpret_cast<char*>(code.data()), numInstrs * sizeof(Instruction));

				functions.emplace_back(nrets, nargs, code);
			}

			return fin.tellg() - startPos;
		}

		std::size_t Program::write(const std::string& file)
		{
			std::ofstream fout(file, std::ios::binary);

			if(!fout)
				throw std::runtime_error("Unable to open output file");

			auto startPos = fout.tellp();

			// file type identifier
			fout.write(BINARY_ID, util::strlen(BINARY_ID));

			// version
			std::uint32_t version = 0;
			fout.write(reinterpret_cast<char*>(&version), sizeof(version));	// TODO: need to figure out how I want to do versioning

			// constants
			std::uint32_t numConstants = constants.size();
			fout.write(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

			for(auto& c : constants)
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
					String str = c;
					std::uint32_t len = str.length();

					fout.write(reinterpret_cast<char*>(&len), sizeof(len));
					fout.write(str.c_str(), len);
				}
			}

			// functions
			std::uint32_t numFunctions = functions.size();
			fout.write(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

			for(auto& f : functions)
			{
				std::uint8_t nrets = f.returns();
				std::uint8_t nargs = f.args();

				fout.write(reinterpret_cast<const char*>(&nrets), sizeof(nrets));
				fout.write(reinterpret_cast<const char*>(&nargs), sizeof(nargs));

				std::uint32_t numInstrs = f.length();

				auto& code = f.bytecode();
				fout.write(reinterpret_cast<const char*>(&numInstrs), sizeof(numInstrs));
				fout.write(reinterpret_cast<const char*>(code.data()), numInstrs * sizeof(Instruction));
			}

			return fout.tellp() - startPos;
		}
	}
}
