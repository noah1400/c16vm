import re
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


def indexBy(arr, prop):
    output = {}
    for item in arr:
        output[item[prop]] = item
    return output


instructions = indexBy(meta, 'instruction')

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

# whitespace or eol
optWhite = p.regex("\s*")


def upperOrLower(string):
    return p.string(string.upper()) | p.string(string.lower())


@p.generate
def registerParser():
    reg = yield (upperOrLower("IP") | upperOrLower("ACC") | upperOrLower("R1") | upperOrLower("R2") | upperOrLower("R3") | upperOrLower("R4") | upperOrLower("R5") | upperOrLower("R6") | upperOrLower("R7") | upperOrLower("R8") | upperOrLower("SP") | upperOrLower("FP") | upperOrLower("MB") | upperOrLower("IM")).map(lambda x: register(x.upper()))
    return reg


hexDigitParser = p.regex("[0-9a-fA-F]")
hexLiteralParser = (p.string("$") >> map_join(
    hexDigitParser.at_least(1))).map(hexLiteral)
addressParser = (p.string("&") >> map_join(
    hexDigitParser.at_least(1))).map(address)
validIdentifierParser = map_join(p.seq(p.regex(
    "[a-zA-Z_]"), p.regex("[a-zA-Z0-9_]+").optional().map(lambda x: '' if x is None else x)))
variableParser = p.string("!").then(validIdentifierParser).map(variable)
labelParser = p.seq(validIdentifierParser, p.string(
    ":"), optWhite).map(lambda x: x[0]).map(label)
operatorParser = (p.string("+").map(opPlus) |
                  p.string("-").map(opMinus) | p.string("*").map(opMultiply))

peekParser = p.peek(p.any_char)


def optionalWhitespaceSurrounded(parser):

    return optWhite.then(parser).skip(optWhite)


def commaSeparated(parser):

    return optionalWhitespaceSurrounded(parser).sep_by(p.string(","))


# constant parsers

@p.generate
def constantParser():
    isExport = yield p.string("+").optional().map(lambda x: x is not None)
    yield p.string("constant")
    yield p.whitespace
    name = yield validIdentifierParser
    yield p.whitespace
    yield p.string("=")
    yield p.whitespace
    value = yield hexLiteralParser
    yield optWhite
    return constant({'isExport': isExport, 'name': name, 'value': value})

# data parsers


@p.generate
def data8Parser():
    isExport = yield p.string("+").optional().map(lambda x: x is not None)
    yield p.string("data8")
    yield p.whitespace
    name = yield validIdentifierParser
    yield p.whitespace
    yield p.string("=")
    yield p.whitespace
    yield p.string("{")
    yield optWhite
    values = yield commaSeparated(hexLiteralParser)
    yield optWhite
    yield p.string("}")
    yield optWhite

    return data({'size': 8, 'isExport': isExport, 'name': name, 'values': values})


@p.generate
def data16Parser():
    isExport = yield p.string("+").optional().map(lambda x: x is not None)
    yield p.string("data16")
    yield p.whitespace
    name = yield validIdentifierParser
    yield p.whitespace
    yield p.string("=")
    yield p.whitespace
    yield p.string("{")
    yield optWhite
    values = yield commaSeparated(hexLiteralParser)
    yield optWhite
    yield p.string("}")
    yield optWhite

    return data({'size': 16, 'isExport': isExport, 'name': name, 'values': values})

# interpret as parser


@p.generate
def interpretAsParser():
    yield p.string("<")
    structure = yield validIdentifierParser
    yield p.string(">")
    yield optWhite
    symbol = yield validIdentifierParser
    yield p.string(".")
    prop = yield validIdentifierParser
    yield optWhite

    return interpretAs({'structure': structure, 'symbol': symbol, 'property': prop})

# expression parsers


expressionElementParser = (
    interpretAsParser | hexLiteralParser | variableParser)


def typifyBracketedExpr(expr):
    # return bracketedExpression(expr.map(lambda x: typifyBracketedExpr(x) if isinstance(x, list) else x))
    return bracketedExpression([(typifyBracketedExpr(x) if isinstance(x, list) else x) for x in expr])


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
        yield optWhite
        nextChar = yield peekParser

        if state == OPEN_BRACKET:
            yield p.string("(")
            expr.append([])
            stack.append(expr[-1])
            yield optWhite
            state = ELEMENT_OR_OPENING_BRACKET
        elif state == CLOSE_BRACKET:
            yield p.string(")")
            stack.pop()
            if len(stack) == 0:
                # we're done
                break

            yield optWhite
            state = OPERATOR_OR_CLOSING_BRACKET
        elif state == ELEMENT_OR_OPENING_BRACKET:
            if nextChar == ")":
                yield p.fail("Unexpected end of expression")

            if nextChar == "(":
                state = OPEN_BRACKET
            else:
                exprElement = yield expressionElementParser.optional()
                if exprElement is None:
                    yield p.fail("Unexpected end of expression element")
                stack[-1].append(exprElement)
                yield optWhite
                state = OPERATOR_OR_CLOSING_BRACKET
        elif state == OPERATOR_OR_CLOSING_BRACKET:
            if nextChar == ")":
                state = CLOSE_BRACKET
                continue

            o = yield operatorParser
            stack[-1].append(o)
            yield optWhite
            state = ELEMENT_OR_OPENING_BRACKET
        else:
            # should never happen
            raise Exception("Unknown state")

    return typifyBracketedExpr(expr)


@p.generate
def squareBracketExprParserWrapper():

    yield p.string("[")
    yield optWhite

    # states:
    EXPECT_ELEMENT = 0
    EXPECT_OPERATOR = 1
    expr = []
    state = EXPECT_ELEMENT

    while True:
        if state == EXPECT_ELEMENT:
            # result = yield expressionElementParser.optional() | bracketedExprParser.optional()
            result = yield p.alt(expressionElementParser, bracketedExprParser)

            if result is None:
                print("Unexpected end of expression result empty")
                break
            expr.append(result)
            state = EXPECT_OPERATOR
            yield optWhite
        elif state == EXPECT_OPERATOR:
            nextChar = yield peekParser
            if nextChar == "]":
                yield p.string("]")
                yield optWhite
                break

            result = yield operatorParser
            expr.append(result)
            state = EXPECT_ELEMENT
            yield optWhite
        else:
            # should never happen
            raise Exception("Unknown state")
    retexpr = squareBracketExpression(expr)
    return retexpr


@p.generate
def squareBracketExprParser():
    expr = yield squareBracketExprParserWrapper.map(lambda x: disambiguateOrderOfOperations(x))

    return expr

# structure parsers


@p.generate
def keyValuePairParser():

    yield optWhite
    key = yield validIdentifierParser
    yield optWhite
    yield p.string(":")
    yield optWhite
    value = yield hexLiteralParser
    yield optWhite

    return {'key': key, 'value': value}


@p.generate
def structureParser():

    isExport = yield p.string("+").optional().map(lambda x: x is not None)

    yield p.string("structure")

    yield p.whitespace
    name = yield validIdentifierParser
    yield optWhite
    yield p.string("{")
    yield optWhite
    members = yield commaSeparated(keyValuePairParser)
    yield optWhite
    yield p.string("}")
    yield optWhite

    s = structure({'isExport': isExport, 'name': name, 'members': members})

    return s

# instruction parsers


def litReg(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        arg1 = yield p.alt(hexLiteralParser, squareBracketExprParser)
        yield p.regex("\s*,\s*")
        arg2 = yield registerParser
        yield optWhite
        return instruction({'instruction': type, 'args': [arg1, arg2]})
    return parser


def regLit(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        r1 = yield registerParser
        yield p.regex("\s*,\s*")
        lit = yield p.alt(hexLiteralParser, squareBracketExprParser)
        yield optWhite
        return instruction({'instruction': type, 'args': [r1, lit]})
    return parser


def regLit8(mnemonic, type):
    regLit(mnemonic, type)


def regReg(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        r1 = yield registerParser
        yield p.regex("\s*,\s*")
        r2 = yield registerParser
        yield optWhite
        return instruction({'instruction': type, 'args': [r1, r2]})
    return parser


def regMem(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        r1 = yield registerParser
        yield p.regex("\s*,\s*")
        addr = yield p.alt(addressParser, p.string("&").then(squareBracketExprParser))
        yield optWhite
        return instruction({'instruction': type, 'args': [r1, addr]})
    return parser


def memReg(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        addr = yield p.alt(addressParser, p.string("&").then(squareBracketExprParser))
        yield p.regex("\s*,\s*")
        r1 = yield registerParser
        yield optWhite
        i = instruction({'instruction': type, 'args': [addr, r1]})

        return i
    return parser


def litMem(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        lit = yield p.alt(hexLiteralParser, squareBracketExprParser)
        yield p.regex("\s*,\s*")
        addr = yield p.alt(addressParser, p.string("&").then(squareBracketExprParser))
        yield optWhite
        return instruction({'instruction': type, 'args': [lit, addr]})
    return parser


def regPtrReg(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        r1 = yield p.string("&").then(registerParser)
        yield p.regex("\s*,\s*")
        r2 = yield registerParser
        yield optWhite
        return instruction({'instruction': type, 'args': [r1, r2]})
    return parser


def litOffReg(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        lit = yield p.alt(hexLiteralParser, squareBracketExprParser)
        yield p.regex("\s*,\s*")
        r1 = yield p.string("&").then(registerParser)
        yield p.regex("\s*,\s*")
        r2 = yield registerParser
        yield optWhite
        return instruction({'instruction': type, 'args': [lit, r1, r2]})
    return parser


def noArgs(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield optWhite
        return instruction({'instruction': type, 'args': []})
    return parser


def singleReg(mnemonic, type):
    @p.generate
    def parser():

        yield upperOrLower(mnemonic)

        yield p.whitespace
        r1 = yield registerParser
        yield optWhite
        return instruction({'instruction': type, 'args': [r1]})
    return parser


def singleLit(mnemonic, type):
    print(mnemonic, type)
    @p.generate
    def parser():
        yield optWhite
        yield upperOrLower(mnemonic)
        yield optWhite
        lit = yield p.alt(hexLiteralParser, squareBracketExprParser)
        if lit is None:
            p.fail("Expected literal")
        yield optWhite
        return instruction({'instruction': type, 'args': [lit]})
    return parser


type_formats = {value: globals()[type]
                for type, value in instructionTypes.items()}

all_instructions = []
for instr in meta:
    if instr['type'] not in type_formats:
        raise Exception("Unknown instruction type: " + instr['type'])
    # Call the parser factory to create a parser.
    parser = type_formats[instr['type']](
        instr['mnemonic'], instr['instruction'])
    all_instructions.append(optWhite >> parser << optWhite)


@p.generate
def instructionParser():

    instr = yield optWhite >> p.alt(*all_instructions) << optWhite
    print("instr", instr)

    return instr


@p.generate
def newlineParser():

    res = yield p.regex("[\r\n\t\f\v ]*")
    if not res:
        yield p.fail("")
    return res


@p.generate
def c16Parser():

    res = yield p.alt(newlineParser, data8Parser, data16Parser, constantParser, structureParser, labelParser, instructionParser)
    return res


def parse(code):
    result = []
    orgLen = len(code)
    parsed = 0
    while 1:
        if not code:
            break
        try:
            res, rem = c16Parser.parse_partial(code)
        except Exception as e:
            raise Exception("Error parsing at position " +
                            str(parsed) + " of " + str(orgLen) + ":\n" + str(e) + "\n" + code)
        parsed += len(code) - len(rem)
        if res is None:
            break
        if isinstance(res, dict):
            result.append(res)
        code = rem
    return result

import pprint


def asm(ast):

    def disasm(tree):
        disasm_out = []
        for node in tree:
            disasm_t = node['type']
            if disasm_t == 'INSTRUCTION':
                v = node['value']
                disasm_out.append([0, '\t' + v['instruction'] + ' ' + ', '.join([x['value'] for x in v['args']]), v])
            if disasm_t == 'LABEL':
                disasm_out.append([0, disasm_t + ' ' + node['value'], node])
            disasm_out.append(None)

    machineCode = []
    symbolicNames = {}
    structures = {}
    currentAddress = 0x0000

    for node in ast:
        t = node['type']
        if t not in ['LABEL', 'STRUCTURE', 'CONSTANT', 'DATA']:
            metadata = instructions[node['value']['instruction']]
            currentAddress += metadata['size']
        else:
            v = node['value']
            vn = v['name'] if 'name' in v else None
            if t == 'LABEL':
                if v in symbolicNames or vn in structures:
                    raise Exception(
                        "Can't create label " + node['value'] + " because a binding with this name already exists.")
                symbolicNames[node['value']] = currentAddress
            elif t == 'STRUCTURE':
                if node['value']['name'] in symbolicNames or node['value']['name'] in structures:
                    raise Exception(
                        "Can't create structure " + node['value'] + " because a binding with this name already exists.")
                structures[node['value']['name']] = {'members': {}}

                offset = 0
                for entry in node['value']['members']:
                    key = entry['key']
                    value = entry['value']
                    structures[node['value']['name']]['members'][key] = {
                        'offset': offset,
                        'size': int(value['value'], 16) & 0xffff
                    }
                    offset += structures[node['value']
                                         ['name']]['members'][key]['size']
            elif t == 'CONSTANT':
                if node['value']['name'] in symbolicNames or node['value']['name'] in structures:
                    raise Exception(
                        "Can't create constant " + node['value']['name'] + " because a binding with this name already exists.")
                symbolicNames[node['value']['name']] = int(
                    node['value']['value']['value'], 16) & 0xffff
            elif t == 'DATA':
                if node['value']['name'] in symbolicNames or node['value']['name'] in structures:
                    raise Exception(
                        "Can't create data " + node['value']['name'] + " because a binding with this name already exists.")
                symbolicNames[node['value']['name']] = currentAddress

                # calc the next offset based on the size of the data
                sizeOfEachValueInBytes = 2 if node['value']['size'] == 16 else 1
                totalSizeOfDataInBytes = len(
                    node['value']['values']) * sizeOfEachValueInBytes
                currentAddress += totalSizeOfDataInBytes

    def getNodeValue(node):
        t = node['type']
        if t == 'VARIABLE':
            if not node['value'] in symbolicNames:
                raise Exception("label " + node['value'] + " wasn't resolved")
            return symbolicNames[node['value']]
        elif t == 'INTERPRET_AS':
            struct = structures[node['value']['structure']]

            if not struct:
                raise Exception(
                    "structure " + node['value']['structure'] + " wasn't resolved")

            member = struct['members'][node['value']['property']]
            if not member:
                raise Exception("property " + node['value']['property'] +
                                " in structure " + node['value']['structure'] + " wasn't resolved")

            if node['value']['symbol'] not in symbolicNames:
                raise Exception(
                    "symbol " + node['value']['symbol'] + " wasn't resolved")
            symbol = symbolicNames[node['value']['symbol']]
            return symbol + member['offset']
        elif t == 'HEX_LITERAL':
            return int(node['value'], 16)
        else:
            raise Exception("Unsupported node type: " + node['type'])

    def encodeLitOrMem(node):
        hexVal = getNodeValue(node)
        highByte = (hexVal & 0xff00) >> 8
        lowByte = hexVal & 0x00ff
        # Big endian byte order
        machineCode.append(highByte)
        machineCode.append(lowByte)

    def encodeLit8(node):
        hexVal = getNodeValue(node)
        lowByte = hexVal & 0x00ff
        machineCode.append(lowByte)

    def encodeReg(reg):
        mappedReg = register_map[reg['value']]
        machineCode.append(mappedReg)

    def encodeData8(node):
        for byte in node['value']['values']:
            parsed = int(byte['value'], 16)
            machineCode.append(parsed & 0xff)

    def encodeData16(node):
        for byte in node['value']['values']:
            parsed = int(byte['value'], 16)
            highByte = (parsed & 0xff00) >> 8
            lowByte = parsed & 0x00ff
            # Big endian byte order
            machineCode.append(highByte)
            machineCode.append(lowByte)

    for node in ast:
        if node['type'] in ['LABEL', 'CONSTANT', 'STRUCTURE']:
            continue
        # encode data
        if node['type'] == 'DATA':
            if node['value']['size'] == 8:
                encodeData8(node)
            else:
                encodeData16(node)
            continue

        metadata = instructions[node['value']['instruction']]
        machineCode.append(metadata['opcode'])
        mt = metadata['type']

        if mt in [instructionTypes['litReg'], instructionTypes['memReg']]:
            encodeLitOrMem(node['value']['args'][0])
            encodeReg(node['value']['args'][1])

        if mt == instructionTypes['regLit8']:
            encodeReg(node['value']['args'][0])
            encodeLit8(node['value']['args'][1])

        if mt in [instructionTypes['regLit'], instructionTypes['regMem']]:
            encodeReg(node['value']['args'][0])
            encodeLitOrMem(node['value']['args'][1])

        if mt == instructionTypes['litMem']:
            encodeLitOrMem(node['value']['args'][0])
            encodeLitOrMem(node['value']['args'][1])

        if mt in [instructionTypes['regReg'], instructionTypes['regPtrReg']]:
            encodeReg(node['value']['args'][0])
            encodeReg(node['value']['args'][1])

        if mt == instructionTypes['litOffReg']:
            encodeLitOrMem(node['value']['args'][0])
            encodeReg(node['value']['args'][1])
            encodeLit8(node['value']['args'][2])

        if mt == instructionTypes['singleReg']:
            encodeReg(node['value']['args'][0])

        if mt == instructionTypes['singleLit']:
            encodeLitOrMem(node['value']['args'][0])

        if mt == instructionTypes['noArgs']:
            continue

    pprint.pprint(structures)
    pprint.pprint(symbolicNames)
    pprint.pprint(machineCode)

    # print("disassembling...")
    # disassembled = disasm(ast)
    # pprint.pprint(disassembled)

    return machineCode


def main():
    code = """       
structure Rectangle {
  x: $4,
  y: $2,
  w: $2,
  h: $2
}

constant a = $5

start_of_code:

  
  
  add $1, r1
  mov &[ <Rectangle> myRectangle.x * ( $1 * !a ) ], r1

  data16 myRectangle = { $A3, $1B, $FF, $FF10 }
    
    """

    parsedOutput = parse(code)
    pprint.pprint(parsedOutput)
    print("\n")
    asmOutput = asm(parsedOutput)
    print(" ".join(["%02x" % x for x in asmOutput]))

if __name__ == '__main__':
    main()
