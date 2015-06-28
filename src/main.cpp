#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "VM.hpp"

int main(int argc, char** argv)
{
	std::ifstream fin("../tests/test.lng");
	
	lng::Lexer::Words words;
	
	std::string temp;
	while(fin >> temp)
	{
		words.push_back(temp);
	}
	
	lng::Lexer lexer;
	lng::Lexer::TokenCode tokens = lexer.run(words);
	
	for(auto& t : tokens)
	{
		std::cout << "type: " << static_cast<int>(t.type) << "\tvalue: " << t.value << '\n';
	}
	
	return 0;
	
	std::vector<std::string> args = {argv + 1, argv + argc};
	std::string file = "";

	int stackSize = 64;
	bool compile = false;
	bool run = false;

	if(args.size() == 0)
	{
		std::cout << "No arguments, use \'-h\' for help\n";
		return 1;
	}

	for(auto it = args.begin(); it != args.end();)
	{
		if(*it == "-c")
		{
			file = *(it + 1);
			it += 2;
			compile = true;
		}
		else if(*it == "-r")
		{
			file = *(it + 1);
			it += 2;
			run = true;
		}
		else if(*it == "-s")
		{
			stackSize = std::stoi(*(it + 1));
			it += 2;
		}
		else if(*it == "-h")
		{
			std::cout	<< "\'-c\' [file] to compile a file\n"
			            << "\'-r\' [file] to run a compiled file\n"
			            << "\'-s\' [num] to set the stack size (defaults to 64)\n"
			            << "\'-h\' to display this message\n";
			it++;
		}
		else
		{
			std::cout << "Unknown argument\n";
		}
	}

	lng::VM vm(stackSize);

	lng::Parser parser;

	lng::VM::Bytecode bytes;

	if(compile)
	{
		bytes = parser.parseFile(file);
	}

	if(run)
	{
		bytes = vm.loadBytes(file);
		vm.run(bytes);
	}

	return 0;
}
