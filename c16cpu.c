#include <c16cpu.h>
#include <c16consts.h>

c16cpu_t *c16cpu_create(C16MemoryMap *memory, uint16_t interuptVectorAddress)
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
    cpu->regNames[REG_MB] = "MB";
    cpu->regNames[REG_IM] = "IM";

    if (interuptVectorAddress == NULL)
    {
        cpu->interuptVectorAddress = 0x1000;
    } else {
        cpu->interuptVectorAddress = interuptVectorAddress;
    }
    cpu->isInInteruptHandler = FALSE;
    c16cpu_setRegister(cpu, "IM", 0xffff);

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

void c16cpu_handleInterupt(c16cpu_t *cpu, uint16_t value)
{
    const char interuptBit = value % 0xf;

    // If the interupt is masked by the interrupt mask register
    // then do not enter the interrupt handler
    const char isUnmasked = ( 1 << interuptBit ) & c16cpu_getRegister(cpu, "IM");
    if (!isUnmasked)
    {
        return;
    }

    // Calculate where in the interrupt vector we'll look
    const uint16_t vectorAddress = cpu->interuptVectorAddress + (interuptBit * sizeof(uint16_t));
    // Get the address from the interupt vector at that address
    const uint16_t interuptAddress = c16memmap_getUint16(cpu->memory, vectorAddress);

    // We only save state when not already in an interrupt
    if (!cpu->isInInteruptHandler)
    {
        // 0 = 0 args. This is just to maintain our calling convention
        // If this were a software defined interrupt, the caller is expected
        // to supply any required data in registers
        c16cpu_push(cpu, 0);
        // Save the state
        c16cpu_pushState(cpu);
    }

    cpu->isInInteruptHandler = TRUE;

    // Jump to the interupt handler
    c16cpu_setRegister(cpu, "IP", interuptAddress);
}

int c16cpu_execute(uint8_t opcode, c16cpu_t *cpu)
{
    switch(opcode)
    {
        case RET_INT:
            {
                cpu->isInInteruptHandler = FALSE;
                c16cpu_popState(cpu);
            }
            return RET_INT;
        case INT:
            // We're only looking at the  least significant nibble
            {
                const uint16_t value = c16cpu_fetch16(cpu) % 0xf;
                c16cpu_handleInterupt(cpu, value);
            }
            return INT;
        // Move literal into register
        case MOV_LIT_REG:
            {
                const uint16_t lit = c16cpu_fetch16(cpu);
                const size_t regIndex = c16cpu_fetchRegisterIndex(cpu);
                c16memory_setUint16(cpu->registers, regIndex, lit);
            }
            return MOV_LIT_REG;
        // Move register to register
        case MOV_REG_REG:
            {
                const size_t registerFrom = c16cpu_fetchRegisterIndex(cpu);
                const size_t registerTo = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, registerFrom);
                c16memory_setUint16(cpu->registers, registerTo, value);
            }
            return MOV_REG_REG;
        // Move register to memory
        case MOV_REG_MEM:
            {
                const size_t registerFrom = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, registerFrom);
                c16memmap_setUint16(cpu->memory, address, value);
            }
            return MOV_REG_MEM;
        // Move memory to register
        case MOV_MEM_REG:
            {
                const uint16_t address = c16cpu_fetch16(cpu);
                const size_t registerTo = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memmap_getUint16(cpu->memory, address);
                c16memory_setUint16(cpu->registers, registerTo, value);
            }
            return MOV_MEM_REG;
        // Move literal to memory
        case MOV_LIT_MEM:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                c16memmap_setUint16(cpu->memory, address, value);
            }
            return MOV_LIT_MEM;
        // Move register* to register
        case MOV_REG_PTR_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t ptr = c16memory_getUint16(cpu->registers, r1);
                const uint16_t value = c16memmap_getUint16(cpu->memory, ptr);
                c16memory_setUint16(cpu->registers, r2, value);
            }
            return MOV_REG_PTR_REG;
        // Move value at [literal + register] to register
        case MOV_LIT_OFF_REG:
            {
                const uint16_t baseAddress = c16cpu_fetch16(cpu);
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t offset = c16memory_getUint16(cpu->registers, r1);
                const uint16_t value = c16memmap_getUint16(cpu->memory, baseAddress + offset);
                c16memory_setUint16(cpu->registers, r2, value);
            }
            return MOV_LIT_OFF_REG;
        // Add register to register
        case ADD_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t registerValue2 = c16memory_getUint16(cpu->registers, r2);
                const uint16_t result = registerValue1 + registerValue2;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return ADD_REG_REG;
        // Add literal to register
        case ADD_LIT_REG:
            {
                const uint16_t literal = c16cpu_fetch16(cpu);
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = literal + registerValue1;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return ADD_LIT_REG;
        // Subtract literal from register value
        case SUB_LIT_REG:
            {
                const uint16_t literal = c16cpu_fetch16(cpu);
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = registerValue1 - literal;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return SUB_LIT_REG;
        // Subtract register value from literal
        case SUB_REG_LIT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t literal = c16cpu_fetch16(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = literal - registerValue1;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return SUB_REG_LIT;
        // Subtract register value from register value
        case SUB_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t registerValue2 = c16memory_getUint16(cpu->registers, r2);
                const uint16_t result = registerValue1 - registerValue2;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return SUB_REG_REG;
        // Multiply literal by register value
        case MUL_LIT_REG:
            {
                const uint16_t literal = c16cpu_fetch16(cpu);
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = literal * registerValue1;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return MUL_LIT_REG;
        // Multiply register value by register value
        case MUL_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t registerValue2 = c16memory_getUint16(cpu->registers, r2);
                const uint16_t result = registerValue1 * registerValue2;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return MUL_REG_REG;
        // Increment value in register (in place)
        case INC_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t oldValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t newValue = oldValue + 1;
                c16memory_setUint16(cpu->registers, r1, newValue);
            }
            return INC_REG;
        // Decrement value in register (in place)
        case DEC_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t oldValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t newValue = oldValue - 1;
                c16memory_setUint16(cpu->registers, r1, newValue);
            }
            return DEC_REG;
        // Left shift register by literal (in place)
        case LSF_REG_LIT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint8_t literal = c16cpu_fetch(cpu);
                const uint16_t oldValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t newValue = oldValue << literal;
                c16memory_setUint16(cpu->registers, r1, newValue);
            }
            return LSF_REG_LIT;
        // Left shift register by register (in place)
        case LSF_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t oldValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t shift = c16memory_getUint16(cpu->registers, r2);
                const uint16_t newValue = oldValue << shift;
                c16memory_setUint16(cpu->registers, r1, newValue);
            }
            return LSF_REG_REG;
        // Right shift register by literal (in place)
        case RSF_REG_LIT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint8_t literal = c16cpu_fetch(cpu);
                const uint16_t oldValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t newValue = oldValue >> literal;
                c16memory_setUint16(cpu->registers, r1, newValue);
            }
            return RSF_REG_LIT;
        // Right shift register by register (in place)
        case RSF_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t oldValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t shift = c16memory_getUint16(cpu->registers, r2);
                const uint16_t newValue = oldValue >> shift;
                c16memory_setUint16(cpu->registers, r1, newValue);
            }
            return RSF_REG_REG;
        // Bitwise AND register with literal
        case AND_REG_LIT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t literal = c16cpu_fetch16(cpu);
                const uint16_t registerValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = registerValue & literal;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return AND_REG_LIT;
        // Bitwise AND register with register
        case AND_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t registerValue2 = c16memory_getUint16(cpu->registers, r2);
                const uint16_t result = registerValue1 & registerValue2;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return AND_REG_REG;
        // Bitwise OR register with literal
        case OR_REG_LIT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t literal = c16cpu_fetch16(cpu);
                const uint16_t registerValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = registerValue | literal;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return OR_REG_LIT;
        // Bitwise OR register with register
        case OR_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t registerValue2 = c16memory_getUint16(cpu->registers, r2);
                const uint16_t result = registerValue1 | registerValue2;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return OR_REG_REG;
        // Bitwise XOR register with literal
        case XOR_REG_LIT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t literal = c16cpu_fetch16(cpu);
                const uint16_t registerValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = registerValue ^ literal;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return XOR_REG_LIT;
        // Bitwise XOR register with register
        case XOR_REG_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const size_t r2 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue1 = c16memory_getUint16(cpu->registers, r1);
                const uint16_t registerValue2 = c16memory_getUint16(cpu->registers, r2);
                const uint16_t result = registerValue1 ^ registerValue2;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return XOR_REG_REG;
        // Bitwise NOT (invert) register
        case NOT:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t registerValue = c16memory_getUint16(cpu->registers, r1);
                const uint16_t result = (~registerValue) & 0xffff;
                c16cpu_setRegister(cpu, "ACC", result);
            }
            return NOT;
        // Jump if literal not equal
        case JMP_NOT_EQ:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value != c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JMP_NOT_EQ;
        // Jump if register not equal
        case JNE_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value != c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JNE_REG;
        // Jump if literal equal
        case JEQ_LIT:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value == c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JEQ_LIT;
        // Jump if register equal
        case JEQ_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value == c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JEQ_REG;
        // Jump if literal less then
        case JLT_LIT:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value < c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JLT_LIT;
        // Jump if register less then
        case JLT_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value < c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JLT_REG;
        // Jump if literal greater then
        case JGT_LIT:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value > c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JGT_LIT;
        // Jump if register greater then
        case JGT_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value > c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JGT_REG;
        // Jump if literal less then or equal
        case JLE_LIT:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value <= c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JLE_LIT;
        // Jump if register less then or equal
        case JLE_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value <= c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JLE_REG;
        // Jump if literal greater then or equal
        case JGE_LIT:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value >= c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JGE_LIT;
        // Jump if register greater then or equal
        case JGE_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                const uint16_t address = c16cpu_fetch16(cpu);
                if (value >= c16cpu_getRegister(cpu, "ACC"))
                {
                    c16cpu_setRegister(cpu, "IP", address);
                }
            }
            return JGE_REG;
        // Push Literal
        case PSH_LIT:
            {
                const uint16_t value = c16cpu_fetch16(cpu);
                c16cpu_push(cpu, value);
            }
            return PSH_LIT;
        // Push Register
        case PSH_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16memory_getUint16(cpu->registers, r1);
                c16cpu_push(cpu, value);
            }
            return PSH_REG;
        // Pop
        case POP:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t value = c16cpu_pop(cpu);
                c16memory_setUint16(cpu->registers, r1, value);
            }
            return POP;
        // Call Literal
        case CAL_LIT:
            {
                const uint16_t address = c16cpu_fetch16(cpu);
                c16cpu_pushState(cpu);
                c16cpu_setRegister(cpu, "IP", address);
            }
            return CAL_LIT;
        // Call Register
        case CAL_REG:
            {
                const size_t r1 = c16cpu_fetchRegisterIndex(cpu);
                const uint16_t address = c16memory_getUint16(cpu->registers, r1);
                c16cpu_pushState(cpu);
                c16cpu_setRegister(cpu, "IP", address);
            }
            return CAL_REG;
        // Return from subroutine
        case RET:
            {
                c16cpu_popState(cpu);
            }
            return RET;
        // Halt all computation
        case HLT:
            {
                (void)0;
            }
            return HLT;
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