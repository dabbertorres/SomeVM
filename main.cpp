#include <iostream>

#include "VM.hpp"

int main(int argc, char **argv)
{
	lng::VM vm(64);
	
	lng::Parser parser;
	
	lng::VM::Bytecode bytes = parser.parseFile("../tests/test1", true);
	
	vm.run(bytes);
	
	return 0;
}
