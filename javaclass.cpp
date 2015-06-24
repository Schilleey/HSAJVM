#include "constants.h"
#include "utils.h"
#include "javaclass.h"

JavaClass::JavaClass()
{
}

JavaClass::~JavaClass()
{
}

bool JavaClass::parseJavaClass(char* data)
{
	int idx = 0;
	
	magic = getU4(&data[idx]); // magic
	idx += 4;
	
	if(magic != 0xCAFEBABE)
		return false;
	
	minor_version = getU2(&data[idx]); // minor_version
	idx += 2;
	major_version = getU2(&data[idx]); // major_version
	idx += 2;
	constant_pool_count = getU2(&data[idx]); // constant_pool_count
	idx += 2;
	
	// Cp info
	if(constant_pool_count > 0)
		parseCpInfo(data, idx);	
	
	access_flags = getU2(&data[idx]); // access_flags
	idx += 2;
	this_class = getU2(&data[idx]); // this_class
	idx += 2;
	super_class = getU2(&data[idx]); // super_class
	idx += 2;
	interfaces_count = getU2(&data[idx]); // interfaces_count
	idx += 2;
	
	// Interfaces
	if(interfaces_count > 0)
		parseInterfaces(data, idx);
	
	fields_count = getU2(&data[idx]); // fields_count
	idx += 2;
	
	// Field info
	if(fields_count > 0)
		parseFieldInfo(data, idx);
	
	methods_count = getU2(&data[idx]); // methods_count
	idx += 2;
	
	// Methods info
	if(methods_count > 0)
		parseMethodInfo(data, idx);
		
	attributes_count = getU2(&data[idx]); // attributes_count
	idx += 2;
	
	// Attributes info
	if(attributes_count > 0)
		parseAttributeInfo(data, idx);
		
	return true;
}

void JavaClass::parseFieldInfo(char* data, int& idx)
{
	fields = new field_info_ex[fields_count];

	if(fields == NULL) return;
	
	for(int i = 0; i < fields_count; i++)
	{
		fields[i].pFieldInfoBase = (field_info*)(&data[idx]);
		
		fields[i].access_flags = getU2(&data[idx]);
		idx += 2;
		fields[i].name_index = getU2(&data[idx]);
		idx += 2;
		fields[i].descriptor_index = getU2(&data[idx]);
		idx += 2;
		fields[i].attributes_count = getU2(&data[idx]);
		idx += 2;
		
		//std::cout << "Field " << i << " has total " << fields[i].attributes_count << " attributes" << std::endl;
		if(fields[i].attributes_count > 0)
		{
			fields[i].attributes = new attribute_info[fields[i].attributes_count];

			for(int a = 0; a < fields[i].attributes_count; a++)
			{					
				unsigned short attribute_name_index = getU2(&data[idx]);
				idx += 2;
				
				std::string attributeType;
				getStringFromConstPool(attribute_name_index, attributeType);
				
				if(attributeType == "ConstantValue")
				{
					char* ca = &data[idx];
					fields[i].attributes[a].attribute_name_index = attribute_name_index;
					fields[i].attributes[a].attribute_length = getU4(ca);
					ca += 4;
					if(fields[i].attributes[a].attribute_length > 0)
					{
						fields[i].attributes[a].info = new unsigned char[fields[i].attributes[a].attribute_length];
						memcpy(fields[i].attributes[a].info, ca, fields[i].attributes[a].attribute_length);
						ca += fields[i].attributes[a].attribute_length;
					}
				}
				
				unsigned int len = getU4(&data[idx]);
				idx += 4;
				idx += len;
			}
		}
	}
}

void JavaClass::parseMethodCodeAttribute(int nMethodIndex, Code_attribute* pCode_attr)
{
	if(methods == NULL || nMethodIndex > methods_count) return;
	char *pMi, *bc;
	pMi = bc = (char*)methods[nMethodIndex].pMethodInfoBase;
	bc += 6;
	int nAttributes = getU2(bc);
	bc += 2;
	
	if(nAttributes > 0)
	{
		//skip attributes
		for(int a = 0; a < nAttributes; a++)
		{
			unsigned short name_index = getU2(bc);
			bc += 2;		
			std::string strAttributeName;
			getStringFromConstPool(name_index, strAttributeName);
			
			//std::cout << "Attribute name: " << strAttributeName << std::endl;

			if(strAttributeName == "Code")
			{
				char* ca = bc;
				pCode_attr->attribute_name_index = name_index; //already scanned;
				pCode_attr->attribute_length = getU4(ca); ca += 4;
				pCode_attr->max_stack = getU2(ca); ca += 2;
				pCode_attr->max_locals = getU2(ca); ca += 2;
				pCode_attr->code_length = getU4(ca); ca += 4;
				if(pCode_attr->code_length > 0)
				{
					pCode_attr->code = new unsigned char[pCode_attr->code_length];
					memcpy(pCode_attr->code, ca, pCode_attr->code_length);
				}
				else
				{
					// may be native code ??
					pCode_attr->code = NULL;
				}
				ca += pCode_attr->code_length;
				pCode_attr->exception_table_length = getU2(ca); ca += 2;

				if(pCode_attr->exception_table_length > 0)
				{
					pCode_attr->exception_table = new Exception_table[pCode_attr->exception_table_length];
					for(int ext = 0; ext < pCode_attr->exception_table_length; ext++)
					{
						pCode_attr->exception_table[ext].start_pc = getU2(ca); ca += 2;
						pCode_attr->exception_table[ext].end_pc = getU2(ca); ca += 2;
						pCode_attr->exception_table[ext].handler_pc = getU2(ca); ca += 2;
						pCode_attr->exception_table[ext].catch_type = getU2(ca); ca += 2;
					}
				}				
			}

			unsigned int len = getU4(bc); bc += 4;
			bc += len;
		}
	}
}

void JavaClass::parseMethodInfo(char* data, int& idx)
{
	methods = new method_info_ex[methods_count];

	if(methods == NULL) return;
	
	for(int i = 0; i < methods_count; i++)
	{
		//methods[i] = new method_info_ex;
		methods[i].pMethodInfoBase = (method_info*)(&data[idx]);
		methods[i].access_flags = getU2(&data[idx]);     // access_flags
		idx += 2;
		methods[i].name_index = getU2(&data[idx]);       // name_index
		idx += 2;
		methods[i].descriptor_index = getU2(&data[idx]); // descriptor_index
		idx += 2;
		methods[i].attributes_count = getU2(&data[idx]); // attributes_count
		idx += 2;

		methods[i].pCode_attr = NULL;
		
		if(methods[i].attributes_count > 0)
		{
			//skip attributes
			for(int a = 0; a < methods[i].attributes_count; a++)
			{
				unsigned short name_index = getU2(&data[idx]);
				idx += 2;
				
				//printf("Attribute name index = %d\n", name_index);
				unsigned int len = getU4(&data[idx]);
				idx += 4;
				idx += len;
			}

			methods[i].pCode_attr = new Code_attribute;
			parseMethodCodeAttribute(i, methods[i].pCode_attr);
		}		
	}
}

void JavaClass::parseAttributeInfo(char* data, int& idx)
{
	attributes = new attribute_info*[attributes_count];
	
	if(methods == NULL) return;
	
	for(int i = 0; i < attributes_count; i++)
	{
		attributes[i] = (attribute_info*)(&data[idx]);
		unsigned short name_index = getU2(&data[idx]); // attribute_name_index
		idx += 2;
		unsigned int len = getU4(&data[idx]); // len
		idx += 4;
		idx += len;
		
		//std::cout << "Attribute name index = " << name_index << std::endl;
	}
}

void JavaClass::parseInterfaces(char* data, int& idx)
{	
	interfaces = new unsigned short[interfaces_count];
	
	for(int i = 0; i < interfaces_count; i++)
	{
		interfaces[i] = getU2(&data[idx]);
		idx += 2;
		std::cout << "Interface at " << (int)interfaces[i] << std::endl;
	}
}

void JavaClass::parseCpInfo(char* data, int& idx)
{	
	constant_pool = new cp_info*[constant_pool_count]; // +1 because index starts at 1

	if(constant_pool == NULL) return;
	
	for(int i = 1; i < constant_pool_count; i++)
	{
		constant_pool[i] = (cp_info*)(&data[idx]);
		
		int size = getConstantPoolSize(data, idx);
		idx += size;

		//std::cout << "Constant pool " << i << " type " << (int)constant_pool[i]->tag << std::endl;
		if(constant_pool[i]->tag == CONSTANT_Long || constant_pool[i]->tag == CONSTANT_Double)
		{
			constant_pool[i+1] = NULL;
			i++;
		}
	}
}

int JavaClass::getConstantPoolSize(char* data, int& idx)
{
	cp_info* cp = (cp_info*)(&data[idx]);

	switch(cp->tag)
	{
		case CONSTANT_Class:
			return 3;
		case CONSTANT_Fieldref:
			return 5;
		case CONSTANT_Methodref:
			return 5;
		case CONSTANT_InterfaceMethodref:
			return 5;
		case CONSTANT_String:
			return 3;
		case CONSTANT_Integer:
			return 5;
		case CONSTANT_Float:
			return 5;
		case CONSTANT_Long:
			return 9;
		case CONSTANT_Double:
			return 9;
		case CONSTANT_NameAndType:
			return 5;
		case CONSTANT_Utf8:
			return (3 + getU2(&data[idx+1]));
	}
	
	return 0;
}

void JavaClass::getStringFromConstPool(int idx, std::string& str)
{
	if(idx < 1 || idx >= constant_pool_count)
		return;
		
	if(constant_pool[idx]->tag != CONSTANT_Utf8)
		return;

	unsigned char *p = (unsigned char*)constant_pool[idx];

	short length = getU2(&p[1]);
	//std::cout << "Length: " << length << std::endl;
	char *buffer = new char[length+1];
	buffer[length] = 0;
	std::memcpy(buffer, &p[3], length);
	str.assign(buffer, length);
	
	delete buffer;
}

void JavaClass::getConstantValueFromConstPool(int idx, int& type, int& value)
{
	if(idx < 1 || idx >= constant_pool_count)
		return;
		
	if(constant_pool[idx]->tag != CONSTANT_Integer) // Needs to bee expanded to fit more types
		return;

	unsigned char *p = (unsigned char*)constant_pool[idx];
	
	type = constant_pool[idx]->tag;
	value = getU4(&p[1]);
}
