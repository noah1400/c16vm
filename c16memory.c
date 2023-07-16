#include <c16memory.h>

C16MemoryAccessDevice *c16memory_createDevice(size_t sizeInBytes)
{
    C16MemoryAccessDevice *device = malloc(sizeof(C16MemoryAccessDevice));
    if (device == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    device->getUint8 = c16memory_getUint8;
    device->setUint8 = c16memory_setUint8;
    device->getUint16 = c16memory_getUint16;
    device->setUint16 = c16memory_setUint16;
    device->data = c16memory_createMemory(sizeInBytes);

    return device;
}

void *c16memory_createMemory(size_t sizeInBytes)
{
    void *buffer = malloc(sizeInBytes);
    if (buffer == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    // set all bytes to 0
    memset(buffer, 0, sizeInBytes);
    return buffer;
}

uint16_t c16memory_getUint16(void *memory, uint16_t offset)
{
    uint8_t *bytes = (uint8_t *)memory;
    uint16_t value = (bytes[offset] << 8) | bytes[offset + 1];
    return value;
}

void c16memory_setUint16(void *memory, uint16_t offset, uint16_t value)
{
    uint8_t *bytes = (uint8_t *)memory;
    bytes[offset] = (value >> 8) & 0xFF;
    bytes[offset + 1] = value & 0xFF;
}

void c16memory_setUint8(void *memory, uint16_t offset, uint8_t value)
{
    uint8_t *bytes = (uint8_t *)memory;
    bytes[offset] = value;
}

uint8_t c16memory_getUint8(void *memory, uint16_t offset)
{
    uint8_t *bytes = (uint8_t *)memory;
    uint8_t value = bytes[offset];
    return value;
}

void c16memory_destroy(void *memory)
{
    free(memory);
}

void c16memory_writeToMemory(void *memory, uint16_t offset, uint8_t value)
{
    uint8_t *bytes = (uint8_t *)memory;
    bytes[offset] = value;
}