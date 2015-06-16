#ifndef STRUCTURES_H
#define STRUCTURES_H

struct cp_info 
{ 
	unsigned char  tag; 
	unsigned char* info; 
};

struct CONSTANT_Class_info 
{
	unsigned char tag;
	unsigned short name_index;
};

struct CONSTANT_Fieldref_info
{
	unsigned char tag;
	unsigned short class_index;
	unsigned short name_and_type_index;
};


struct CONSTANT_Methodref_info
{
	unsigned char tag;
	unsigned short class_index;
	unsigned short name_and_type_index;
};

struct CONSTANT_InterfaceMethodref_info
{
	unsigned char tag;
	unsigned short class_index;
	unsigned short name_and_type_index;
};

struct CONSTANT_String_info
{
	unsigned char tag;
	unsigned short string_index;
};

struct CONSTANT_Integer_info
{
	unsigned char tag;
	unsigned int bytes;
};

struct CONSTANT_Float_info
{
	unsigned char tag;
	unsigned int bytes;
};

struct CONSTANT_Long_info
{
	unsigned char tag;
	unsigned int high_bytes;
	unsigned int low_bytes;
};

struct CONSTANT_Double_info
{
	unsigned char tag;
	unsigned int high_bytes;
	unsigned int low_bytes;
};
	
struct CONSTANT_NameAndType_info
{
	unsigned char tag;
	unsigned short name_index;
	unsigned short descriptor_index;
};

struct CONSTANT_Utf8_info
{
	unsigned char tag;
	unsigned short length;
	unsigned char* bytes;//[length];
};

//----------------------------------------------------------

struct attribute_info
{
	unsigned short attribute_name_index;
	unsigned int   attribute_length;
	unsigned char* info;//[attribute_length];
};

struct field_info 
{ 
	unsigned short access_flags; 
	unsigned short name_index; 
	unsigned short descriptor_index; 
	unsigned short attributes_count; 
	attribute_info* attributes; //[attributes_count]; 
};

struct field_info_ex: public field_info
{
	field_info* pFieldInfoBase;
};

struct Exception_table
{
	unsigned short start_pc;
	unsigned short end_pc;
    unsigned short handler_pc;
    unsigned short catch_type;
};

struct Code_attribute
{
	unsigned short attribute_name_index;
	unsigned int attribute_length;
	unsigned short max_stack;
	unsigned short max_locals;
	unsigned int code_length;
	unsigned char* code; //[code_length];
	unsigned short exception_table_length;
	Exception_table* exception_table; //[exception_table_length];
	unsigned short attributes_count;
	attribute_info* attributes; //[attributes_count];
};

struct method_info 
{ 
	unsigned short  access_flags; 
	unsigned short  name_index; 
	unsigned short  descriptor_index; 
	unsigned short  attributes_count; 
	attribute_info* attributes; //[attributes_count]; 
};

struct method_info_ex: public method_info
{
	method_info* pMethodInfoBase;
	Code_attribute* pCode_attr;
};

//----------------------------------------------------------

struct JavaClassFileFormat 
{ 
	unsigned int     magic; 
	unsigned short   minor_version; 
	unsigned short   major_version; 
	unsigned short   constant_pool_count; 
	cp_info**        constant_pool; //[constant_pool_count-1]; 
	unsigned short   access_flags; 
	unsigned short   this_class; 
	unsigned short   super_class; 
	unsigned short   interfaces_count; 
	unsigned short*  interfaces; //[interfaces_count]; 
	unsigned short   fields_count; 
	field_info_ex*   fields; //[fields_count]; 
	unsigned short   methods_count; 
	method_info_ex*  methods; //[methods_count]; 
	unsigned short   attributes_count; 
	attribute_info** attributes; //[attributes_count]; 
};

#endif // STRUCTURES_H