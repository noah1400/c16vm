#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>
#include <c16screen.h>

int main(void)
{
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    C16MemoryAccessDevice *memory = c16memory_createDevice(256*256);
    void *data = memory->data;
    C16MemoryAccessDevice *screen = c16screen_createDevice();
    c16memmap_map(mapper, memory, 0, 0xffff, 0);
    c16memmap_map(mapper, screen, 0x3000, 0x30ff, 1);
    c16cpu_t *cpu = c16cpu_create(mapper);
    uint16_t waitSubroutineAddress = 0x4100;
    size_t i = 0;

    

    return 0;
}
