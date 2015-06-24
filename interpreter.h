#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "frame.h"

//Instructions of our program
enum OPC
{
	NOP = 0x00, 
	
	ICONST_0 = 0x03,
	ICONST_1= 0x04,
	ICONST_4 = 0x07,
	ICONST_5 = 0x08,
	ICONST_M1 = 0x02,
	
	ISTORE_0 = 0x3b,
	ISTORE_1 = 0x3c,
	ISTORE_2 = 0x3d,
	
	ILOAD_0 = 0x1a,
	ILOAD_1 = 0x1b,
	ILOAD_2 = 0x1c,
	
	INVOKESPECIAL = 0xb7,
	INVOKESTATIC = 0xb8,
	
	IF_ICMPGE = 0xa2,
	ISHL = 0x78,
	IINC = 0x84,
	GOTO = 0xa7,
	IFLT = 0x9b,
	ALOAD_0 = 0x2a,
	BIPUSH = 0x10,
	
	RETURN = 0xb1
};

class Interpreter
{
public:
	Interpreter();
	virtual ~Interpreter();
	
	void execute(Frame* frame);
	void printDebuggingInformations(bool print);
	
private:
	bool printDInfo;
};

#endif // INTERPRETER_H
