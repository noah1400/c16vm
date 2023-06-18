#include <c16screen.h>

C16MemoryAccessDevice *c16screen_createDevice()
{
    C16MemoryAccessDevice *device = malloc(sizeof(C16MemoryAccessDevice));
    if (device == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    device->getUint8 = _c16screen_getUint8;
    device->setUint8 = _c16screen_setUint8;
    device->getUint16 = _c16screen_getUint16;
    device->setUint16 = _c16screen_setUint16;
    device->data = NULL;

    return device;
}

void _c16screen_erase()
{
    printf("\x1b[2J");
}

void _c16screen_moveTo(uint8_t x, uint8_t y)
{
    printf("\x1b[%d;%dH", y, x);
}

void _c16screen_setBold()
{
    printf("\x1b[1m");
}

void _c16screen_setRegular()
{
    printf("\x1b[0m");
}

uint16_t _c16screen_getUint16(void *memory, uint16_t address)
{
    (void)memory;
    (void)address;
    return 0;
}

uint8_t _c16screen_getUint8(void *memory, uint16_t address)
{
    (void)memory;
    (void)address;
    return 0;
}

void _c16screen_setUint16(void *memory, uint16_t address, uint16_t value)
{
    (void)memory;
    uint8_t command = (value & 0xFF00) >> 8;
    uint8_t chValue = value & 0x00FF;

    if (command == 0xff)
    {
        _c16screen_erase();
    } else if (command == 0x01)
    {
        _c16screen_setBold();
    } else if (command == 0x02)
    {
        _c16screen_setRegular();
    }

    uint8_t x = (address % 16) +1;
    uint8_t y = ((int)address / 16) +1;
    _c16screen_moveTo(x*2, y);
    char ch = (char)chValue;
    printf("%c", ch);
}

void _c16screen_setUint8(void *memory, uint16_t address, uint8_t value)
{
    (void)memory;
    (void)address;
    (void)value;
}