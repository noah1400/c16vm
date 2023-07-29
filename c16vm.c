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
        if (screenOffset < banksize && banked) {
            printf("Error: Screen offset must be greater than bank size.\n");
            exit(EXIT_FAILURE);
        }
        char numBytesOfRegularMemory = screenOffset - regOffset;
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

void c16vm_load(c16vm_t *vm,uint16_t address, uint8_t *data, uint16_t size)
{
    c16memmap_load(vm->mapper, address, data, size);
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

int main(void)
{
    // const int bankSize = 0xff;
    // const int nBanks = 8;
    const uint16_t screenOffset = 0xf000;
    const uint16_t interruptVector = 0x1000;
    C16MemoryMap *mapper = c16memmap_createMemoryMap();
    c16cpu_t *cpu = c16cpu_create(mapper, interruptVector);
    // C16MemoryAccessDevice *membankDevice = c16membank_createDevice(nBanks, bankSize, cpu);
    C16MemoryAccessDevice *regularMemoryDevice = c16memory_createDevice(screenOffset);
    C16MemoryAccessDevice *screenDevice = c16graphics_createDevice();
    // c16memmap_map(mapper, membankDevice, 0x0000, bankSize, FALSE);
    c16memmap_map(mapper, regularMemoryDevice, 0x0000, screenOffset, TRUE);
    c16memmap_map(mapper, screenDevice, screenOffset, 0xffff, TRUE);

    c16memmap_print(mapper);

    exit(0);
    
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

    uint8_t left = c16graphics_Left2ByteOfAddressAt(63, 2, screenOffset);
    uint8_t right = c16graphics_Right2ByteOfAddressAt(63, 2, screenOffset);

    c16memmap_load(mapper, 0x0000, (uint8_t[]){
        // mov $1234, r1 -> color value
        0x10, 0x12, 0x34, 0x02,
        // mov r1, coords(left, right)
        0x12, 0x02, left, right,
        // mov $1, r1 -> activate flag
        0x10, 0x00, 0x01, 0x02,
        // mov r1, $f000
        0x12, 0x02, 0xf0, 0x00,
        // mov $0, r1 -> deactivate flag
        0x10, 0x00, 0x00, 0x02,
        // mov r1, $f000
        0x12, 0x02, 0xf0, 0x00,
        // hlt
        0xff
    }, 25);

    // c16cpu_attachDebugger(cpu, debug);
    c16cpu_run(cpu, 0);

    return 0;
}
