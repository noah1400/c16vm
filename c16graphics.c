#include <c16graphics.h>


uint16_t c16graphics_screenWidth = 320;
uint16_t c16graphics_screenHeight = 200;
// at 0x0000

uint16_t c16graphics_active = 0;
S2D_Window *c16graphics_window = NULL;

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
    c16graphics_window = S2D_CreateWindow(
        "Commodore 16",
        c16graphics_screenWidth,
        c16graphics_screenHeight,
        ___c16graphics_update,
        ___c16graphics_render,
        0
    );
    if (!c16graphics_window)
    {
        printf("Error: Could not create window.\n");
        exit(EXIT_FAILURE);
    }
    S2D_Show(c16graphics_window);
}

void __c16graphics_destroyWindow()
{
    S2D_FreeWindow(c16graphics_window);
}

void _c16graphics_setUint16(void *memory, uint16_t offset, uint16_t value)
{
    (void)memory;
    if (offset == 0x0000)
    {
        if (value == 0x0001 && c16graphics_active == 0)
        {
            c16graphics_active = 1;
            __c16graphics_createWindow();
        }
        else if (value == 0x0000)
        {
            c16graphics_active = 0;
            __c16graphics_destroyWindow();
        }
    }
}