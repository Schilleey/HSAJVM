#include <vector>
#include "frame.h"

Frame::Frame(method_info_ex* method)
	: operandStack(0), pClass(0), pMethod(method)
{
	pc = 0;
	sp = 0;
	operandStack = new Stack<unsigned int>();
	localStore.resize((int)method->pCode_attr->max_locals);
}

Frame::~Frame()
{
	if(operandStack)
		delete operandStack;
}

JavaClass* Frame::getClass()
{
	return pClass;
}

method_info_ex* Frame::getMethod()
{
	return pMethod;
}

Stack<unsigned int>* Frame::getOpStack()
{
	return operandStack;
}

std::vector<unsigned int>& Frame::getLocalStore()
{
	return localStore;
}
