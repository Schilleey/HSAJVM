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
	Stack<int>* getOpStack();
	std::vector<int>& getLocalStore();
	
	unsigned int pc;
	unsigned int sp;
	
private:
	Stack<int>* operandStack;
	std::vector<int> localStore;
	JavaClass* pClass;
	method_info_ex* pMethod;
};

#endif // FRAME_H
