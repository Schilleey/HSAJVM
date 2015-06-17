#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "constants.h"
#include "opcodes.h"
#include "utils.h"
#include "javaclass.h"

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
	RETURN = 0xb1
	
	
};

void printInformations()
{
	int version_major = 0;
	int version_minor = 0;
	std::cout << "HSAJVM - Hochschule Augsburg Java Virtual Machine" << std::endl;
	std::cout << "Developers: Florian Haag, Thomas Gebele, Patrick Link" << std::endl;
	std::cout << "Version: " << version_major << "." << version_minor << std::endl << std::endl;
}

//----------------------------------------------------------

int main(int argc, char** argv)
{	
	printInformations();
	
	if(argc < 2)
	{
		std::cout << "ERROR: No class specified..." << std::endl;
		return -1;
	}
	
	std::string className(argv[1]);
	className.append(".class");
	
	std::ifstream bytestream;
	bytestream.open(className.c_str(), std::ifstream::in | std::ifstream::binary);

	if (bytestream.fail())
	{
	    std::cout << "ERROR: Cannot open the file..." << std::endl;
	    return -1;
	}

	bytestream.seekg(0, bytestream.end);
	int length = bytestream.tellg();
	bytestream.seekg(0, bytestream.beg);
	char* bytes = new char[length];

	bytestream.read(bytes, length);
	bytestream.close();
	
	// Create the JavaClass
	JavaClass* javaclass = new JavaClass;
	bool no_error = javaclass->parseJavaClass(bytes);
	
	if(!no_error)
	{
		std::cout << "ERROR: Parsing java class..." << std::endl;
		return -1;
	}
	
	std::cout << std::endl;
	
	std::cout << "Informations" << std::endl;
	std::cout << "============" << std::endl << std::endl;

	//std::cout << length << std::endl;
	std::cout << "Magic: " << std::hex << javaclass->magic << std::endl;
	std::cout << "Minor version: " << std::hex << javaclass->minor_version << std::endl;
	std::cout << "Major version: " << std::hex << javaclass->major_version << std::endl;
	
	std::cout << std::endl;
	
	std::cout << "Constant pool" << std::endl;
	std::cout << "=============" << std::endl << std::endl;
	
	for(unsigned int i = 1; i < javaclass->constant_pool_count; i++)
	{
		std::cout << "Pool " << i << " type: " << std::dec << (int)javaclass->constant_pool[i]->tag << std::endl;
		if(javaclass->constant_pool[i]->tag == 1)
		{
			std::string poolValue;
			javaclass->getStringFromConstPool(i, poolValue);
			std::cout << "String at " << i << " => " << poolValue << std::endl;
		}
	}
	
	std::cout << std::endl;
	
	std::cout << "Fields" << std::endl;
	std::cout << "======" << std::endl << std::endl;
	
	for(unsigned int i = 0; i < javaclass->fields_count; i++)
	{
		std::string fieldName, desc;
		javaclass->getStringFromConstPool(javaclass->fields[i].name_index, fieldName);
		javaclass->getStringFromConstPool(javaclass->fields[i].descriptor_index, desc);
		std::cout << "Field: " << fieldName << ", " << desc << std::endl;
		
		for(unsigned int j = 0; j < javaclass->fields[i].attributes_count; j++)
		{
			std::string fieldAttribName;
			unsigned short attrib_name_index = javaclass->fields[i].attributes[j].attribute_name_index;
			javaclass->getStringFromConstPool(attrib_name_index, fieldAttribName);
			
			if(fieldAttribName == "ConstantValue")
			{
				unsigned short value_type = getU2(javaclass->fields[i].attributes[j].info);
				int field_type, value;
				javaclass->getConstantValueFromConstPool(value_type, field_type, value);
				std::cout << "Attribute " << j << ": Type = " << fieldAttribName << ", Value type = " << field_type << " Value = " << value << std::endl;
			}
		}
	}
	
	std::cout << std::endl;
	
	std::cout << "Code" << std::endl;
	std::cout << "====" << std::endl << std::endl;
	
	for(unsigned int i = 0; i < javaclass->methods_count; i++)
	{
		std::string methodName;
		javaclass->getStringFromConstPool(javaclass->methods[i].name_index, methodName);
		
		if(javaclass->methods[i].pCode_attr != NULL)
		{
			std::cout << "Begin code (" << methodName << ")" << std::endl;
			for(unsigned int j = 0; j < javaclass->methods[i].pCode_attr->code_length; j++)
			{
				std::cout << "OpCode: " << std::hex << (int)javaclass->methods[i].pCode_attr->code[j];
				//Here 4 testing start with operation Code				
				switch((int)javaclass->methods[i].pCode_attr->code[j])
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
					default:
						std::cout << " !!! UNDEFINED_NOT_IN_LIST !!!" << std::endl;
						break;
				}
			}
			std::cout << "End code" << std::endl << std::endl;
		}
		else if(javaclass->methods[i].access_flags && ACC_NATIVE)
		{
			std::cout << "Method '" << methodName << "' is native!" << std::endl << std::endl;
		}
	}
	return 0;
}

