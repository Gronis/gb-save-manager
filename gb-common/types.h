#ifndef TYPES_H
#define TYPES_H

typedef unsigned char bool;
#define false ((bool)0)
#define true  ((bool)1)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned int usize_t;

typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef int size_t;

#define as_u8_ptr(v) ((uint8_t*)(v))

#endif // TYPES_H
