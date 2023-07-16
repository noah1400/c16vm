import parsy as p

# Instructions

instructionTypes = {
    'litReg': 0,
    'regLit': 1,
    'regLit8': 2,
    'regReg': 3,
    'regMem': 4,
    'memReg': 5,
    'litMem': 6,
    'regPtrReg': 7,
    'litOffReg': 8,
    'noArgs': 9,
    'singleReg': 10,
    'singleLit': 11
}

instructionTypeSizes = {
    'litReg': 4,
    'regLit': 4,
    'regLit8': 3,
    'regReg': 3,
    'regMem': 4,
    'memReg': 4,
    'litMem': 5,
    'regPtrReg': 3,
    'litOffReg': 5,
    'noArgs': 1,
    'singleReg': 2,
    'singleLit': 3
}

meta = [
    {
        'instruction': 'RET_INT',
        'opcode': 0xFC,
        'type': instructionTypes['noArgs'],
        'size': instructionTypeSizes['noArgs'],
        'mnemonic': 'rti',
    },
    {
        'instruction': 'INT',
        'opcode': 0xFD,
        'type': instructionTypes['singleLit'],
        'size': instructionTypeSizes['singleLit'],
        'mnemonic': 'int',
    },
    {
        'instruction': 'MOV_LIT_REG',
        'opcode': 0x10,
        'type': instructionTypes['litReg'],
        'size': instructionTypeSizes['litReg'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'MOV_REG_REG',
        'opcode': 0x11,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'MOV_REG_MEM',
        'opcode': 0x12,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'MOV_MEM_REG',
        'opcode': 0x13,
        'type': instructionTypes['memReg'],
        'size': instructionTypeSizes['memReg'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'MOV_LIT_MEM',
        'opcode': 0x1B,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'MOV_REG_PTR_REG',
        'opcode': 0x1C,
        'type': instructionTypes['regPtrReg'],
        'size': instructionTypeSizes['regPtrReg'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'MOV_LIT_OFF_REG',
        'opcode': 0x1D,
        'type': instructionTypes['litOffReg'],
        'size': instructionTypeSizes['litOffReg'],
        'mnemonic': 'mov',
    },
    {
        'instruction': 'ADD_REG_REG',
        'opcode': 0x14,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'add',
    },
    {
        'instruction': 'ADD_LIT_REG',
        'opcode': 0x3F,
        'type': instructionTypes['litReg'],
        'size': instructionTypeSizes['litReg'],
        'mnemonic': 'add',
    },
    {
        'instruction': 'SUB_LIT_REG',
        'opcode': 0x16,
        'type': instructionTypes['litReg'],
        'size': instructionTypeSizes['litReg'],
        'mnemonic': 'sub',
    },
    {
        'instruction': 'SUB_REG_LIT',
        'opcode': 0x1E,
        'type': instructionTypes['regLit'],
        'size': instructionTypeSizes['regLit'],
        'mnemonic': 'sub',
    },
    {
        'instruction': 'SUB_REG_REG',
        'opcode': 0x1F,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'sub',
    },
    {
        'instruction': 'INC_REG',
        'opcode': 0x35,
        'type': instructionTypes['singleReg'],
        'size': instructionTypeSizes['singleReg'],
        'mnemonic': 'inc',
    },
    {
        'instruction': 'DEC_REG',
        'opcode': 0x36,
        'type': instructionTypes['singleReg'],
        'size': instructionTypeSizes['singleReg'],
        'mnemonic': 'dec',
    },
    {
        'instruction': 'MUL_LIT_REG',
        'opcode': 0x20,
        'type': instructionTypes['litReg'],
        'size': instructionTypeSizes['litReg'],
        'mnemonic': 'mul',
    },
    {
        'instruction': 'MUL_REG_REG',
        'opcode': 0x21,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'mul',
    },
    {
        'instruction': 'LSF_REG_LIT',
        'opcode': 0x26,
        'type': instructionTypes['regLit8'],
        'size': instructionTypeSizes['regLit8'],
        'mnemonic': 'lsf',
    },
    {
        'instruction': 'LSF_REG_REG',
        'opcode': 0x27,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'lsf',
    },
    {
        'instruction': 'RSF_REG_LIT',
        'opcode': 0x2A,
        'type': instructionTypes['regLit8'],
        'size': instructionTypeSizes['regLit8'],
        'mnemonic': 'rsf',
    },
    {
        'instruction': 'RSF_REG_REG',
        'opcode': 0x2B,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'rsf',
    },
    {
        'instruction': 'AND_REG_LIT',
        'opcode': 0x2E,
        'type': instructionTypes['regLit'],
        'size': instructionTypeSizes['regLit'],
        'mnemonic': 'and',
    },
    {
        'instruction': 'AND_REG_REG',
        'opcode': 0x2F,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'and',
    },
    {
        'instruction': 'OR_REG_LIT',
        'opcode': 0x30,
        'type': instructionTypes['regLit'],
        'size': instructionTypeSizes['regLit'],
        'mnemonic': 'or',
    },
    {
        'instruction': 'OR_REG_REG',
        'opcode': 0x31,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'or',
    },
    {
        'instruction': 'XOR_REG_LIT',
        'opcode': 0x32,
        'type': instructionTypes['regLit'],
        'size': instructionTypeSizes['regLit'],
        'mnemonic': 'xor',
    },
    {
        'instruction': 'XOR_REG_REG',
        'opcode': 0x33,
        'type': instructionTypes['regReg'],
        'size': instructionTypeSizes['regReg'],
        'mnemonic': 'xor',
    },
    {
        'instruction': 'NOT',
        'opcode': 0x34,
        'type': instructionTypes['singleReg'],
        'size': instructionTypeSizes['singleReg'],
        'mnemonic': 'not',
    },
    {
        'instruction': 'JMP_NOT_EQ',
        'opcode': 0x15,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'jne',
    },
    {
        'instruction': 'JNE_REG',
        'opcode': 0x40,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'jne',
    },
    {
        'instruction': 'JEQ_REG',
        'opcode': 0x3E,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'jeq',
    },
    {
        'instruction': 'JEQ_LIT',
        'opcode': 0x41,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'jeq',
    },
    {
        'instruction': 'JLT_REG',
        'opcode': 0x42,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'jlt',
    },
    {
        'instruction': 'JLT_LIT',
        'opcode': 0x43,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'jlt',
    },
    {
        'instruction': 'JGT_REG',
        'opcode': 0x44,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'jgt',
    },
    {
        'instruction': 'JGT_LIT',
        'opcode': 0x45,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'jgt',
    },
    {
        'instruction': 'JLE_REG',
        'opcode': 0x46,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'jle',
    },
    {
        'instruction': 'JLE_LIT',
        'opcode': 0x47,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'jle',
    },
    {
        'instruction': 'JGE_REG',
        'opcode': 0x48,
        'type': instructionTypes['regMem'],
        'size': instructionTypeSizes['regMem'],
        'mnemonic': 'jge',
    },
    {
        'instruction': 'JGE_LIT',
        'opcode': 0x49,
        'type': instructionTypes['litMem'],
        'size': instructionTypeSizes['litMem'],
        'mnemonic': 'jge',
    },
    {
        'instruction': 'PSH_LIT',
        'opcode': 0x17,
        'type': instructionTypes['singleLit'],
        'size': instructionTypeSizes['singleLit'],
        'mnemonic': 'psh',
    },
    {
        'instruction': 'PSH_REG',
        'opcode': 0x18,
        'type': instructionTypes['singleReg'],
        'size': instructionTypeSizes['singleReg'],
        'mnemonic': 'psh',
    },
    {
        'instruction': 'POP',
        'opcode': 0x1A,
        'type': instructionTypes['singleReg'],
        'size': instructionTypeSizes['singleReg'],
        'mnemonic': 'pop',
    },
    {
        'instruction': 'CAL_LIT',
        'opcode': 0x5E,
        'type': instructionTypes['singleLit'],
        'size': instructionTypeSizes['singleLit'],
        'mnemonic': 'cal',
    },
    {
        'instruction': 'CAL_REG',
        'opcode': 0x5F,
        'type': instructionTypes['singleReg'],
        'size': instructionTypeSizes['singleReg'],
        'mnemonic': 'cal',
    },
    {
        'instruction': 'RET',
        'opcode': 0x60,
        'type': instructionTypes['noArgs'],
        'size': instructionTypeSizes['noArgs'],
        'mnemonic': 'ret',
    },
    {
        'instruction': 'HLT',
        'opcode': 0xFF,
        'type': instructionTypes['noArgs'],
        'size': instructionTypeSizes['noArgs'],
        'mnemonic': 'hlt',
    }
]

# utils


def asType(type):
    def inner(value):
        return {'type': type, 'value': value}
    return inner

def map_join(parser: p.Parser):
    return parser.map(lambda x: ''.join(x))

# types


register = asType('REGISTER')
hexLiteral = asType('HEX_LITERAL')
address = asType('ADDRESS')
variable = asType('VARIABLE')

opPlus = asType('OP_PLUS')
opMinus = asType('OP_MINUS')
opMultiply = asType('OP_MULTIPLY')

binaryOperation = asType('BINARY_OPERATION')
bracketedExpression = asType('BRACKETED_EXPRESSION')
squareBracketExpression = asType('SQUARE_BRACKET_EXPRESSION')

instruction = asType('INSTRUCTION')
label = asType('LABEL')
data = asType('DATA')
structure = asType('STRUCTURE')
constant = asType('CONSTANT')
interpretAs = asType('INTERPRET_AS')

# registers

registers = [
    "IP", "ACC",
    "R1", "R2", "R3", "R4", 
    "R5", "R6", "R7", "R8",
    "SP", "FP", "MB", "IM"
]

register_map = {reg_name: index for index, reg_name in enumerate(registers)}

# common parsers

def upperOrLower(string):
    return p.string(string.upper()) | p.string(string.lower())

def registerParser():
    reg = upperOrLower("IP")
    for reg_name in registers[1:]:
        reg = reg | upperOrLower(reg_name)
    return reg.map(register)

hexDigitParser = p.regex("[0-9a-fA-F]")
hexLiteralParser = (p.string("$") >> map_join(hexDigitParser.at_least(1))).map(hexLiteral)
addressParser = (p.string("&") >> map_join(hexDigitParser.at_least(1))).map(address)
validIdentifierParser = map_join(p.seq(p.regex("[a-zA-Z_]"), p.regex("[a-zA-Z0-9_]+").optional().map(lambda x: '' if x is None else x)))
variableParser = p.string("!").then(validIdentifierParser).map(variable)
labelParser = p.seq(validIdentifierParser, p.string(":"), p.regex("\s*")).map(lambda x: x[0]).map(label)
operatorParser = (p.string("+").map(opPlus) | p.string("-").map(opMinus) | p.string("*").map(opMultiply))

peekParser = p.peek(p.any_char)


def optionalWhitespaceSurrounded(parser):
    return p.regex("\s*").then(parser).skip(p.regex("\s*"))

def commaSeparated(parser):
    return optionalWhitespaceSurrounded(parser).sep_by(p.string(","))


# constant parsers

@p.generate
def constantParser():
    isExport = yield p.string("+").optional().map(lambda x: x is not None)
    yield p.string("constant")
    yield p.regex("\s+")
    name = yield validIdentifierParser
    yield p.regex("\s+")
    yield p.string("=")
    yield p.regex("\s+")
    value = yield hexLiteralParser
    yield p.regex("\s*")
    return constant({'isExport': isExport, 'name': name, 'value': value})

# data parsers

@p.generate
def data8Parser():
    isExport = yield p.string("+").optional().map(lambda x: x is not None)
    yield p.string("data8")
    yield p.regex("\s+")
    name = yield validIdentifierParser
    yield p.regex("\s+")
    yield p.string("=")
    yield p.regex("\s+")
    yield p.string("{")
    yield p.regex("\s*")
    values = yield commaSeparated(hexLiteralParser)
    yield p.regex("\s*")
    yield p.string("}")
    yield p.regex("\s*")

    return data({'size': 8, 'isExport': isExport, 'name': name, 'values': values})

@p.generate
def data16Parser():
    isExport = yield p.string("+").optional().map(lambda x: x is not None)
    yield p.string("data16")
    yield p.regex("\s+")
    name = yield validIdentifierParser
    yield p.regex("\s+")
    yield p.string("=")
    yield p.regex("\s+")
    yield p.string("{")
    yield p.regex("\s*")
    values = yield commaSeparated(hexLiteralParser)
    yield p.regex("\s*")
    yield p.string("}")
    yield p.regex("\s*")

    return data({'size': 16, 'isExport': isExport, 'name': name, 'values': values})

# interpret as parser

@p.generate
def interpretAsParser():
    yield p.string("<")
    structure = yield validIdentifierParser
    yield p.string(">")
    yield p.regex("\s*")
    symbol = yield validIdentifierParser
    yield p.string(".")
    prop = yield validIdentifierParser
    yield p.regex("\s*")

    return interpretAs({'structure': structure, 'symbol': symbol, 'property': prop})

# expression parsers

expressionElementParser = (interpretAsParser() | hexLiteralParser | variableParser )

def typifyBracketedExpr(expr):
    return bracketedExpression(expr.map(lambda x: typifyBracketedExpr(x) if isinstance(x, list) else x))

@p.generate
def disambiguateOrderOfOperations(expr):
    if expr['type'] != 'SQUARE_BRACKET_EXPRESSION' and expr['type'] != 'BRACKETED_EXPRESSION':
        return expr
    
    if len(expr['value']) == 1:
        return expr['value'][0]
    
    priorities = {
        'OP_MULTIPLY': 2,
        'OP_PLUS': 1,
        'OP_MINUS': 0
    }
    candidateExpression = {
        'priority': -1,
    }
    for i in range(1, len(expr['value']), 2):
        level = priorities[expr['value'][i]['type']]
        if level > candidateExpression['priority']:
            candidateExpression = {
                'priority': level,
                'a': i-1,
                'b': i+1,
                'op': expr['value'][i]
            }

    li = candidateExpression['a']
    ri = candidateExpression['b']
    newExpression = bracketedExpression([
        *expr['value'][:li],
        binaryOperation({
            'a': disambiguateOrderOfOperations(expr['value'][li]),
            'b': disambiguateOrderOfOperations(expr['value'][ri]),
            'op': candidateExpression['op']
        }),
        *expr['value'][ri+1:]
    ])

    return disambiguateOrderOfOperations(newExpression)

@p.generate
def bracketedExprParser():
    # states:
    OPEN_BRACKET = 0
    OPERATOR_OR_CLOSING_BRACKET = 1
    ELEMENT_OR_OPENING_BRACKET = 2
    CLOSE_BRACKET = 3

    state = ELEMENT_OR_OPENING_BRACKET

    expr = []
    stack = [expr]
    yield p.string("(")

    while True:
        nextChar = yield peekParser
        
        if state == OPEN_BRACKET:
            yield p.string("(")
            expr.append([])
            stack.append(expr[-1])
            yield p.regex("\s*")
            state = ELEMENT_OR_OPENING_BRACKET
        elif state == CLOSE_BRACKET:
            yield p.string(")")
            stack.pop()
            if len(stack) == 0:
                # we're done
                break

            yield p.regex("\s*")
            state = OPERATOR_OR_CLOSING_BRACKET
        elif state == ELEMENT_OR_OPENING_BRACKET:
            if nextChar == ")":
                yield p.fail("Unexpected end of expression")

            if nextChar == "(":
                state = OPEN_BRACKET
            else:
                exprElement = yield expressionElementParser
                stack[-1].append(exprElement)
                yield p.regex("\s*")
                state = OPERATOR_OR_CLOSING_BRACKET   
        elif state == OPERATOR_OR_CLOSING_BRACKET:
            if nextChar == ")":
                state = CLOSE_BRACKET
                continue
            
            o = yield operatorParser
            stack[-1].append(o)
            yield p.regex("\s*")
            state = ELEMENT_OR_OPENING_BRACKET
        else:
            # should never happen
            raise Exception("Unknown state")
    
    return typifyBracketedExpr(expr)

@p.generate
def squareBracketExprParserWrapper():

    yield p.string("[")
    yield p.regex("\s*")

    # states:
    EXPECT_ELEMENT = 0
    EXPECT_OPERATOR = 1
    expr = []
    state = EXPECT_ELEMENT

    while True:
        if state == EXPECT_ELEMENT:
            result = yield (bracketedExprParser() | expressionElementParser)
            expr.append(result)
            state = EXPECT_OPERATOR
            yield p.regex("\s*")
        elif state == EXPECT_OPERATOR:
            nextChar = yield peekParser
            if nextChar == "]":
                yield p.string("]")
                yield p.regex("\s*")
                break

            result = yield operatorParser
            expr.append(result)
            state = EXPECT_ELEMENT
            yield p.regex("\s*")
        else:
            # should never happen
            raise Exception("Unknown state")
    return squareBracketExpression(expr)

@p.generate
def squareBracketExprParser():
    return squareBracketExprParserWrapper().map(disambiguateOrderOfOperations)

def main():
    pass


if __name__ == '__main__':
    main()
