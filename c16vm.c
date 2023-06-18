#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>

void debugCallback(c16cpu_t *cpu)
{
    c16cpu_step(cpu);
    c16cpu_debug(cpu);
    c16cpu_viewMemoryAt(cpu, c16cpu_getRegister(cpu, "IP"), 8);
    c16cpu_viewMemoryAt(cpu, 0xffff - 1 - 42, 44);
}

int main(void)
{
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    C16MemoryAccessDevice *memory = c16memory_createDevice(256*256);
    c16memmap_map(mapper, memory, 0, 0xffff, 0);
    c16cpu_t *cpu = c16cpu_create(mapper);

    uint16_t subroutineAddress = 0x3000;

    size_t i = 0;

    /*
    * PSH_LIT 0x3333
    * PSH_LIT 0x2222
    * PSH_LIT 0x1111
    * MOV_LIT_REG 0x1234, R1
    * MOV_LIT_REG 0x5678, R4
    * PSH_LIT 0x0000
    * CAL_LIT 0x3000
    * PSH_LIT 0x4444
    * 
    * 0x3333:
    * PSH_LIT 0x0102
    * PSH_LIT 0x0304
    * PSH_LIT 0x0506
    * MOV_LIT_REG 0x0708, R1
    * MOV_LIT_REG 0x090A, R8
    * RET
    */

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x33);
    c16memmap_setUint8(mapper, i++, 0x33);

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x22);
    c16memmap_setUint8(mapper, i++, 0x22);

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x11);
    c16memmap_setUint8(mapper, i++, 0x11);

    c16memmap_setUint8(mapper, i++, MOV_LIT_REG);
    c16memmap_setUint8(mapper, i++, 0x12);
    c16memmap_setUint8(mapper, i++, 0x34);
    c16memmap_setUint8(mapper, i++, R1);

    c16memmap_setUint8(mapper, i++, MOV_LIT_REG);
    c16memmap_setUint8(mapper, i++, 0x56);
    c16memmap_setUint8(mapper, i++, 0x78);
    c16memmap_setUint8(mapper, i++, R4);

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x00);
    c16memmap_setUint8(mapper, i++, 0x00);

    c16memmap_setUint8(mapper, i++, CAL_LIT);
    c16memmap_setUint8(mapper, i++, (subroutineAddress & 0xff00) >> 8);
    c16memmap_setUint8(mapper, i++, subroutineAddress & 0x00ff);

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x44);
    c16memmap_setUint8(mapper, i++, 0x44);

    i = subroutineAddress;

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x01);
    c16memmap_setUint8(mapper, i++, 0x02);

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x03);
    c16memmap_setUint8(mapper, i++, 0x04);

    c16memmap_setUint8(mapper, i++, PSH_LIT);
    c16memmap_setUint8(mapper, i++, 0x05);
    c16memmap_setUint8(mapper, i++, 0x06);

    c16memmap_setUint8(mapper, i++, MOV_LIT_REG);
    c16memmap_setUint8(mapper, i++, 0x07);
    c16memmap_setUint8(mapper, i++, 0x08);
    c16memmap_setUint8(mapper, i++, R1);

    c16memmap_setUint8(mapper, i++, MOV_LIT_REG);
    c16memmap_setUint8(mapper, i++, 0x09);
    c16memmap_setUint8(mapper, i++, 0x0A);
    c16memmap_setUint8(mapper, i++, R8);

    c16memmap_setUint8(mapper, i++, RET);

    c16cpu_debug(cpu);
    c16cpu_viewMemoryAt(cpu, c16cpu_getRegister(cpu, "IP"), 8);
    c16cpu_viewMemoryAt(cpu, 0xffff-1-42, 44);

    c16cpu_attachDebugger(cpu, debugCallback);

    c16cpu_destroy(cpu);




    return 0;
}
