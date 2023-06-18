#include <c16cpu.h>
#include <c16consts.h>

c16cpu_t *c16cpu_create(C16MemoryMap *memory)
{
    c16cpu_t *cpu = malloc(sizeof(c16cpu_t));
    if (cpu == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    cpu->memory = memory;

    cpu->registers = c16memory_createMemory(REG_COUNT * sizeof(uint16_t));

    cpu->regNames[REG_IP] = "IP";
    cpu->regNames[REG_ACC] = "ACC";
    cpu->regNames[REG_R1] = "R1";
    cpu->regNames[REG_R2] = "R2";
    cpu->regNames[REG_R3] = "R3";
    cpu->regNames[REG_R4] = "R4";
    cpu->regNames[REG_R5] = "R5";
    cpu->regNames[REG_R6] = "R6";
    cpu->regNames[REG_R7] = "R7";
    cpu->regNames[REG_R8] = "R8";
    cpu->regNames[REG_SP] = "SP";
    cpu->regNames[REG_FP] = "FP";

    c16cpu_setRegister(cpu, "SP", 0xffff - 1);
    c16cpu_setRegister(cpu, "FP", 0xffff - 1);


    cpu->stackFrameSize = 0;

    return cpu;
}

int _c16cpu_getRegisterIndex(c16cpu_t *cpu, char *regName)
{
    for (int i = 0; i < REG_COUNT; i++)
    {
        if (strcmp(cpu->regNames[i], regName) == 0)
        {
            return i;
        }
    }
    return -1;
}

int c16cpu_mapRegisterToOffset(c16cpu_t *cpu, char *regName)
{
    int index = _c16cpu_getRegisterIndex(cpu, regName);
    if (index == -1)
    {
        return -1;
    }
    return index * sizeof(uint16_t);
}

reg_t c16cpu_mapRegisterToEnum(c16cpu_t *cpu, char *regName)
{
    int index = _c16cpu_getRegisterIndex(cpu, regName);
    if (index == -1)
    {
        return -1;
    }
    return index;
}

uint16_t c16cpu_getRegister(c16cpu_t *cpu, char *regName)
{
    int offset = c16cpu_mapRegisterToOffset(cpu, regName);
    if (offset == -1)
    {
        return -1;
    }
    return c16memory_getUint16(cpu->registers, offset);
}

void c16cpu_setRegister(c16cpu_t *cpu, char *regName, uint16_t value)
{
    int offset = c16cpu_mapRegisterToOffset(cpu, regName);
    if (offset == -1)
    {
        return;
    }
    c16memory_setUint16(cpu->registers, offset, value);
}

uint8_t c16cpu_fetch(c16cpu_t *cpu)
{
    uint16_t nextInstructionAddress = c16cpu_getRegister(cpu, "IP");
    uint8_t instruction = c16memmap_getUint8(cpu->memory, nextInstructionAddress);
    c16cpu_setRegister(cpu, "IP", nextInstructionAddress + sizeof(uint8_t));
    return instruction;
}

uint16_t c16cpu_fetch16(c16cpu_t *cpu)
{
    uint16_t nextInstructionAddress = c16cpu_getRegister(cpu, "IP");
    uint16_t instruction = c16memmap_getUint16(cpu->memory, nextInstructionAddress);
    c16cpu_setRegister(cpu, "IP", nextInstructionAddress + sizeof(uint16_t));
    return instruction;
}

void c16cpu_push(c16cpu_t *cpu, uint16_t value)
{
    uint16_t spAddress = c16cpu_getRegister(cpu, "SP");
    c16memmap_setUint16(cpu->memory, spAddress, value);
    c16cpu_setRegister(cpu, "SP", spAddress - sizeof(uint16_t));
    cpu->stackFrameSize += sizeof(uint16_t);
}

uint16_t c16cpu_pop(c16cpu_t *cpu)
{
    uint16_t nextSpAddress = c16cpu_getRegister(cpu, "SP") + sizeof(uint16_t);
    c16cpu_setRegister(cpu, "SP", nextSpAddress);
    cpu->stackFrameSize -= sizeof(uint16_t);
    return c16memmap_getUint16(cpu->memory, nextSpAddress);
}

void c16cpu_pushState(c16cpu_t *cpu)
{
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R1"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R2"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R3"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R4"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R5"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R6"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R7"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "R8"));
    c16cpu_push(cpu, c16cpu_getRegister(cpu, "IP"));
    c16cpu_push(cpu, cpu->stackFrameSize + sizeof(uint16_t));

    c16cpu_setRegister(cpu, "FP", c16cpu_getRegister(cpu, "SP"));
    cpu->stackFrameSize = 0;
}

void c16cpu_popState(c16cpu_t *cpu)
{
    uint16_t framePointerAddress = c16cpu_getRegister(cpu, "FP");
    c16cpu_setRegister(cpu, "SP", framePointerAddress);

    cpu->stackFrameSize = c16cpu_pop(cpu);
    const uint16_t sfs = cpu->stackFrameSize;

    c16cpu_setRegister(cpu, "IP", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R8", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R7", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R6", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R5", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R4", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R3", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R2", c16cpu_pop(cpu));
    c16cpu_setRegister(cpu, "R1", c16cpu_pop(cpu));

    const uint16_t nArgs = c16cpu_pop(cpu);
    for (uint16_t i = 0; i < nArgs; i++)
    {
        c16cpu_pop(cpu);
    }

    c16cpu_setRegister(cpu, "FP", framePointerAddress + sfs);
}

size_t c16cpu_fetchRegisterIndex(c16cpu_t *cpu)
{
    return (c16cpu_fetch(cpu) % REG_COUNT) * sizeof(uint16_t);
}



int c16cpu_execute(uint8_t opcode, c16cpu_t *cpu)
{
    switch (opcode)
    {
    case MOV_LIT_REG:
    {
        uint16_t literal = c16cpu_fetch16(cpu);
        uint16_t reg = c16cpu_fetchRegisterIndex(cpu);
        c16memory_setUint16(cpu->registers, reg, literal);
    }
        return MOV_LIT_REG;
    case MOV_REG_REG:
    {
        uint16_t registerFrom = c16cpu_fetchRegisterIndex(cpu);
        uint16_t registerTo = c16cpu_fetchRegisterIndex(cpu);
        uint16_t value = c16memory_getUint16(cpu->registers, registerFrom);
        c16memory_setUint16(cpu->registers, registerTo, value);
    }
        return MOV_REG_REG;
    case MOV_REG_MEM:
    {
        uint16_t registerFrom = c16cpu_fetchRegisterIndex(cpu);
        uint16_t address = c16cpu_fetch16(cpu);
        uint16_t value = c16memory_getUint16(cpu->registers, registerFrom);
        c16memmap_setUint16(cpu->memory, address, value);
    }
        return MOV_REG_MEM;
    case MOV_MEM_REG:
    {
        uint16_t address = c16cpu_fetch16(cpu);
        uint16_t registerTo = c16cpu_fetchRegisterIndex(cpu);;
        uint16_t value = c16memmap_getUint16(cpu->memory, address);
        c16memory_setUint16(cpu->registers, registerTo, value);
    }
        return MOV_MEM_REG;
    case ADD_REG_REG:
    {
        uint16_t r1 = c16cpu_fetchRegisterIndex(cpu);;
        uint16_t r2 = c16cpu_fetchRegisterIndex(cpu);;
        uint16_t r1Value = c16memory_getUint16(cpu->registers, r1);
        uint16_t r2Value = c16memory_getUint16(cpu->registers, r2);
        uint16_t result = r1Value + r2Value;
        c16cpu_setRegister(cpu, "ACC", result);
    }
        return ADD_REG_REG;
    case JMP_NOT_EQ:
    {
        uint16_t value = c16cpu_fetch16(cpu);
        uint16_t address = c16cpu_fetch16(cpu);

        uint16_t acc = c16cpu_getRegister(cpu, "ACC");

        // printf("JMP_NOT_EQ: %d != %d\n", value, acc);

        if (value != acc)
        {
            c16cpu_setRegister(cpu, "IP", address);
        }
    }
        return JMP_NOT_EQ;
    case PSH_LIT:
    {
        uint16_t value = c16cpu_fetch16(cpu);
        c16cpu_push(cpu, value);
    }
        return PSH_LIT;
    case PSH_REG:
    {
        uint16_t registerIndex = c16cpu_fetchRegisterIndex(cpu);
        uint16_t value = c16memory_getUint16(cpu->registers, registerIndex);
        c16cpu_push(cpu, value);
    }
        return PSH_REG;
    case POP:
    {
        uint16_t registerIndex = c16cpu_fetchRegisterIndex(cpu);
        uint16_t value = c16cpu_pop(cpu);
        c16memory_setUint16(cpu->registers, registerIndex, value);
    }
        return POP;
    case CAL_LIT:
    {
        uint16_t address = c16cpu_fetch16(cpu);
        c16cpu_pushState(cpu);
        c16cpu_setRegister(cpu, "IP", address);
    }
        return CAL_LIT;
    case CAL_REG:
    {
        uint16_t registerIndex = c16cpu_fetchRegisterIndex(cpu);
        uint16_t address = c16memory_getUint16(cpu->registers, registerIndex);
        c16cpu_pushState(cpu);
        c16cpu_setRegister(cpu, "IP", address);
    }
        return CAL_REG;
    case RET:
    {
        c16cpu_popState(cpu);
    }
        return RET;
        
    case NOP:
    {
        return NOP;
    }
    case HLT:
    {
        return HLT;
    }
    }

    printf("Error: Unknown opcode: 0x%02x\n", opcode);

    c16cpu_debug(cpu);
    printf("IP: 0x%04x\n", c16cpu_getRegister(cpu, "IP"));
    c16cpu_viewMemoryAtWithHighlightedByte(cpu, c16cpu_getRegister(cpu, "IP")-8, 16, c16cpu_getRegister(cpu, "IP")-1);

    exit(EXIT_FAILURE);
}

void c16cpu_debug(c16cpu_t *cpu)
{
    printf("Registers:\n");
    for (int i = 0; i < REG_COUNT; i++)
    {
        printf("%s: 0x%04x\n", cpu->regNames[i], c16cpu_getRegister(cpu, cpu->regNames[i]));
    }
    printf("\n");
}

void c16cpu_viewMemoryAt(c16cpu_t *cpu, uint16_t offset, uint16_t size)
{
    printf("0x%04x: ", offset);
    for (int i = 0; i < size; i++)
    {
        printf("%02x ", c16memmap_getUint8(cpu->memory, offset + i));
    }
    printf("\n");
}

void c16cpu_viewMemoryAtWithHighlightedByte(c16cpu_t *cpu, uint16_t offset, uint16_t size, uint16_t highlightedByte)
{
    printf("0x%04x: ", offset);
    for (int i = 0; i < size; i++)
    {
        if (offset + i == highlightedByte)
        {
            // Black background red text
            printf("\033[40m\033[91m");
        }
        printf("%02x ", c16memmap_getUint8(cpu->memory, offset + i));
        if (offset + i == highlightedByte)
        {
            printf("\033[0m");
        }
    }
    printf("\n");
}

int c16cpu_step(c16cpu_t *cpu)
{
    uint8_t opcode = c16cpu_fetch(cpu);
    return c16cpu_execute(opcode, cpu);
}


void c16cpu_attachDebugger(c16cpu_t *cpu, void(*f)(c16cpu_t *cpu))
{
    char input[100];
    while (1)
    {
        f(cpu);
        printf("\n");
        printf("Press enter to step, or q to quit\n");
        scanf("%s", input);
        if (input[0] == 'q')
        {
            break;
        }
        if (input[0] > '0' && input[0] < '9')
        {
            int steps = (int)input[0] - '0';
            for (int i = 0; i < steps; i++)
            {
                f(cpu);
            }
        }

    }

    printf("Exiting debugger\n");
}

void c16cpu_destroy(c16cpu_t *cpu)
{
    c16memmap_destroyMemoryMap(cpu->memory);
    c16memory_destroy(cpu->registers);
    free(cpu);
}

void _c16cpu_sleep_ms(double ms)
{
    SLEEP_MS(ms);
}

void c16cpu_run(c16cpu_t *cpu)
{
#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
#else
    struct timespec start, end;
#endif

    double elapsed_ms, target_ms, sleep_ms;

#ifdef _WIN32
    QueryPerformanceFrequency(&frequency);
#endif

    while(1) {
#ifdef _WIN32
        QueryPerformanceCounter(&start);
#else
        clock_gettime(CLOCK_MONOTONIC, &start);
#endif
        int opcode = c16cpu_step(cpu);
        if (opcode == HLT)
        {
            break;
        }
#ifdef _WIN32
        QueryPerformanceCounter(&end);
        elapsed_ms = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
#else
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0;
        elapsed_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;
#endif
        target_ms = 1000.0 / C16_CPU_SPEED;

        sleep_ms = target_ms - elapsed_ms;
        if (sleep_ms > 0)
        {
            _c16cpu_sleep_ms(sleep_ms);
        }
    }

}