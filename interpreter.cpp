#include <iostream>
#include "interpreter.h"
#include "constants.h"
#include "utils.h"

void setLeds(int state)
{	
	for(int i = 0; i < 8; i++)
	{
		std::cout << ( ((state >> i) & 1) ? "*" : "." );
	}
	std::cout << std::endl;
}

Interpreter::Interpreter()
	: printDInfo(false)
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::execute(Frame* frame)
{	
	if(frame->getMethod()->pCode_attr != NULL)
	{
		if(printDInfo) std::cout << "Begin code" << std::endl;
		
		unsigned char* opcodes = frame->getMethod()->pCode_attr->code;
		
		while(1)
		{
			if(printDInfo) std::cout << "OpCode: " << std::hex << (int)frame->getMethod()->pCode_attr->code[frame->pc];
							
			unsigned char opcode = frame->getMethod()->pCode_attr->code[frame->pc];
			
			switch((int)opcode)
			{
				case NOP:
					{
						if(printDInfo) std::cout << " NOP recognized" << std::endl;
						frame->pc++;
						break;
					}
					
				case ICONST_M1:
				case ICONST_0:
				case ICONST_1:
				case ICONST_4:
				case ICONST_5:
					{
						if(printDInfo) std::cout << " ICONST_X recognized" << std::endl;
						frame->sp++;
						//std::cout << " To be pushed: " << std::dec << ((unsigned char)opcodes[frame->pc] - ICONST_0) << std::endl;
						frame->getOpStack()->push((unsigned char)opcodes[frame->pc] - ICONST_0); 
						frame->pc++;
						break;
					}
				
				case ISTORE_0:				
				case ISTORE_1:
				case ISTORE_2:
					{
						if(printDInfo) std::cout << " ISTORE_X recognized" << std::endl;						
						int op1 = frame->getOpStack()->pop();
						frame->getLocalStore().at((unsigned char)opcodes[frame->pc] - ISTORE_0) = op1;			
						frame->pc++;
						break;	
					}
				
				case ILOAD_0:	
				case ILOAD_1:
				case ILOAD_2:
					{
						if(printDInfo) std::cout << " ILOAD_X recognized" << std::endl;
						int op1 = frame->getLocalStore().at((unsigned char)opcodes[frame->pc] - ILOAD_0);
						frame->getOpStack()->push((signed char)op1);
						frame->pc++;
						break;
					}
					
				case INVOKESPECIAL:
					{
						if(printDInfo) std::cout << " INVOKESPECIAL recognized" << std::endl;
						frame->pc++;
						break;
					}
					
				case INVOKESTATIC:
					{
						if(printDInfo) std::cout << " INVOKESTATIC recognized" << std::endl;
						int op1 = frame->getOpStack()->pop();
						if(printDInfo) std::cout << " setLeds(" << std::dec << op1 << ")" << std::endl;
						setLeds(op1);
						frame->pc += 3;
						break;
					}
					
				case IF_ICMPGE:
					{
						if(printDInfo) std::cout << " IF_ICMPGE recognized" << std::endl;
						int op2 = frame->getOpStack()->pop();
						int op1 = frame->getOpStack()->pop();
						
						//std::cout << "OP1: " << std::dec << op1 << std::endl;
						//std::cout << "OP2: " << std::dec << op2 << std::endl;
						
						if(op1 >= op2)
						{
							frame->pc += getI2(&opcodes[frame->pc+1]);
						}
						else
						{
							frame->pc += 3;
						}
						
						frame->sp -= 2;
						break;
					}
					
				case ISHL:
					{
						if(printDInfo) std::cout << " ISHL recognized" << std::endl;
						int op2 = frame->getOpStack()->pop();
						int op1 = frame->getOpStack()->pop();
						op1 = op1 << (op2 & 0x0000001f);
						//std::cout << " After shl: " << std::dec << op1 << std::endl;
						frame->getOpStack()->push(op1);
						frame->pc++;
						break;
					}
					
				case IINC:
					{
						if(printDInfo) std::cout << " IINC recognized" << std::endl;
						//std::cout << " IINC minus: " << std::dec << (int)((signed char)opcodes[frame->pc+2]) << std::endl;
						frame->getLocalStore().at((unsigned char)opcodes[frame->pc+1]) += (int)((signed char)opcodes[frame->pc+2]);
						frame->pc += 3;
						break;
					}
					
				case GOTO:
					{
						if(printDInfo) std::cout << " GOTO recognized" << std::endl;
						// Get next 2 bytes from pc+1 and convert them to short (unsigned int)
						frame->pc += (unsigned int)getI2(&opcodes[frame->pc+1]);
						break;
					}

				case IFLT:
					{
						if(printDInfo) std::cout << " IFLT recognized" << std::endl;
						signed int op1 = frame->getOpStack()->pop();
						
						//std::cout << " FLTvalue: " << std::dec << op1 << std::endl;
						
						if(op1 < 0)
						{
							//short jump = getI2(&opcodes[frame->pc+1]);
							//std::cout << "FLT ###: " << std::dec << jump << std::endl;
							frame->pc += getI2(&opcodes[frame->pc+1]);
						}
						else
						{
							frame->pc += 3;
						}
						frame->sp--;
						
						break;
					}
					
				case ALOAD_0:
					{
						if(printDInfo) std::cout << " ALOAD_0 recognized" << std::endl;
						frame->getOpStack()->push(frame->getLocalStore().at((unsigned char)opcodes[frame->pc] - ALOAD_0));
						frame->pc++;
						break;
					}
					
				case BIPUSH:
					{
						if(printDInfo) std::cout << " BIPUSH recognized" << std::endl;
						//std::cout << " BPValue: " << std::dec << (int)opcodes[frame->pc+1] << std::endl;
						frame->getOpStack()->push((int)opcodes[frame->pc+1]);
						frame->pc += 2;
						break;
					}
					
				case RETURN:
					{
						if(printDInfo) std::cout << " RETURN recognized" << std::endl;
						if(printDInfo) std::cout << "End code" << std::endl << std::endl;
						return;
					}
					
				default:
					{
						if(printDInfo) std::cout << " !!! UNDEFINED_NOT_IN_LIST !!!" << std::endl;
						frame->pc++;
						break;
					}
			}	
			
		}
		
	}
	else if(frame->getMethod()->access_flags && ACC_NATIVE)
	{
		std::cout << "Method is native!" << std::endl << std::endl;
	}
}

void Interpreter::printDebuggingInformations(bool print)
{
	printDInfo = print;
}
