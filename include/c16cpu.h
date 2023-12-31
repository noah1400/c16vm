#ifndef _c16cpu_h_
#define _c16cpu_h_

#include <string.h>
#include <stdint.h>
#include <c16memory.h>
#include <stdlib.h>
#include <c16instructions.h>
#include <c16consts.h>
#include <c16memmap.h>
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(x) Sleep(x)
#else
    #include <unistd.h>
    #include <time.h>
    #define SLEEP_MS(x) usleep(x*1000)
#endif



typedef struct _c16cpu_t
{
    C16MemoryMap *memory;
    void *registers;
    char *regNames[REG_COUNT];
    size_t stackFrameSize;
    uint16_t interuptVectorAddress;
    char isInInteruptHandler;
} c16cpu_t;

typedef enum
{
    IP = 0,
    ACC = 1,
    R1 = 2,
    R2 = 3,
    R3 = 4,
    R4 = 5,
    R5 = 6,
    R6 = 7,
    R7 = 8,
    R8 = 9
} reg_t;

c16cpu_t *c16cpu_create(C16MemoryMap *memoryMap, uint16_t interuptVectorAddress);

int c16cpu_mapRegisterToOffset(c16cpu_t *cpu, char *regName);
reg_t c16cpu_mapRegisterToEnum(c16cpu_t *cpu, char *regName);

uint16_t c16cpu_getRegister(c16cpu_t *cpu, char *regName);
void c16cpu_setRegister(c16cpu_t *cpu, char *regName, uint16_t value);

uint8_t c16cpu_fetch(c16cpu_t *cpu);
uint16_t c16cpu_fetch16(c16cpu_t *cpu);

void c16cpu_push(c16cpu_t *cpu, uint16_t value);
uint16_t c16cpu_pop(c16cpu_t *cpu);

void c16cpu_pushState(c16cpu_t *cpu);
void c16cpu_popState(c16cpu_t *cpu);

size_t c16cpu_fetchRegisterIndex(c16cpu_t *cpu);

void c16cpu_handleInterupt(c16cpu_t *cpu, uint16_t value);

int c16cpu_execute(uint8_t opcode, c16cpu_t *cpu);
void c16cpu_run(c16cpu_t *cpu, int debug);

void c16cpu_debug(c16cpu_t *cpu);
void c16cpu_viewMemoryAt(c16cpu_t *cpu, uint16_t offset, uint16_t size);
void c16cpu_viewMemoryAtWithHighlightedByte(c16cpu_t *cpu, uint16_t offset, uint16_t size, uint16_t highlightedByte);
int c16cpu_step(c16cpu_t *cpu);
void c16cpu_attachDebugger(c16cpu_t *cpu, void(*f)(c16cpu_t *cpu));

void c16cpu_destroy(c16cpu_t *cpu);

#endif // _c16cpu_h_