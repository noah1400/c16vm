# c16vm
 16-bit Virtual Machine implemented in c

# Instruction Set

`O`: OpCode
`R`: Register Index
`L`: Literal value
`M`: Memory address

| Instruction | Op-Code | Description | Structure |
| ----------- | ------- | ----------- | --------- |
| NOP         | 0x00    | Do nothing  | `0xOO`
| MOV_LIT_REG | 0x10    | Move literal value to register | `0xOOLLLLRR` |
| MOV_REG_REG | 0x11    | Move register to register | `0xOORRRR` |
| MOV_REG_MEM | 0x12    | Move register to memory | `0xOORRMMMM` |
| MOV_MEM_REG | 0x13    | Move memory to register | `0xOOMMMMRR` |
| ADD_REG_REG | 0x14    | Add two registers R1 + R2 -> ACC | `0xOORRRR` |
| JMP_NOT_EQ  | 0x15    | Jump if not equal | `0xOOLLLLMMMM` |
| PSH_LIT     | 0x17    | Push literal value on to the stack | `0xOOLLLL` |
| PSH_REG     | 0x18    | Push register on to the stack | `0xOORR` |
| POP         | 0x1A    | Pop value from stack into register | `0xOORR` |
| CAL_LIT     | 0x5E    | Call subroutine from literal address | `0xOOLLLL` |
| CAL_REG     | 0x5F    | Call subroutine from register value | `0xOORR` |
| RET         | 0x60    | Return from subroutine | `0xOO` |