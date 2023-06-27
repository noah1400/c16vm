#include <c16meta.h>

#include <stdio.h>
#include <assert.h>

void testMeta()
{

    uint8_t opcodes[] = {
        NOP,
        MOV_LIT_REG,
        MOV_REG_REG,
        MOV_REG_MEM,
        MOV_MEM_REG,
        MOV_LIT_MEM,
        MOV_REG_PTR_REG,
        MOV_LIT_OFF_REG,
        ADD_REG_REG,
        ADD_LIT_REG,
        SUB_LIT_REG,
        SUB_REG_LIT,
        SUB_REG_REG,
        INC_REG,
        DEC_REG,
        MUL_LIT_REG,
        MUL_REG_REG,
        LSF_REG_LIT,
        LSF_REG_REG,
        RSF_REG_LIT,
        RSF_REG_REG,
        AND_REG_LIT,
        AND_REG_REG,
        OR_REG_LIT,
        OR_REG_REG,
        XOR_REG_LIT,
        XOR_REG_REG,
        NOT,
        JMP_NOT_EQ,
        JNE_REG,
        JEQ_REG,
        JEQ_LIT,
        JLT_REG,
        JLT_LIT,
        JGT_REG,
        JGT_LIT,
        JLE_REG,
        JLE_LIT,
        JGE_REG,
        JGE_LIT,
        PSH_LIT,
        PSH_REG,
        POP,
        CAL_LIT,
        CAL_REG,
        RET,
        HLT};

    
        c16instrMeta **c16instrMetaTable = c16meta_init();

    for (int i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++)
    {
        uint8_t searchFor = opcodes[i];
        c16instrMeta *instrMeta = c16meta_get(c16instrMetaTable, searchFor);
        uint8_t opcode = instrMeta->opcode;
        assert(opcode == searchFor);
    }
    c16meta_free(c16instrMetaTable);

    printf("c16meta passed\n");
}

int main()
{
    testMeta();
    printf("All tests passed\n");
    return 0;
}