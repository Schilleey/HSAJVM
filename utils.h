#ifndef UTILS_H
#define UTILS_H

#include <algorithm>

template <class T>
void swap_bytes(T* data)
{
	unsigned char* memc = reinterpret_cast<unsigned char*>(data);
	std::reverse(memc, memc + sizeof(T));
}

#define getU4(p) (unsigned int)( (unsigned int)((p)[0])<<24 & 0xFF000000 | (unsigned int)((p)[1])<<16 & 0x00FF0000 | (unsigned int)((p)[2])<<8 & 0x0000FF00| (unsigned int)((p)[3]) & 0x000000FF)
#define getU2(p) (unsigned short)((p)[0]<< 8 & 0x0000FF00 |(p)[1])
#define getI2(p) (short)(((p)[0]<<8)|(p)[1])

#endif // UTILS_H
