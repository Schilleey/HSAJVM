#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "constants.h"
#include "utils.h"
#include "javaclass.h"
#include "interpreter.h"

void printInformations()
{
	int version_major = 0;
	int version_minor = 0;
	std::cout << "HSAJVM - Hochschule Augsburg Java Virtual Machine" << std::endl;
	std::cout << "Developers: Florian Haag, Thomas Gebele, Patrick Link" << std::endl;
	std::cout << "Version: " << version_major << "." << version_minor << std::endl << std::endl;
}

void printHelp()
{
	std::cout << "Usage: hsajvm [-c|-r] <Classname>" << std::endl;
}

void printJavaClassInformations(JavaClass* javaclass)
{
	std::cout << std::endl;
	
	std::cout << "Informations" << std::endl;
	std::cout << "============" << std::endl << std::endl;

	//std::cout << length << std::endl;
	std::cout << "Magic: " << std::hex << javaclass->magic << std::endl;
	std::cout << "Minor version: " << std::hex << javaclass->minor_version << std::endl;
	std::cout << "Major version: " << std::hex << javaclass->major_version << std::endl;
	
	std::cout << std::endl;
	
	std::cout << "Pool count: " << std::dec << javaclass->constant_pool_count << std::endl;
	std::cout << "Interfaces count: " << std::dec << javaclass->interfaces_count << std::endl;
	std::cout << "Fields count: " << std::dec << javaclass->fields_count << std::endl;
	std::cout << "Methods count: " << std::dec << javaclass->methods_count << std::endl;
	std::cout << "Attributes count: " << std::dec << javaclass->attributes_count << std::endl;
	
	std::cout << std::endl;
	
	std::cout << "Methods" << std::endl;
	std::cout << "=======" << std::endl << std::endl;
	
	std::string strName, strDesc;
	for(unsigned int i = 0; i < javaclass->methods_count; i++)
	{
		javaclass->getStringFromConstPool(javaclass->methods[i].name_index, strName);
		javaclass->getStringFromConstPool(javaclass->methods[i].descriptor_index, strDesc);
		
		std::cout << "Method = " << strName << " - " << strDesc << std::endl;
	}
	
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
}

//----------------------------------------------------------

int main(int argc, char** argv)
{	
	printInformations();
	
	bool pClassInformations = false;
	bool pRunInformations = false;
	
	if(argc < 2)
	{
		printHelp();
		return 0;
	}
	else if(argc < 3)
	{
	}
	else if(argc < 4)
	{
		if(std::string(argv[1]) == "-c")
		{
			pClassInformations = true;
		}
		else if(std::string(argv[1]) == "-r")
		{
			pRunInformations = true;
		}
		else if((std::string(argv[1]) == "-cr") || (std::string(argv[1]) == "-rc"))
		{
			pClassInformations = true;
			pRunInformations = true;
		}
		else
		{
			std::cout << "ERROR: Unrecognized command line argument: " << argv[1] << std::endl << std::endl;
			printHelp();
			return -1;
		}
	}
	else if(argc > 3)
	{
		std::cout << "ERROR: Too many arguments..." << std::endl << std::endl;
		printHelp();
		return -1;
	}
	else
	{
		printHelp();
		return 0;
	}
	
	std::string className(argv[argc-1]);
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

	// Read the bytecode from class file
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
	
	// JavaClass debugging informations
	if(pClassInformations)
	{
		printJavaClassInformations(javaclass);
		
		std::cout << std::endl;
		
		std::cout << "Code" << std::endl;
		std::cout << "====" << std::endl << std::endl;
	}
	
	// Method at index 2 is main
	Frame* mFrame = new Frame(&javaclass->methods[2]);
	
	// Interpreter executes the frame
	Interpreter interpreter;
	interpreter.printDebuggingInformations(pRunInformations);
	interpreter.execute(mFrame);
	
	return 0;
}

