#include <c16vm.h>

c16vm_t *c16vm_create(uint16_t banksize, uint16_t nBanks, uint16_t screenOffset, uint16_t interruptVector)
{
    c16vm_t *vm = malloc(sizeof(c16vm_t));
    if (vm == NULL)
    {
        printf("Error: Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    c16cpu_t *cpu = c16cpu_create(mapper, interruptVector);
    uint16_t regOffset = 0x0000;
    char banked = banksize && nBanks;
    char screen = screenOffset && 1;
    if (banked) {
        C16MemoryAccessDevice *membankDevice = c16membank_createDevice(nBanks, banksize, cpu);
        c16memmap_map(mapper, membankDevice, 0x0000, banksize, FALSE);
        regOffset += banksize;
    }
    if (screen) {
        if (banked && screenOffset < banksize) {
            printf("Error: Screen offset must be greater than bank size.\n");
            exit(EXIT_FAILURE);
        }
        uint16_t numBytesOfRegularMemory = screenOffset - regOffset;
        C16MemoryAccessDevice *regularMemoryDevice = c16memory_createDevice(numBytesOfRegularMemory);
        c16memmap_map(mapper, regularMemoryDevice, regOffset, numBytesOfRegularMemory, TRUE);
        C16MemoryAccessDevice *graphicsDevice = c16graphics_createDevice();
        c16memmap_map(mapper, graphicsDevice, screenOffset, 0xffff, TRUE);
    } else {
        C16MemoryAccessDevice *regularMemoryDevice = c16memory_createDevice(0xffff - regOffset);
        c16memmap_map(mapper, regularMemoryDevice, regOffset, 0xffff, TRUE);
    }

    vm->banksize = banksize;
    vm->nBanks = nBanks;
    vm->screenOffset = screenOffset;
    vm->interruptVector = interruptVector;
    vm->mapper = mapper;
    vm->cpu = cpu;
    vm->debug = 0;

    return vm;
}

void c16vm_destroy(c16vm_t *vm)
{
    c16cpu_destroy(vm->cpu); // this will also destroy the mapper
    free(vm);
}

void c16vm_load(c16vm_t *vm, uint16_t address, uint8_t *data, uint16_t size)
{
    c16memmap_load(vm->mapper, address, data, size);
}

void c16vm_intVector(c16vm_t *vm, uint8_t n, uint16_t address)
{
    c16memmap_setUint16(vm->mapper, vm->interruptVector + n * sizeof(uint16_t), address);
}


void c16vm_run(c16vm_t *vm)
{
    c16cpu_run(vm->cpu, vm->debug);
}

void c16vm_printMap(c16vm_t *vm)
{
    c16memmap_print(vm->mapper);
}


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