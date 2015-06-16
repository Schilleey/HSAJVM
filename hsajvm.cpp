#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstring>

#include "constants.h"
#include "opcodes.h"
#include "utils.h"
#include "javaclass.h"

void printInformations()
{
	int version_major = 0;
	int version_minor = 0;
	std::cout << "HSAJVM - Hochschule Augsburg Java Virtual Machine" << std::endl;
	std::cout << "Developers: Florian Haag, Thomas Gebele, Patrick Link" << std::endl;
	std::cout << "Version: " << version_major << "." << version_minor << std::endl << std::endl;
}

void swapBytes(void *_object, size_t _size)
{
	unsigned char *start, *end;

	for ( start = (unsigned char *)_object, end = start + _size - 1; start < end; ++start, --end )
	{
		unsigned char swap = *start;
		*start = *end;
		*end = swap;
	}
}

template <class T>
void swap_bytes(T* data)
{
	unsigned char* memc = reinterpret_cast<unsigned char*>(data);
	std::reverse(memc, memc + sizeof(T));
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
				std::cout << "OpCode: " << std::hex << (int)javaclass->methods[i].pCode_attr->code[j] << std::endl;
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
