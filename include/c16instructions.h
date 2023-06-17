#ifndef _c16instructions_h_
#define _c16instructions_h_

#define NOP         (uint8_t)0x00
#define MOV_LIT_REG (uint8_t)0x10
#define MOV_REG_REG (uint8_t)0x11
#define MOV_REG_MEM (uint8_t)0x12
#define MOV_MEM_REG (uint8_t)0x13
#define ADD_REG_REG (uint8_t)0x14
#define JMP_NOT_EQ  (uint8_t)0x15
#define PSH_LIT     (uint8_t)0x17
#define PSH_REG     (uint8_t)0x18
#define POP         (uint8_t)0x1A
#define CAL_LIT     (uint8_t)0x5E
#define CAL_REG     (uint8_t)0x5F
#define RET         (uint8_t)0x60
#define HLT         (uint8_t)0xFF

#endif // _c16instructions_h_