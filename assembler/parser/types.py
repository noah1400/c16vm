from .util import asType

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