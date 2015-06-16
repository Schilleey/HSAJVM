#ifndef UTILS_H
#define UTILS_H

#define getU4(p) (unsigned int)( (unsigned int)((p)[0])<<24 & 0xFF000000 | (unsigned int)((p)[1])<<16 & 0x00FF0000 | (unsigned int)((p)[2])<<8 & 0x0000FF00| (unsigned int)((p)[3]) & 0x000000FF)
#define getU2(p) (unsigned short)((p)[0]<< 8 & 0x0000FF00 |(p)[1])

#endif // UTILS_H