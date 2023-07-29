#include <c16graphics.h>


uint16_t c16graphics_screenWidth = 64;
uint16_t c16graphics_screenHeight = 64;
// at 0x0000

uint16_t c16graphics_active = 0;
// S2D_Window *c16graphics_window = NULL;

C16MemoryAccessDevice* c16graphics_createDevice()
{
    C16MemoryAccessDevice *device = malloc(sizeof(C16MemoryAccessDevice));
    if (device == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    device->getUint8 = _c16graphics_getUint8;
    device->setUint8 = _c16graphics_setUint8;
    device->getUint16 = _c16graphics_getUint16;
    device->setUint16 = _c16graphics_setUint16;

    device->data = NULL;

    return device;
}

uint8_t _c16graphics_getUint8(void *memory, uint16_t offset)
{
    (void)memory;
    (void)offset;
    return 0;
}

uint16_t _c16graphics_getUint16(void *memory, uint16_t offset)
{
    (void)memory;
    (void)offset;
    return 0;
}

void _c16graphics_setUint8(void *memory, uint16_t offset, uint8_t value)
{
    (void)memory;
    (void)offset;
    (void)value;
}

void ___c16graphics_render()
{
    (void)0;
}

void ___c16graphics_update()
{
    (void)0;
}

void __c16graphics_createWindow()
{
    // c16graphics_window = S2D_CreateWindow(
    //     "Commodore 16",
    //     c16graphics_screenWidth,
    //     c16graphics_screenHeight,
    //     ___c16graphics_update,
    //     ___c16graphics_render,
    //     0
    // );
    // if (!c16graphics_window)
    // {
    //     printf("Error: Could not create window.\n");
    //     exit(EXIT_FAILURE);
    // }
    // S2D_Show(c16graphics_window);
}

void __c16graphics_destroyWindow()
{
    // S2D_FreeWindow(c16graphics_window);
}

uint16_t c16graphics_coordsToAddress(uint8_t x, uint8_t y, uint16_t offset)
{
    uint16_t address =  y * c16graphics_screenWidth + x + 1; // add 1 to offset the 0x0000
    return address + offset;
}

uint8_t c16graphics_Left2ByteOfAddressAt(uint8_t x, uint8_t y, uint16_t offset)
{
    uint16_t address = c16graphics_coordsToAddress(x, y, offset);
    return (address & 0xFF00) >> 8;
}

uint8_t c16graphics_Right2ByteOfAddressAt(uint8_t x, uint8_t y, uint16_t offset)
{
    uint16_t address = c16graphics_coordsToAddress(x, y, offset);
    return address & 0x00FF;
}

void _c16graphics_setUint16(void *memory, uint16_t offset, uint16_t value)
{

    (void)memory;
    if (offset == 0x0000)
    {
        if (value == 0x0001 && c16graphics_active == 0)
        {
            c16graphics_active = 1;
            // __c16graphics_createWindow();
            printf("Graphics window not implemented yet. activating\n");
            return;
        }
        else if (value == 0x0000)
        {
            c16graphics_active = 0;
            // __c16graphics_destroyWindow();
            printf("Graphics window not implemented yet. deactivating\n");
            return;
        }
    }

    uint16_t val = offset - 1; // remove 1 to get the actual offset
    // see pixels as an array and value as the index
    // get the x and y coordinates from the index
    uint16_t x = val % c16graphics_screenWidth;
    uint16_t y = val / c16graphics_screenWidth;

    printf(" drawing pixel at x: %d, y: %d\n", x, y);
    printf(" with value: %x\n", value);
}