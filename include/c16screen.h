#ifndef _c16screen_h_
#define _c16screen_h_
#include <c16memmap.h>
#include <string.h>

C16MemoryAccessDevice* c16screen_createDevice();

void _c16screen_erase();
void _c16screen_moveTo(uint8_t x, uint8_t y);
void _c16screen_setBold();
void _c16screen_setRegular();

uint16_t _c16screen_getUint16(void *memory, uint16_t address);
void _c16screen_setUint16(void *memory, uint16_t address, uint16_t value);
uint8_t _c16screen_getUint8(void *memory, uint16_t address);
void _c16screen_setUint8(void *memory, uint16_t address, uint8_t value);


#endif