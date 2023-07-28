#ifndef _c16graphics_h_
#define _c16graphics_h_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <c16memmap.h>
#include <c16memory.h>
#include <simple2d.h>

// Global variables for graphics interface
// Only used in c16graphics.c
extern uint16_t c16graphics_screenWidth;
extern uint16_t c16graphics_screenHeight;
extern uint16_t c16graphics_active;

extern S2D_Window *c16graphics_window;

C16MemoryAccessDevice* c16graphics_createDevice();
void c16graphics_destroy(C16MemoryAccessDevice* device);

uint16_t _c16graphics_getUint16(void *memory, uint16_t offset);
void _c16graphics_setUint16(void *memory, uint16_t offset, uint16_t value);

void __c16graphics_createWindow();
void __c16graphics_destroyWindow();
void ___c16graphics_render();
void ___c16graphics_update();

uint8_t _c16graphics_getUint8(void *memory, uint16_t offset);
void _c16graphics_setUint8(void *memory, uint16_t offset, uint8_t value);


#endif // _c16graphics_h_