#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>
#include <c16screen.h>

void debugCallback(c16cpu_t *cpu)
{
    c16cpu_step(cpu);
    c16cpu_debug(cpu);
    c16cpu_viewMemoryAt(cpu, 0xffff - 1 - 42, 44);
}

void writeCharToScreen(void *memory, size_t *i, char ch, uint8_t command, uint8_t position)
{
    //printf("i: %04llx -> MOV_LIT_REG 0x%02x R1\n", *i, 0x01);
    c16memory_writeToMemory(memory, (*i)++, MOV_LIT_REG);
    c16memory_writeToMemory(memory, (*i)++, command);
    c16memory_writeToMemory(memory, (*i)++, ch);
    c16memory_writeToMemory(memory, (*i)++, R1);

    //printf("i: %04llx -> MOV_REG_MEM R1 0x%04x\n", *i, position);
    c16memory_writeToMemory(memory, (*i)++, MOV_REG_MEM);
    c16memory_writeToMemory(memory, (*i)++, R1);
    c16memory_writeToMemory(memory, (*i)++, 0x30);
    c16memory_writeToMemory(memory, (*i)++, position);
}

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

    char boldValue = 0;

    for (int x = 3; x <= 15; x+=2)
    {
        boldValue = boldValue == 0 ? 1 : 0;
        // clear screen
        writeCharToScreen(data, &i, ' ', 0xff, 0);
        
        for (int index = 0; index <= 0xff; index++)
        {
            uint8_t command = (index % 2 == 0) ? 0x01 : 0x02;
            char ch = (index % x == 0) ? ' ' : '+';
            writeCharToScreen(data, &i, ch, command, index);
        }

        // no arguments for functional call
        printf("i: %04llx -> PSH_LIT 0x0000\n", i);
        c16memory_writeToMemory(data, i++, PSH_LIT);
        c16memory_writeToMemory(data, i++, 0x00);
        c16memory_writeToMemory(data, i++, 0x00);

        // call subroutine
        printf("i: %04llx -> CAL_LIT 0x%04x\n",i, waitSubroutineAddress);
        c16memory_writeToMemory(data, i++, CAL_LIT);
        c16memory_writeToMemory(data, i++, (waitSubroutineAddress & 0xff00) >> 8);
        c16memory_writeToMemory(data, i++, (waitSubroutineAddress & 0x00ff));
    }

    // Jump to the beginning of the program
    printf("i: %04llx -> JMP_LIT 0x0000\n", i);
    c16memory_writeToMemory(data, i++, MOV_LIT_REG);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, IP);

    i = waitSubroutineAddress;

    // R1 is a constant 1, which we add to the accumulator
    printf("i: %04llx -> MOV_LIT_REG 0x0001 R1\n", i);
    c16memory_writeToMemory(data, i++, MOV_LIT_REG);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x01);
    c16memory_writeToMemory(data, i++, R1);

    // Acc starts at zero
    printf("i: %04llx -> MOV_LIT_REG 0x0000 ACC\n", i);
    c16memory_writeToMemory(data, i++, MOV_LIT_REG);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, ACC);

    printf("loopstart: %04llx\n", i);
    size_t loopStart = i;

    // Add R1 to Acc
    printf("i: %04llx -> ADD_REG_REG ACC R1\n", i);
    c16memory_writeToMemory(data, i++, ADD_REG_REG);
    c16memory_writeToMemory(data, i++, ACC);
    c16memory_writeToMemory(data, i++, R1);

    // if (Acc != 0xccff) jump to the start of the loop
    printf("i: %04llx -> JMP_NOT_EQ 0x000a 0x%04llx\n", i, loopStart);
    c16memory_writeToMemory(data, i++, JMP_NOT_EQ);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x0A);
    c16memory_writeToMemory(data, i++, (loopStart & 0xff00) >> 8);
    c16memory_writeToMemory(data, i++, (loopStart & 0x00ff));

    // return from subroutine
    printf("i: %04llx -> RET\n", i);
    c16memory_writeToMemory(data, i++, RET);

    // end of program
    // c16cpu_debug(cpu);
    // c16cpu_viewMemoryAt(cpu, c16cpu_getRegister(cpu, "IP"), 8);
    // c16cpu_viewMemoryAt(cpu, 0xffff - 1 - 42, 44);

    c16cpu_run(cpu);
    // c16cpu_attachDebugger(cpu, debugCallback);

    return 0;
}
