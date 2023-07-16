#ifndef _c16memmap_h_
#define _c16memmap_h_
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct MemoryAccessDevice {
    void *data; // Pointer to device data - can be NULL if not needed (e.g. Screen)
    uint8_t (*getUint8)(void *data, uint16_t address);
    void (*setUint8)(void *data, uint16_t address, uint8_t value);
    uint16_t (*getUint16)(void *data, uint16_t address);
    void (*setUint16)(void *data, uint16_t address, uint16_t value);
} C16MemoryAccessDevice;

typedef struct MemoryMapRegion {
    C16MemoryAccessDevice *device;
    uint16_t start;
    uint16_t end;
    char remap;
} C16MemoryMapRegion;

typedef struct MemoryMap {
    C16MemoryMapRegion **regions;
    uint8_t regionCount;
} C16MemoryMap;

C16MemoryMap *c16memmap_createMemoryMap();
void c16memmap_destroyMemoryMap(C16MemoryMap *map);
void c16memmap_map(C16MemoryMap *map, C16MemoryAccessDevice *device, uint16_t start, uint16_t end, char remap);

C16MemoryMapRegion *c16memmap_findRegion(C16MemoryMap *map, uint16_t address);
uint16_t c16memmap_getUint16(C16MemoryMap *map, uint16_t address);
void c16memmap_setUint16(C16MemoryMap *map, uint16_t address, uint16_t value);
uint8_t c16memmap_getUint8(C16MemoryMap *map, uint16_t address);
void c16memmap_setUint8(C16MemoryMap *map, uint16_t address, uint8_t value);
void c16memmap_load(C16MemoryMap *map, uint16_t startAdress, uint8_t *data, size_t length);

#endif