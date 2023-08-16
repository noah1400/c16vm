# c16vm

**Note:** This project is no longer actively developed. It was an exploration into 16-bit virtual machine concepts. Please check out the upgraded version with expanded functionality and a larger address range: [c64vm](https://www.github.com/noah1400/c64vm).

## Introduction

**c16vm** is a 16-bit virtual machine implemented in C. This project aimed to provide insights into memory maps, stacks, instructions, and the mechanics of virtual machines. However, as development progressed, the limitations of the 16-bit address range became evident, prompting a pause in this project. The decision was made to transition to an upgraded version with a larger address range, resulting in the creation of the [c64vm](https://www.github.com/noah1400/c64vm) project.

## Build

To build **c16vm**, follow these steps:

1. Clone this repository to your local machine.
2. Navigate to the repository directory.
3. Run the following command to compile the project:

    ```sh
    gcc -o c16vm c16memory.c c16cpu.c c16memmap.c c16screen.c c16membank.c c16graphics.c c16vm.c c16main.c -Iinclude  -Wall -Werror -Wpedantic
    ```

    This compilation process is compatible with both Linux and Windows machines.

## Instruction Set

The complete list of instructions and their corresponding opcodes can be found in [c16instructions.h](https://github.com/noah1400/c16vm/blob/main/include/c16instructions.h). Implementation details for all instructions are available in [c16cpu.c](https://github.com/noah1400/c16vm/blob/main/c16cpu.c#L211).

---

**Disclaimer:** This project is no longer being actively developed. While this **c16vm** repository remains for reference purposes, its development is now in a read-only state. For an upgraded and more capable version of this project, currently under development, check out [c64vm](https://www.github.com/noah1400/c64vm).
