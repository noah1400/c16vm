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

    c16memmap_load(mapper, 0x2000, (uint8_t[]){
        // mov $42, r1
        0x10, 0x00, 0x42, 0x02,
        // mov $55, r2
        0x10, 0x00, 0x55, 0x03,
        // add r1, r2
        0x14, 0x02, 0x03,
        // rti
        0xfc
    }, 12);

    c16memmap_load(mapper, 0x3000,  (uint8_t[]){
        // mov $65, r1
        0x10, 0x00, 0x65, 0x02,
        // mov $22, r2
        0x10, 0x00, 0x22, 0x03,
        // xor r1, r2
        0x33, 0x02, 0x03,
        // rti
        0xfc
    }, 12);

    c16memmap_load(mapper, 0x0000, (uint8_t[]){
        // mov $1, r1
        0x10, 0x00, 0x01, 0x02,
        // mov $2, r2
        0x10, 0x00, 0x02, 0x03,
        // mov $3, r3
        0x10, 0x00, 0x03, 0x04,
        // mov $4, r4
        0x10, 0x00, 0x04, 0x05,
        // psh $5
        0x17, 0x00, 0x05,
        // int $0
        0xfd, 0x00, 0x00,
        // pop r1
        0x1a, 0x02,
        // psh $6
        0x17, 0x00, 0x06,
        // psh $7
        0x17, 0x00, 0x07,
    }, 30);

    c16cpu_run(cpu, 1);

    return 0;
}
