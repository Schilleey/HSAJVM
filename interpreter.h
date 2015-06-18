#ifndef INTERPRETER_H
#define INTERPRETER_H

//Instructions of our program
enum OPC
{
	NOP = 0x00, 
	ICONST_0 = 0x03,
	ISTORE_1 = 0x3c,
	ILOAD_1 = 0x1b,
	LCONST_1 = 0x10,
	ICONST_5 = 0x08,
	IF_ICMPGE = 0xa2,
	ILOAD_2 = 0x1c,
	ICONST_4 = 0x07,
	DCONST_0 = 0x0f,
	ICONST_1= 0x04,
	ISHL = 0x78,
	ISTORE_2 = 0x3d,
	INVOKESTATIC = 0xb8,
	ICONST_M1 = 0x02,
	IINC = 0x84,
	ACONST_NULL = 0x01,
	GOTO = 0xa7,
	IMPDEP2 = 0xff,
	IFLT = 0x9b,
	RETURN = 0xb1,
	ALOAD_0 = 0x2a,
	INVOKESPECIAL = 0xb7	
};

class Interpreter
{
public:
	Interpreter();
	virtual ~Interpreter();
	
	void execute(unsigned char opcode);
};

//----------------------------------------------------------

// OPCODES

/*
aload_0
invokespecial
invokestatic
return
iconst_0
iconst_1
iconst_4
istore_0
istore_2
iload_1
iload_2
bipush
if_icmpge
ishl
iflt
iinc
goto
*/

//----------------------------------------------------------

#endif // INTERPRETER_H