# c16vm

 16-bit Virtual Machine implemented in C

## Build

```txt
gcc -o c16vm c16memory.c c16cpu.c c16memmap.c c16screen.c c16membank.c c16vm.c -Iinclude -Wall -Werror -Wpedantic
```

## Instruction Set

All instructions and opcodes listed in [c16instructions.h](https://github.com/noah1400/c16vm/blob/main/include/c16instructions.h)

Implementations of all instructions are done in [c16cpu.c](https://github.com/noah1400/c16vm/blob/8defde73e1779eee78da042cab681b9b1d97651c/c16cpu.c#L173)

## Note

Compiling assembly is not working yet so don't even try

## TODO

- Custom Assembly Language
- Graphics ( probably using openGL )
