#include <c16vm.h>

int main(void)
{
    c16vm_t *vm = c16vm_create(0, 0, 0xf000, 0x1000);
    c16vm_printMap(vm);
    c16vm_intVector(vm, 0, 0x2000);
    c16vm_intVector(vm, 1, 0x3000);

    uint8_t left = c16graphics_Left2ByteOfAddressAt(63, 2, vm->screenOffset);
    uint8_t right = c16graphics_Right2ByteOfAddressAt(63, 2, vm->screenOffset);

    c16vm_load(vm, 0x0000, (uint8_t[]){
        // mov $1, r1 -> activate flag
        0x10, 0x00, 0x01, 0x02,
        // mov r1, $f000
        0x12, 0x02, 0xf0, 0x00,
        // mov $1234, r1 -> color value
        0x10, 0x12, 0x34, 0x02,
        // mov r1, coords(left, right)
        0x12, 0x02, left, right,
        // mov $0, r1 -> deactivate flag
        0x10, 0x00, 0x00, 0x02,
        // mov r1, $f000
        0x12, 0x02, 0xf0, 0x00,
        // hlt
        0xff
    }, 25);

    c16vm_run(vm);

    c16vm_destroy(vm);
    return 0;
}