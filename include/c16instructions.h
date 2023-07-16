#ifndef _c16instructions_h_
#define _c16instructions_h_

#include <stdint.h>

#define RET_INT         (uint8_t) 0xFC
#define INT             (uint8_t) 0xFD
#define MOV_LIT_REG     (uint8_t) 0x10    
#define MOV_REG_REG     (uint8_t) 0x11    
#define MOV_REG_MEM     (uint8_t) 0x12    
#define MOV_MEM_REG     (uint8_t) 0x13    
#define MOV_LIT_MEM     (uint8_t) 0x1B    
#define MOV_REG_PTR_REG (uint8_t) 0x1C
#define MOV_LIT_OFF_REG (uint8_t) 0x1D
#define ADD_REG_REG     (uint8_t) 0x14
#define ADD_LIT_REG     (uint8_t) 0x3F
#define SUB_LIT_REG     (uint8_t) 0x16
#define SUB_REG_LIT     (uint8_t) 0x1E
#define SUB_REG_REG     (uint8_t) 0x1F
#define INC_REG         (uint8_t) 0x35
#define DEC_REG         (uint8_t) 0x36
#define MUL_LIT_REG     (uint8_t) 0x20
#define MUL_REG_REG     (uint8_t) 0x21
#define LSF_REG_LIT     (uint8_t) 0x26
#define LSF_REG_REG     (uint8_t) 0x27
#define RSF_REG_LIT     (uint8_t) 0x2A
#define RSF_REG_REG     (uint8_t) 0x2B
#define AND_REG_LIT     (uint8_t) 0x2E
#define AND_REG_REG     (uint8_t) 0x2F
#define OR_REG_LIT      (uint8_t) 0x30
#define OR_REG_REG      (uint8_t) 0x31
#define XOR_REG_LIT     (uint8_t) 0x32
#define XOR_REG_REG     (uint8_t) 0x33
#define NOT             (uint8_t) 0x34
#define JMP_NOT_EQ      (uint8_t) 0x15
#define JNE_REG         (uint8_t) 0x40
#define JEQ_REG         (uint8_t) 0x3E
#define JEQ_LIT         (uint8_t) 0x41
#define JLT_REG         (uint8_t) 0x42
#define JLT_LIT         (uint8_t) 0x43
#define JGT_REG         (uint8_t) 0x44
#define JGT_LIT         (uint8_t) 0x45
#define JLE_REG         (uint8_t) 0x46
#define JLE_LIT         (uint8_t) 0x47
#define JGE_REG         (uint8_t) 0x48
#define JGE_LIT         (uint8_t) 0x49
#define PSH_LIT         (uint8_t) 0x17
#define PSH_REG         (uint8_t) 0x18
#define POP             (uint8_t) 0x1A
#define CAL_LIT         (uint8_t) 0x5E
#define CAL_REG         (uint8_t) 0x5F
#define RET             (uint8_t) 0x60
#define HLT             (uint8_t) 0xFF



#endif // _c16instructions_h_