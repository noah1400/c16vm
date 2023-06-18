#include <c16memory.h>
#include <c16cpu.h>
#include <c16consts.h>
#include <c16screen.h>

void debugCallback(c16cpu_t *cpu)
{
    c16cpu_step(cpu);
    printf("ACC: ");
    c16cpu_viewMemoryAt(cpu, c16cpu_getRegister(cpu, "ACC"), 8);
    // c16cpu_debug(cpu);
    // c16cpu_viewMemoryAt(cpu, c16cpu_getRegister(cpu, "IP"), 8);
    // c16cpu_viewMemoryAt(cpu, 0xffff - 1 - 42, 44);
}

void writeCharToScreen(void *memory, size_t *i, char ch, uint8_t command, uint8_t position)
{
    c16memory_writeToMemory(memory, (*i)++, MOV_LIT_REG);
    c16memory_writeToMemory(memory, (*i)++, command);
    c16memory_writeToMemory(memory, (*i)++, ch);
    c16memory_writeToMemory(memory, (*i)++, R1);

    c16memory_writeToMemory(memory, (*i)++, MOV_REG_MEM);
    c16memory_writeToMemory(memory, (*i)++, R1);
    c16memory_writeToMemory(memory, (*i)++, 0x30);
    c16memory_writeToMemory(memory, (*i)++, position);
}

int main(void)
{
    printf("C16VM\n");
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    C16MemoryAccessDevice *memory = c16memory_createDevice(256*256);
    void *data = memory->data;
    printf("Memory: %p\n", data);
    C16MemoryAccessDevice *screen = c16screen_createDevice();
    c16memmap_map(mapper, memory, 0, 0xffff, 0);
    c16memmap_map(mapper, screen, 0x3000, 0x30ff, 1);
    printf("Mapped memory\n");
    c16cpu_t *cpu = c16cpu_create(mapper);
    printf("Created CPU\n");
    uint16_t waitSubroutineAddress = 0x3100;
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
        c16memory_writeToMemory(data, i++, PSH_LIT);
        c16memory_writeToMemory(data, i++, 0x00);
        c16memory_writeToMemory(data, i++, 0x00);

        // call subroutine
        c16memory_writeToMemory(data, i++, CAL_LIT);
        c16memory_writeToMemory(data, i++, (waitSubroutineAddress & 0xff00) >> 8);
        c16memory_writeToMemory(data, i++, (waitSubroutineAddress & 0x00ff));
    }

    // Jump to the beginning of the program
    c16memory_writeToMemory(data, i++, MOV_LIT_REG);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, IP);

    i = waitSubroutineAddress;

    // R1 is a constant 1, which we add to the accumulator
    c16memory_writeToMemory(data, i++, MOV_LIT_REG);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x01);
    c16memory_writeToMemory(data, i++, R1);

    // Acc starts at zero
    c16memory_writeToMemory(data, i++, MOV_LIT_REG);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, 0x00);
    c16memory_writeToMemory(data, i++, ACC);

    size_t loopStart = i;

    // Add R1 to Acc
    c16memory_writeToMemory(data, i++, ADD_REG_REG);
    c16memory_writeToMemory(data, i++, ACC);
    c16memory_writeToMemory(data, i++, R1);

    // if (Acc != 0xccff) jump to the start of the loop
    c16memory_writeToMemory(data, i++, JMP_NOT_EQ);
    c16memory_writeToMemory(data, i++, 0xff);
    c16memory_writeToMemory(data, i++, 0xff);
    c16memory_writeToMemory(data, i++, (loopStart & 0xff00) >> 8);
    c16memory_writeToMemory(data, i++, (loopStart & 0x00ff));

    // return from subroutine
    c16memory_writeToMemory(data, i++, RET);

    // end of program
    // c16cpu_debug(cpu);
    // c16cpu_viewMemoryAt(cpu, c16cpu_getRegister(cpu, "IP"), 8);
    // c16cpu_viewMemoryAt(cpu, 0xffff - 1 - 42, 44);

    c16cpu_run(cpu);
    // c16cpu_attachDebugger(cpu, debugCallback);

    return 0;
}
