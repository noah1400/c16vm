#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>
#include <c16screen.h>
#include <c16memmap.h>
#include <c16membank.h>

int main(void)
{
    const int bankSize = 0xff;
    const int nBanks = 8;
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    c16cpu_t *cpu = c16cpu_create(mapper, 0x1000);
    C16MemoryAccessDevice *membankDevice = c16membank_createDevice(nBanks, bankSize, cpu);
    C16MemoryAccessDevice *regularMemoryDevice = c16memory_createDevice(0xff01);
    c16memmap_map(mapper, membankDevice, 0x0, bankSize, FALSE);
    c16memmap_map(mapper, regularMemoryDevice, bankSize, 0xffff, TRUE);
    
    c16memmap_setUint16(mapper, 0x1000 + 0x00, 0x2000);
    c16memmap_setUint16(mapper, 0x1000 + 0x02, 0x3000);

    int data[] = {12};

    c16memmap_load(mapper, 0x2000, {
        // mov $42, r1
        0x10, 0x00, 0x42, 0x02,
        // mov $55, r2
        0x10, 0x00, 0x55, 0x03,
        // add r1, r2
        0x14, 0x02, 0x03,
        // rti
        0xfc
    },12);

    c16memmap_load(mapper, 0x3000,  {
        // mov $65, r1
        0x10, 0x00, 0x65, 0x02,
        // mov $22, r2
        0x10, 0x00, 0x22, 0x03,
        // xor r1, r2
        0x33, 0x02, 0x03,
        // rti
        0xfc
    }, 12);

    return 0;
}
