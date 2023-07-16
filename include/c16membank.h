#ifndef __c16membank_H__
#define __c16membank_H__

#include <c16memmap.h>
#include <c16cpu.h>
#include <stdint.h>

// Global variables for memory bank
// Only used in c16membank.c
extern c16cpu_t *c16membank_cpu;
extern uint16_t c16membank_bankCount;
extern uint16_t c16membank_bankSize;

C16MemoryAccessDevice *c16membank_createDevice(size_t n, size_t bankSize, c16cpu_t *cpu);

uint16_t _c16membank_getActualAddress(uint16_t address);

uint16_t _c16membank_getUint16(void *memory, uint16_t address);
void _c16membank_setUint16(void *memory, uint16_t address, uint16_t value);
uint8_t _c16membank_getUint8(void *memory, uint16_t address);
void _c16membank_setUint8(void *memory, uint16_t address, uint8_t value);

#endif // __c16membank_H__