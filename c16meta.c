#include <c16meta.h>

c16instrMeta **c16meta_init()
{

    c16instrMeta **c16instrMetaTable = malloc(sizeof(c16instrMeta *) * c16meta_instrCount);

    int i = 0;
    c16instrMeta *meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "NOP");
    meta->opcode = NOP;
    meta->type = c16instrType_noArgs;
    meta->size = c16instrSize_noArgs;
    strcpy(meta->mnemonic, "nop");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_LIT_REG");
    meta->opcode = MOV_LIT_REG;
    meta->type = c16instrType_litReg;
    meta->size = c16instrSize_litReg;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_REG_REG");
    meta->opcode = MOV_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_REG_MEM");
    meta->opcode = MOV_REG_MEM;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_MEM_REG");
    meta->opcode = MOV_MEM_REG;
    meta->type = c16instrType_memReg;
    meta->size = c16instrSize_memReg;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_LIT_MEM");
    meta->opcode = MOV_LIT_MEM;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_REG_PTR_REG");
    meta->opcode = MOV_REG_PTR_REG;
    meta->type = c16instrType_regPtrReg;
    meta->size = c16instrSize_regPtrReg;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MOV_LIT_OFF_REG");
    meta->opcode = MOV_LIT_OFF_REG;
    meta->type = c16instrType_litOffReg;
    meta->size = c16instrSize_litOffReg;
    strcpy(meta->mnemonic, "mov");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "ADD_REG_REG");
    meta->opcode = ADD_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "add");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "ADD_LIT_REG");
    meta->opcode = ADD_LIT_REG;
    meta->type = c16instrType_litReg;
    meta->size = c16instrSize_litReg;
    strcpy(meta->mnemonic, "add");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "SUB_LIT_REG");
    meta->opcode = SUB_LIT_REG;
    meta->type = c16instrType_litReg;
    meta->size = c16instrSize_litReg;
    strcpy(meta->mnemonic, "sub");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "SUB_REG_LIT");
    meta->opcode = SUB_REG_LIT;
    meta->type = c16instrType_regLit;
    meta->size = c16instrSize_regLit;
    strcpy(meta->mnemonic, "sub");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "SUB_REG_REG");
    meta->opcode = SUB_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "sub");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "INC_REG");
    meta->opcode = INC_REG;
    meta->type = c16instrType_singleReg;
    meta->size = c16instrSize_singleReg;
    strcpy(meta->mnemonic, "inc");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "DEC_REG");
    meta->opcode = DEC_REG;
    meta->type = c16instrType_singleReg;
    meta->size = c16instrSize_singleReg;
    strcpy(meta->mnemonic, "dec");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MUL_LIT_REG");
    meta->opcode = MUL_LIT_REG;
    meta->type = c16instrType_litReg;
    meta->size = c16instrSize_litReg;
    strcpy(meta->mnemonic, "mul");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "MUL_REG_REG");
    meta->opcode = MUL_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "mul");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "LSF_REG_LIT");
    meta->opcode = LSF_REG_LIT;
    meta->type = c16instrType_regLit8;
    meta->size = c16instrSize_regLit8;
    strcpy(meta->mnemonic, "lsf");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "LSF_REG_REG");
    meta->opcode = LSF_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "lsf");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "RSF_REG_LIT");
    meta->opcode = RSF_REG_LIT;
    meta->type = c16instrType_regLit8;
    meta->size = c16instrSize_regLit8;
    strcpy(meta->mnemonic, "rsf");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "RSF_REG_REG");
    meta->opcode = RSF_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "rsf");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "AND_REG_LIT");
    meta->opcode = AND_REG_LIT;
    meta->type = c16instrType_regLit;
    meta->size = c16instrSize_regLit;
    strcpy(meta->mnemonic, "and");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "AND_REG_REG");
    meta->opcode = AND_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "and");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "OR_REG_LIT");
    meta->opcode = OR_REG_LIT;
    meta->type = c16instrType_regLit;
    meta->size = c16instrSize_regLit;
    strcpy(meta->mnemonic, "or");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "OR_REG_REG");
    meta->opcode = OR_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "or");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "XOR_REG_LIT");
    meta->opcode = XOR_REG_LIT;
    meta->type = c16instrType_regLit;
    meta->size = c16instrSize_regLit;
    strcpy(meta->mnemonic, "xor");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "XOR_REG_REG");
    meta->opcode = XOR_REG_REG;
    meta->type = c16instrType_regReg;
    meta->size = c16instrSize_regReg;
    strcpy(meta->mnemonic, "xor");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "NOT");
    meta->opcode = NOT;
    meta->type = c16instrType_singleReg;
    meta->size = c16instrSize_singleReg;
    strcpy(meta->mnemonic, "not");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JMP_NOT_EQ");
    meta->opcode = JMP_NOT_EQ;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "jne");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JNE_REG");
    meta->opcode = JNE_REG;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "jne");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JEQ_REG");
    meta->opcode = JEQ_REG;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "jeq");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JEQ_LIT");
    meta->opcode = JEQ_LIT;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "jeq");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JLT_REG");
    meta->opcode = JLT_REG;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "jlt");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JLT_LIT");
    meta->opcode = JLT_LIT;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "jlt");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JGT_REG");
    meta->opcode = JGT_REG;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "jgt");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JGT_LIT");
    meta->opcode = JGT_LIT;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "jgt");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JLE_REG");
    meta->opcode = JLE_REG;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "jle");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JLE_LIT");
    meta->opcode = JLE_LIT;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "jle");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JGE_REG");
    meta->opcode = JGE_REG;
    meta->type = c16instrType_regMem;
    meta->size = c16instrSize_regMem;
    strcpy(meta->mnemonic, "jge");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "JGE_LIT");
    meta->opcode = JGE_LIT;
    meta->type = c16instrType_litMem;
    meta->size = c16instrSize_litMem;
    strcpy(meta->mnemonic, "jge");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "PSH_LIT");
    meta->opcode = PSH_LIT;
    meta->type = c16instrType_singleLit;
    meta->size = c16instrSize_singleLit;
    strcpy(meta->mnemonic, "psh");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "PSH_REG");
    meta->opcode = PSH_REG;
    meta->type = c16instrType_singleReg;
    meta->size = c16instrSize_singleReg;
    strcpy(meta->mnemonic, "psh");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "POP");
    meta->opcode = POP;
    meta->type = c16instrType_singleReg;
    meta->size = c16instrSize_singleReg;
    strcpy(meta->mnemonic, "pop");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "CAL_LIT");
    meta->opcode = CAL_LIT;
    meta->type = c16instrType_singleLit;
    meta->size = c16instrSize_singleLit;
    strcpy(meta->mnemonic, "cal");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "CAL_REG");
    meta->opcode = CAL_REG;
    meta->type = c16instrType_singleReg;
    meta->size = c16instrSize_singleReg;
    strcpy(meta->mnemonic, "cal");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "RET");
    meta->opcode = RET;
    meta->type = c16instrType_noArgs;
    meta->size = c16instrSize_noArgs;
    strcpy(meta->mnemonic, "ret");
    c16instrMetaTable[i++] = meta;

    meta = malloc(sizeof(c16instrMeta));
    strcpy(meta->instruction, "HLT");
    meta->opcode = HLT;
    meta->type = c16instrType_noArgs;
    meta->size = c16instrSize_noArgs;
    strcpy(meta->mnemonic, "hlt");
    c16instrMetaTable[i++] = meta;

    c16meta_sort(c16instrMetaTable);

    return c16instrMetaTable;
}

int c16meta_compare(const void *a, const void *b)
{
    return ((*(c16instrMeta **)a)->opcode) - ((*(c16instrMeta **)b)->opcode);
}


void c16meta_sort(c16instrMeta **c16instrMetaTable)
{
    qsort(c16instrMetaTable, c16meta_instrCount, sizeof(c16instrMeta *), c16meta_compare);
}

c16instrMeta *c16meta_get(c16instrMeta **c16instrMetaTable, uint8_t opcode)
{
    c16instrMeta *key = malloc(sizeof(c16instrMeta));
    key->opcode = opcode;
    c16instrMeta **result = bsearch(&key, c16instrMetaTable, c16meta_instrCount, sizeof(c16instrMeta *), c16meta_compare);
    free(key);
    if (result == NULL)
    {
        return NULL;
    }
    return *result;
}


void c16meta_free(c16instrMeta **c16instrMetaTable)
{
    for (int i = 0; i < c16meta_instrCount; i++)
    {
        free(c16instrMetaTable[i]);
    }
    free(c16instrMetaTable);
}