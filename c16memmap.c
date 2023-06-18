#include <c16memmap.h>

C16MemoryMap *c16memmap_createMemoryMap()
{
    C16MemoryMap *map = malloc(sizeof(C16MemoryMap));
    if (map == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    map->regionCount = 0;
    map->regions = NULL;
    return map;
}

/*
* Does not free device memory, as it is assumed that the device will be freed elsewhere.
*/
void c16memmap_destroyMemoryMap(C16MemoryMap *map)
{
    for (int i = 0; i < map->regionCount; i++)
    {
        free(map->regions[i]);
    }
    free(map->regions);
    free(map);
}

void c16memmap_map(C16MemoryMap *map, C16MemoryAccessDevice *device, uint16_t start, uint16_t end, char remap)
{
    C16MemoryMapRegion *region = malloc(sizeof(C16MemoryMapRegion));
    if (region == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    region->device = device;
    region->start = start;
    region->end = end;
    region->remap = remap;

    map->regions = realloc(map->regions, (map->regionCount + 1) * sizeof(C16MemoryMapRegion *));
    for (int i = map->regionCount; i > 0; i--)
    {
        map->regions[i] = map->regions[i - 1];
    }

    map->regions[0] = region;
    map->regionCount++;
}

C16MemoryMapRegion *c16memmap_findRegion(C16MemoryMap *map, uint16_t address)
{
    for (int i = 0; i < map->regionCount; i++)
    {
        if (address >= map->regions[i]->start && address <= map->regions[i]->end)
        {
            return map->regions[i];
        }
    }
    return NULL;
}

uint16_t c16memmap_getUint16(C16MemoryMap *map, uint16_t address)
{
    C16MemoryMapRegion *region = c16memmap_findRegion(map, address);
    if (region == NULL)
    {
        printf("Error: Could not find region for address %04X.\n", address);
        exit(EXIT_FAILURE);
    }
    uint16_t finalAddress = region->remap ? address - region->start : address;
    return region->device->getUint16(region->device->data, finalAddress);
}

void c16memmap_setUint16(C16MemoryMap *map, uint16_t address, uint16_t value)
{
    C16MemoryMapRegion *region = c16memmap_findRegion(map, address);
    if (region == NULL)
    {
        printf("Error: Could not find region for address %04X.\n", address);
        exit(EXIT_FAILURE);
    }
    uint16_t finalAddress = region->remap ? address - region->start : address;
    region->device->setUint16(region->device->data, finalAddress, value);
}

uint8_t c16memmap_getUint8(C16MemoryMap *map, uint16_t address)
{
    C16MemoryMapRegion *region = c16memmap_findRegion(map, address);
    if (region == NULL)
    {
        printf("Error: Could not find region for address %04X.\n", address);
        exit(EXIT_FAILURE);
    }
    uint16_t finalAddress = region->remap ? address - region->start : address;
    return region->device->getUint8(region->device->data, finalAddress);
}

void c16memmap_setUint8(C16MemoryMap *map, uint16_t address, uint8_t value)
{
    C16MemoryMapRegion *region = c16memmap_findRegion(map, address);
    if (region == NULL)
    {
        printf("Error: Could not find region for address %04X.\n", address);
        exit(EXIT_FAILURE);
    }
    uint16_t finalAddress = region->remap ? address - region->start : address;
    region->device->setUint8(region->device->data, finalAddress, value);
}


