# c16vm

 16-bit Virtual Machine implemented in C

## Build

```txt
gcc -o c16vm c16memory.c c16cpu.c c16memmap.c c16screen.c c16vm.c -Iinclude -Wall -Werror -Wpedantic
```

## Instruction Set

All instructions and opcodes listed in [c16instructions.h](https://github.com/noah1400/c16vm/blob/main/include/c16instructions.h)

## TODO

- Custom Assembly Language
- Graphics ( probably using openGL )
