#include "Program.hpp"

#include <istream>
#include <ostream>

namespace svm
{
	constexpr auto* BINARY_ID = ".svm";
	constexpr std::uint32_t VERSION = 0;

	std::uint64_t Program::load(std::istream& input)
	{
		if (!input)
			throw std::runtime_error("Unable to access input stream");

		auto startPos = input.tellg();

		std::string identifier(4, 0);
		input.read(&identifier[0], std::strlen(BINARY_ID));

		if (identifier != BINARY_ID)
			throw std::runtime_error("Input file is not a valid svm binary");

		// check version...
		std::uint32_t version = 0;
		input.read(reinterpret_cast<char*>(&version), sizeof(version));

		if (version != VERSION)
			throw std::runtime_error("Incompatible version");

		// constants
		std::uint64_t numConstants = 0;
		input.read(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

		for (std::uint64_t i = 0; i < numConstants; ++i)
		{
			/* TODO: better to do a 4 (?) byte number of bytes, followed by that many bytes, no matter the type
             * We are statically-typed after all
            Type type;
			input.read(reinterpret_cast<char*>(&type), sizeof(type));

			switch (type)
			{
			case Type::Nil:
				constants.emplace_back();
				break;

			case Type::Bool:
			{
				Bool val;
				input.read(reinterpret_cast<char*>(&val), sizeof(val));
				constants.emplace_back(val);
				break;
			}

			case Type::Float:
			{
				Float val;
				input.read(reinterpret_cast<char*>(&val), sizeof(val));
				constants.emplace_back(val);
				break;
			}

			// constant arrays are assumed to be Strings
			case Type::Array:
			{
				std::uint64_t len = 0;
				input.read(reinterpret_cast<char*>(&len), sizeof(len));

				Array<char> val(len);
				input.read(reinterpret_cast<char*>(&val[0]), len);

				constants.emplace_back(val);
				break;
			}

			default:
			{
				std::string msg = "Unknown type: ";
				msg += static_cast<std::uint8_t>(type);

				throw std::runtime_error(msg);
			}
			}*/
		}

		// functions
		std::uint64_t numFunctions = 0;
		input.read(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

		for (std::uint64_t i = 0; i < numFunctions; ++i)
		{
			std::uint8_t nrets;
			std::uint8_t nargs;

			input.read(reinterpret_cast<char*>(&nrets), sizeof(nrets));
			input.read(reinterpret_cast<char*>(&nargs), sizeof(nargs));

			std::uint64_t numInstrs;
			input.read(reinterpret_cast<char*>(&numInstrs), sizeof(numInstrs));

			Bytecode code(numInstrs);

			input.read(reinterpret_cast<char*>(code.data()), numInstrs * sizeof(Instruction));

			functions.emplace_back(nrets, nargs, code);
		}

		return input.tellg() - startPos;
	}

	std::uint64_t Program::write(std::ostream& output) const
	{
		if (!output)
			throw std::runtime_error("Unable to access output stream");

		auto startPos = output.tellp();

		// file type identifier
		output.write(BINARY_ID, std::strlen(BINARY_ID));

		// TODO: need to figure out how I want to do versioning
		// version
		std::uint32_t version = 0;
		output.write(reinterpret_cast<char*>(&version), sizeof(version));

		// constants
		std::uint64_t numConstants = constants.size();
		output.write(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

		for (auto& c : constants)
		{
			/* TODO: better to do a 4 (?) byte number of bytes, followed by that many bytes, no matter the type
             * We are statically-typed after all
			auto type = c.type();

			output.write(reinterpret_cast<char*>(&type), sizeof(type));

			if (type != Type::Array)
			{
				Bytes bytes = c;
				output.write(reinterpret_cast<const char*>(bytes.data()), bytes.length());
			}
			else
			{
				// constant arrays are assumed to be Strings
				Array<char> str = c;
				std::uint64_t len = str.length();

				output.write(reinterpret_cast<char*>(&len), sizeof(len));
				output.write(str.data(), len);
			} */
		}

		// functions
		std::uint64_t numFunctions = functions.size();
		output.write(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

		for (auto& f : functions)
		{
			std::uint8_t nrets = f.returns();
			std::uint8_t nargs = f.args();

			output.write(reinterpret_cast<const char*>(&nrets), sizeof(nrets));
			output.write(reinterpret_cast<const char*>(&nargs), sizeof(nargs));

			std::uint64_t numInstrs = f.length();

			auto& code = f.bytecode();
			output.write(reinterpret_cast<const char*>(&numInstrs), sizeof(numInstrs));
			output.write(reinterpret_cast<const char*>(code.data()), numInstrs * sizeof(Instruction));
		}

		return output.tellp() - startPos;
	}
}

