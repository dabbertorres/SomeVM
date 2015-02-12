#include <iostream>

#include "VM.hpp"

std::vector<std::string> handleArgs(int argc, char** argv);

int main(int argc, char **argv)
{
	std::vector<std::string> args = handleArgs(argc, argv);
	std::string file = "";
	int stackSize = 64;
	bool compile = false;
	bool load = false;
	bool run = false;
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
		else if(*it == "-l")
		{
			file = *(it + 1);
			it += 2;
			load = true;
		}
		else if(*it == "-s")
		{
			stackSize = std::stoi(*(it + 1));
			it += 2;
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
		bytes = parser.parseFile(file, true);
	else
		bytes = parser.parseFile(file, false);
	
	if(load)
		bytes = vm.loadBytes(file);
	
	if(run)
		vm.run(bytes);
	
	return 0;
}

std::vector<std::string> handleArgs(int argc, char** argv)
{
	std::vector<std::string> args;
	
	for(int i = 1; i < argc; i++)
		args.push_back(argv[i]);
	
	return args;
}