#ifndef _c16vm_h_
#define _c16vm_h_

#include <stdint.h>
#include <stdlib.h>
#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>
#include <c16graphics.h>
#include <c16memmap.h>
#include <c16membank.h>
#include <c16instructions.h>




typedef struct _c16vm_t {
    uint16_t banksize;
    uint16_t nBanks;
    uint16_t screenOffset;
    uint16_t interruptVector;
    C16MemoryMap *mapper;
    c16cpu_t *cpu;
    char debug; // 0 = no debug, 1 = debug
} c16vm_t;

c16vm_t *c16vm_create(uint16_t banksize, uint16_t nBanks, uint16_t screenOffset, uint16_t interruptVector);
void c16vm_destroy(c16vm_t *vm);

void c16vm_load(c16vm_t *vm, uint16_t address, uint8_t *data, uint16_t size);

void c16vm_intVector(c16vm_t *vm, uint8_t n, uint16_t address);

void c16vm_run(c16vm_t *vm);

void c16vm_printMap(c16vm_t *vm);

#endif // _c16vm_h_