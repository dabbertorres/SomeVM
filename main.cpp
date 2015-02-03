#include <iostream>

#include "VM.hpp"

int main(int argc, char **argv)
{
	lng::VM vm(64);
	
	lng::VM::Bytecode bytes;
	
	// push 25 and store it to memory
	bytes.push_back(static_cast<byte>(lng::Instruction::PushLiteral));
	bytes.push_back(static_cast<byte>(lng::ValueType::Number));
	bytes.push_back(25);
	bytes.push_back(static_cast<byte>(lng::Instruction::Store));
	
	// push 5
	bytes.push_back(static_cast<byte>(lng::Instruction::PushLiteral));
	bytes.push_back(static_cast<byte>(lng::ValueType::Number));
	bytes.push_back(5);
	
	// push value in memory
	bytes.push_back(static_cast<byte>(lng::Instruction::PushVariable));
	bytes.push_back(0);
	
	// add the two values
	bytes.push_back(static_cast<byte>(lng::Instruction::Add));
	
	// erase the old result and the store the new result
	bytes.push_back(static_cast<byte>(lng::Instruction::Erase));
	bytes.push_back(0);
	bytes.push_back(static_cast<byte>(lng::Instruction::Store));
	
	// push 50
	bytes.push_back(static_cast<byte>(lng::Instruction::PushLiteral));
	bytes.push_back(static_cast<byte>(lng::ValueType::Number));
	bytes.push_back(50);
	
	// push our result
	bytes.push_back(static_cast<byte>(lng::Instruction::PushVariable));
	bytes.push_back(0);
	
	// is result is equal to 50?
	bytes.push_back(static_cast<byte>(lng::Instruction::Equal));
	
	// if result is equal to 50, end, else, repeat above
	bytes.push_back(static_cast<byte>(lng::Instruction::If));
	bytes.push_back(22);
	bytes.push_back(4);
	
	bytes.push_back(static_cast<byte>(lng::Instruction::End));
	
	vm.run(bytes);
	
	return 0;
}
