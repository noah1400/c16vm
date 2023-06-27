#ifndef _c16meta_h_
#define _c16meta_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <c16instructions.h>
#include <stdio.h>

#define c16meta_instrCount 47

// Instruction Types
#define c16instrType_litReg     0
#define c16instrType_regLit     1
#define c16instrType_regLit8    2
#define c16instrType_regReg     3
#define c16instrType_regMem     4
#define c16instrType_memReg     5
#define c16instrType_litMem     6
#define c16instrType_regPtrReg  7
#define c16instrType_litOffReg  8
#define c16instrType_noArgs     9
#define c16instrType_singleReg  10
#define c16instrType_singleLit  11

// Instruction Type Sizes
#define c16instrSize_litReg 4
#define c16instrSize_regLit 4
#define c16instrSize_regLit8 3
#define c16instrSize_regReg 3
#define c16instrSize_regMem 4
#define c16instrSize_memReg 4
#define c16instrSize_litMem 5
#define c16instrSize_regPtrReg 3
#define c16instrSize_litOffReg 5
#define c16instrSize_noArgs 1
#define c16instrSize_singleReg 2
#define c16instrSize_singleLit 3

typedef struct {
    char instruction[16];
    uint8_t opcode;
    int type;
    int size;
    char mnemonic[4]; // 3 chars + null terminator
} c16instrMeta;

c16instrMeta **c16meta_init(); // fills c16instrMetaTable
void c16meta_free(c16instrMeta **instrMetaTable); // frees c16instrMetaTable
void c16meta_sort(c16instrMeta **c16instrMetaTable);
int c16meta_compare(const void *a, const void *b);
c16instrMeta* c16meta_get(c16instrMeta **c16instrMetaTable, uint8_t opcode);

#endif