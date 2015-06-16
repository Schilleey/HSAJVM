#ifndef JAVACLASS_H
#define JAVACLASS_H

#include <string>
#include <iostream>
#include <cstring>

#include "structures.h"


class JavaClass: public JavaClassFileFormat
{
public:
	JavaClass();
	virtual ~JavaClass();
	
	bool parseJavaClass(char* data);
	void getStringFromConstPool(int idx, std::string& str);
	void getConstantValueFromConstPool(int idx, int& type, int& value);
	
private:
	void parseFieldInfo(char* data, int& idx);
	void parseMethodCodeAttribute(int nMethodIndex, Code_attribute* pCode_attr);
	void parseMethodInfo(char* data, int& idx);
	void parseAttributeInfo(char* data, int& idx);
	void parseInterfaces(char* data, int& idx);
	void parseCpInfo(char* data, int& idx);
	int getConstantPoolSize(char* data, int& idx);
};

#endif // JAVACLASS_H