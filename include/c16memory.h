#ifndef _c16memory_h_
#define _c16memory_h_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *c16memory_createMemory(size_t sizeInBytes);

uint16_t c16memory_getUint16(void *memory, size_t offset);
void c16memory_setUint16(void *memory, size_t offset, uint16_t value);

uint8_t c16memory_getUint8(void *memory, size_t offset);
void c16memory_setUint8(void *memory, size_t offset, uint8_t value);

void c16memory_destroy(void *memory);

void c16memory_writeToMemory(void *memory, size_t offset, uint8_t value);

#endif // _c16memory_h_