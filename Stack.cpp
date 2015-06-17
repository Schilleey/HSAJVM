#include <iostream>

//--Stack---------------------------------------------------
template <class T>
class List
{
public:
	T elem;
	List<T>* next;
		
	List(T newelem, List<T> *newnext)
	{
		elem = newelem;
		next = newnext;
	};
};

template <class T>
class Stack
{
private:
	List<T>* top;
	List<T>* firstelem;
public:
	Stack(){
		firstelem = new List<T>(0,top);
		top = firstelem;
	}
	
	void push(T elem)
	{
		top = new List<T>(elem, top);
	}
	
	T pop()
	{
		List<T>* popelem = top;
		top = top->next;
		return popelem->elem;
	}
	
	bool isempty(){
		return top == firstelem;
	}
};

// TEST Main

int main(int argc, char** argv){

	std::cout << "Write on Stack" << std::endl;
	std::cout << "=============" << std::endl << std::endl;
	
	Stack<int> stack;
	for(int i = 0; i < 20; i++){
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