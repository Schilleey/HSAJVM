#include <iostream>
#include "../stack.h"

int main(int argc, char** argv)
{
	std::cout << "Write on Stack" << std::endl;
	std::cout << "=============" << std::endl << std::endl;
	
	Stack<int> stack;
	for(int i = 0; i < 20; i++) {
		std::cout << i << std::endl;	
		stack.push(i);
	}
	
	std::cout << std::endl;
	std::cout << "Pop Stack Content" << std::endl;
	std::cout << "=============" << std::endl << std::endl;
	
	while(!stack.isempty())
	{
		int i = stack.pop();
		std::cout << i << std::endl;	
	}

	std::cout << "Rewrite on Stack" << std::endl;
	std::cout << "=============" << std::endl << std::endl;
	stack.push('T');
	stack.push('E');
	stack.push('S');
	stack.push('T');
	
	
	std::cout << std::endl;
	std::cout << "Stack Content" << std::endl;
	std::cout << "=============" << std::endl << std::endl;
	
	while(!stack.isempty())
	{
		char i = stack.pop();
		std::cout << i << std::endl;	
	}
}