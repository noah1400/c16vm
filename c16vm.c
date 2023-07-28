#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>
#include <c16graphics.h>
#include <c16memmap.h>
#include <c16membank.h>
#include <c16instructions.h>

const uint16_t screenOffset = 0xfc00;


void debug(c16cpu_t *cpu)
{
    c16cpu_debug(cpu);
    c16cpu_viewMemoryAt(cpu, 0xffff - 31, 16);
    c16cpu_viewMemoryAt(cpu, 0xffff - 15, 16);
    int opcode = c16cpu_step(cpu);
    if (opcode == HLT) {
        printf("HLT\n");
        exit(0);
    }
}

// left byte
uint8_t val1(uint8_t x, uint8_t y)
{
    printf("x: %d, y: %d, offset: %x\n", x, y, screenOffset);
    uint16_t address = c16graphics_getByteToWrite(x, y);
    printf("address: %x\n", address);
    uint8_t ret =  ((address+screenOffset) >> 8) & 0xff;
    return ret;
}

// right byte
uint8_t val2(uint8_t x, uint8_t y)
{
    uint16_t address = c16graphics_getByteToWrite(x, y);
    uint8_t ret = (address + screenOffset) & 0xff;
    return ret;
}

int main(void)
{
    // const int bankSize = 0xff;
    // const int nBanks = 8;
    const uint16_t interruptVector = 0x1000;
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    c16cpu_t *cpu = c16cpu_create(mapper, interruptVector);
    // C16MemoryAccessDevice *membankDevice = c16membank_createDevice(nBanks, bankSize, cpu);
    C16MemoryAccessDevice *regularMemoryDevice = c16memory_createDevice(screenOffset);
    C16MemoryAccessDevice *screenDevice = c16graphics_createDevice();
    // c16memmap_map(mapper, membankDevice, 0x0000, bankSize, FALSE);
    c16memmap_map(mapper, regularMemoryDevice, 0x0000, screenOffset, TRUE);
    c16memmap_map(mapper, screenDevice, screenOffset, 0xffff, TRUE);

    
    // setup interupt vector
    c16memmap_setUint16(mapper, interruptVector + 0x00, 0x2000);
    c16memmap_setUint16(mapper, interruptVector + 0x02, 0x3000);

    // c16memmap_load(mapper, 0x2000, (uint8_t[]){
    //     // mov $42, r1
    //     0x10, 0x00, 0x42, 0x02,
    //     // mov $55, r2
    //     0x10, 0x00, 0x55, 0x03,
    //     // add r1, r2
    //     0x14, 0x02, 0x03,
    //     // rti
    //     0xfc
    // }, 12);

    // c16memmap_load(mapper, 0x3000,  (uint8_t[]){
    //     // mov $65, r1
    //     0x10, 0x00, 0x65, 0x02,
    //     // mov $22, r2
    //     0x10, 0x00, 0x22, 0x03,
    //     // xor r1, r2
    //     0x33, 0x02, 0x03,
    //     // rti
    //     0xfc
    // }, 12);

    // c16memmap_load(mapper, 0x0000, (uint8_t[]){
    //     // mov $1, r1
    //     0x10, 0x00, 0x01, 0x02,
    //     // mov $2, r2
    //     0x10, 0x00, 0x02, 0x03,
    //     // mov $3, r3
    //     0x10, 0x00, 0x03, 0x04,
    //     // mov $4, r4
    //     0x10, 0x00, 0x04, 0x05,
    //     // psh $5
    //     0x17, 0x00, 0x05,
    //     // int $0
    //     0xfd, 0x00, 0x00,
    //     // pop r1
    //     0x1a, 0x02,
    //     // psh $6
    //     0x17, 0x00, 0x06,
    //     // psh $7
    //     0x17, 0x00, 0x07,
    //     // hlt
    //     0xff
    // }, 31);

    c16memmap_load(mapper, 0x0000, (uint8_t[]){
        // mov $1234, r1 -> color value
        0x10, 0x12, 0x34, 0x02,
        // mov r1, coords(0, 5)
        0x12, 0x02, val2(0,5), val1(0,5),
        // mov $1, r1 -> activate flag
        0x10, 0x00, 0x01, 0x02,
        // mov r1, $ff00
        0x12, 0x02, 0xff, 0x00,
        // hlt
        0xff
    }, 17);

    c16cpu_attachDebugger(cpu, debug);
    // c16cpu_run(cpu, 0);

    return 0;
}
