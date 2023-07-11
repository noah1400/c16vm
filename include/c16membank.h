#ifndef __c16membank_H__
#define __c16membank_H__

#include <c16memmap.h>
#include <c16cpu.h>

C16MemoryAccessDevice *c16membank_createDevice(size_t n, size_t bankSize, c16cpu_t *cpu);

uint16_t _c16membank_getUint16(void *memory, uint16_t address);
void _c16membank_setUint16(void *memory, uint16_t address, uint16_t value);
uint8_t _c16membank_getUint8(void *memory, uint16_t address);
void _c16membank_setUint8(void *memory, uint16_t address, uint8_t value);

#endif // __c16membank_H__