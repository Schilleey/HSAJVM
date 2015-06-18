#include <iostream>
#include "interpreter.h"

Interpreter::Interpreter()
{
}
Interpreter::~Interpreter()
{
}

void Interpreter::execute(unsigned char opcode)
{
	switch((int)opcode)
				{
					case NOP:
						std::cout << " NOP recognized" << std::endl;
						break;
					case ICONST_0:
						std::cout << " ICONST_0 recognized" << std::endl;
						break;						
					case ISTORE_1:
						std::cout << " ISTORE_1 recognized" << std::endl;
						break;	
					case ILOAD_1:
						std::cout << " ILOAD_1 recognized" << std::endl;
						break;
					case LCONST_1:
						std::cout << " LCONST_1 recognized" << std::endl;
						break;
					case ICONST_5:
						std::cout << " ICONST_5 recognized" << std::endl;
						break;
					case IF_ICMPGE:
						std::cout << " IF_ICMPGE recognized" << std::endl;
						break;
					case ILOAD_2:
						std::cout << " ILOAD_2 recognized" << std::endl;
						break;
					case ICONST_4:
						std::cout << " ICONST_4 recognized" << std::endl;
						break;
					case DCONST_0:
						std::cout << " DCONST_0 recognized" << std::endl;
						break;
					case ICONST_1:
						std::cout << " ICONST_1 recognized" << std::endl;
						break;
					case ISHL:
						std::cout << " ISHL recognized" << std::endl;
						break;
					case ISTORE_2:
						std::cout << " ISTORE_2 recognized" << std::endl;
						break;
					case INVOKESTATIC:
						std::cout << " INVOKESTATIC recognized" << std::endl;
						break;
					case ICONST_M1:
						std::cout << " ICONST_M1 recognized" << std::endl;
						break;
					case IINC:
						std::cout << " IINC recognized" << std::endl;
						break;
					case ACONST_NULL:
						std::cout << " ACONST_NULL recognized" << std::endl;
						break;
					case GOTO:
						std::cout << " GOTO recognized" << std::endl;
						break;
					case IMPDEP2:
						std::cout << " IMPDEP2 recognized" << std::endl;
						break;
					case IFLT:
						std::cout << " IFLT recognized" << std::endl;
						break;
					case RETURN:
						std::cout << " RETURN recognized" << std::endl;
						break;
					case ALOAD_0:
						std::cout << " RETURN recognized" << std::endl;
						break;
					case INVOKESPECIAL:
						std::cout << " RETURN recognized" << std::endl;
						break;
					default:
						std::cout << " !!! UNDEFINED_NOT_IN_LIST !!!" << std::endl;
						break;
				}	
}