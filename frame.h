#ifndef FRAME_H
#define FRAME_H

#include <vector>

#include "structures.h"
#include "javaclass.h"
#include "stack.h"

class Frame
{
public:
	Frame(method_info_ex* method);
	virtual ~Frame();
	
	JavaClass* getClass();
	method_info_ex* getMethod();
	Stack<unsigned int>* getOpStack();
	std::vector<unsigned int>& getLocalStore();
	
	unsigned int pc;
	unsigned int sp;
	
private:
	Stack<unsigned int>* operandStack;
	std::vector<unsigned int> localStore;
	JavaClass* pClass;
	method_info_ex* pMethod;
};

#endif // FRAME_H
