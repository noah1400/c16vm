#include <c16membank.h>

c16cpu_t *c16membank_cpu = NULL;
uint16_t c16membank_bankCount = 0;
uint16_t c16membank_bankSize = 0;


C16MemoryAccessDevice *c16membank_createDevice(size_t n, size_t bankSize, c16cpu_t *cpu)
{
    c16membank_cpu = cpu;
    c16membank_bankSize = bankSize;
    c16membank_bankCount = n;

    C16MemoryAccessDevice *device = malloc(sizeof(C16MemoryAccessDevice));
    device->data = c16memory_createMemory(n * bankSize);

    device->getUint8 = _c16membank_getUint8;
    device->setUint8 = _c16membank_setUint8;
    device->getUint16 = _c16membank_getUint16;
    device->setUint16 = _c16membank_setUint16;

    return device;
}

uint16_t _c16membank_getActualAddress(uint16_t address)
{
    c16cpu_t *cpu = c16membank_cpu;
    uint16_t mb = c16cpu_getRegister(cpu, "MB");
    uint16_t index = mb % c16membank_bankCount;
    uint16_t offset = (index * c16membank_bankSize);
    uint16_t actualAddress = address + offset;
    return actualAddress;
}

uint16_t _c16membank_getUint16(void *memory, uint16_t address)
{
    uint16_t actualAddress = _c16membank_getActualAddress(address);

    uint8_t *bytes = (uint8_t *)memory;

    uint16_t value = (bytes[actualAddress] << 8) | bytes[actualAddress + 1];
    return value;
}

void _c16membank_setUint16(void *memory, uint16_t address, uint16_t value)
{
    uint16_t actualAddress = _c16membank_getActualAddress(address);

    uint8_t *bytes = (uint8_t *)memory;

    bytes[actualAddress] = (value >> 8) & 0xFF;
    bytes[actualAddress + 1] = value & 0xFF;
}

uint8_t _c16membank_getUint8(void *memory, uint16_t address)
{
    uint16_t actualAddress = _c16membank_getActualAddress(address);

    uint8_t *bytes = (uint8_t *)memory;

    uint8_t value = bytes[actualAddress];
    return value;
}

void _c16membank_setUint8(void *memory, uint16_t address, uint8_t value)
{
    uint16_t actualAddress = _c16membank_getActualAddress(address);

    uint8_t *bytes = (uint8_t *)memory;

    bytes[actualAddress] = value;
}