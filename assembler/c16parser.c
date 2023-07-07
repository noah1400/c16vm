#include <c16parser.h>



void _c16parser_addTempParser(c16parser_node **anchor, mpc_parser_t *parser)
{
    c16parser_node *node = malloc(sizeof(c16parser_node));
    node->parser = parser;
    node->next = NULL;

    if (*anchor == NULL)
    {
        *anchor = node;
    } else {
        c16parser_node *current = *anchor;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = node;
    }
}

void _c16parser_freeTempParsers(c16parser_node **anchor)
{
    c16parser_node *current = *anchor;
    while (current != NULL)
    {
        c16parser_node *next = current->next;
        mpc_cleanup(1, current->parser);
        free(current);
        current = next;
    }
    *anchor = NULL;
}

mpc_parser_t *_c16parser_build(c16parser_node **anchor)
{
    mpc_parser_t *c16parser = mpc_new("c16parser");
    mpc_parser_t *data8 = _c16parser_data8(anchor);
    mpc_parser_t *data16 = _c16parser_data16(anchor);
    mpc_parser_t *constant = _c16parser_constant(anchor);
    mpc_parser_t *structure = _c16parser_structure(anchor);
    mpc_parser_t *instruction = _c16parser_instruction(anchor);
    mpc_parser_t *label = _c16parser_label(anchor);

    mpc_parser_t *choice = mpc_or(6, data8, data16, constant, structure, instruction, label);
    mpc_parser_t *many = mpc_many(mpcf_strfold, choice);
    mpc_define(c16parser, many);

    _c16parser_addTempParser(anchor, data8);
    _c16parser_addTempParser(anchor, data16);
    _c16parser_addTempParser(anchor, constant);
    _c16parser_addTempParser(anchor, structure);
    _c16parser_addTempParser(anchor, instruction);
    _c16parser_addTempParser(anchor, label);
    _c16parser_addTempParser(anchor, choice);
    _c16parser_addTempParser(anchor, many);

    return c16parser;
    
}

mpc_result_t *c16parser_run(char *input)
{
    c16parser_node *anchor = NULL;

    mpc_result_t *r = malloc(sizeof(mpc_result_t));
    mpc_parser_t *c16parser = _c16parser_build(&anchor);
    
    if(mpc_parse("input", input, c16parser, r))
    {
        mpc_ast_print(r->output);
        mpc_ast_delete(r->output);
    } else {
        mpc_err_print(r->error);
        mpc_err_delete(r->error);
    }
    
    
    mpc_delete(c16parser);
    _c16parser_freeTempParsers(&anchor);
}


// Parsers

mpc_parser_t *_c16parser_data16(c16parser_node **anchor)
{
    return _c16parser_dataParser("16", anchor);
}

mpc_parser_t *_c16parser_data8(c16parser_node **anchor)
{
    return _c16parser_dataParser("8", anchor);
}

mpc_parser_t *_c16parser_dataParser(char *size, c16parser_node **anchor)
{
    char *namePre = "data";
    char *name = malloc(strlen(namePre) + strlen(size) + 1);
    strcpy(name, namePre);
    strcat(name, size);
    mpc_parser_t *data = mpc_new(name);
    

    mpc_parser_t *isExport = mpc_new("isExport"); // zero or one time '+'
    mpc_define(isExport, mpc_char('+'));
    mpc_parser_t *dataSize = mpc_new("dataSize"); // string name
    mpc_define(dataSize, mpc_string(name));
    mpc_parser_t *ident = _c16parser_validIdentifier();
    mpc_parser_t *whiteSpace = mpc_new("whiteSpace"); // one or more times ' '
    mpc_define(whiteSpace, mpc_whitespaces());
    mpc_parser_t *hexLiteral = _c16parser_hexLiteral(anchor);
    mpc_parser_t *values = mpc_new("values"); // comma separated hex values
    mpca_lang(MPCA_LANG_DEFAULT,
        "values : <hexLiteral> (',' <hexLiteral>)* ;",
        hexLiteral, values);

    char *rule = " : <isExport>? <dataSize> <ident> <whiteSpace> '=' <whiteSpace> '{' <whiteSpace> <values> <whiteSpace> '}' /\\s*/ ;";
    char *rule2 = malloc(strlen(name) + strlen(rule) + 1);
    strcpy(rule2, name);
    strcat(rule2, rule);
    
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        rule2,
        isExport, dataSize, ident, whiteSpace, values, data, NULL);
    free(rule2);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, isExport);
    _c16parser_addTempParser(anchor, dataSize);
    _c16parser_addTempParser(anchor, ident);
    _c16parser_addTempParser(anchor, whiteSpace);
    _c16parser_addTempParser(anchor, hexLiteral);
    _c16parser_addTempParser(anchor, values);
    free(name);

    return data;
}

mpc_parser_t *_c16parser_constant(c16parser_node **anchor)
{
    mpc_parser_t *isExport = mpc_new("isExport"); // zero or one time '+'
    mpc_define(isExport, mpc_char('+'));
    mpc_parser_t *constantStr = mpc_new("constant"); // string name
    mpc_define(constantStr, mpc_string("constant"));
    mpc_parser_t *ident = _c16parser_validIdentifier();
    mpc_parser_t *hexLiteral = _c16parser_hexLiteral(anchor);

    mpc_parser_t *constant = mpc_new("constant");
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "constant : <isExport>?<constantStr>/\\s+/<ident>/\\s+/'='/\\s+/<hexLiteral>/\\s*/",
        isExport, constantStr, ident, hexLiteral, constant, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, isExport);
    _c16parser_addTempParser(anchor, constantStr);
    _c16parser_addTempParser(anchor, ident);
    _c16parser_addTempParser(anchor, hexLiteral);

    return constant;
}

mpc_parser_t *_c16parser_structure(c16parser_node **anchor)
{
    mpc_parser_t *isExport = mpc_new("isExport"); // zero or one time '+'
    mpc_define(isExport, mpc_char('+'));
    mpc_parser_t *structureStr = mpc_new("structure"); // string name
    mpc_define(structureStr, mpc_string("structure"));
    mpc_parser_t *ident = _c16parser_validIdentifier();
    mpc_parser_t *commaSeperatedKeyVal = mpc_new("comSepKeyVal"); // comma separated key value pairs
    mpc_parser_t *keyValuePair = _c16parser_keyValuePair(anchor);
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "comSepKeyVal : <keyValuePair> (',' <keyValuePair>)* ;",
        keyValuePair, commaSeperatedKeyVal);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    mpc_parser_t *structure = mpc_new("structure");
    err = mpca_lang(MPCA_LANG_DEFAULT,
    "structure : <isExport>?<structureStr>/\\s+/<ident>/\\s+/'{'/\\s+/<comSepKeyVal>/\\s+/'}'/\\s*/",
    isExport, structureStr, ident, commaSeperatedKeyVal, structure, NULL);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, isExport);
    _c16parser_addTempParser(anchor, structureStr);
    _c16parser_addTempParser(anchor, ident);
    _c16parser_addTempParser(anchor, commaSeperatedKeyVal);
    _c16parser_addTempParser(anchor, keyValuePair);

    return structure;
}

mpc_parser_t *_c16parser_instruction(c16parser_node **anchor)
{

    // TODO: access function metadata ('c16meta.h')

    mpc_parser_t *instr = mpc_new("instruction");
    mpc_define(instr, mpc_string("instr"));

    return instr;
}

mpc_parser_t *_c16parser_label(c16parser_node **anchor)
{
    mpc_parser_t *ident = _c16parser_validIdentifier();
    mpc_parser_t *label = mpc_new("label");
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "label : <ident> ':' /\\s*/ ;",
        ident, label, NULL);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, ident);


    return label;
}

mpc_parser_t *_c16parser_keyValuePair(c16parser_node **anchor)
{
    mpc_parser_t *ident = _c16parser_validIdentifier();
    mpc_parser_t *hexLiteral = _c16parser_hexLiteral(anchor);
    mpc_parser_t *keyValuePair = mpc_new("keyValuePair");
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "keyValuePair : /\\s*/<ident>/\\s+/':'/\\s+/<hexLiteral>/\\s*/",
        ident, hexLiteral, keyValuePair, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, ident);
    _c16parser_addTempParser(anchor, hexLiteral);

    return keyValuePair;
}

mpc_parser_t *_c16parser_register()
{
    mpc_parser_t *reg = mpc_new("register");
    mpc_define(reg, mpc_re("(?:r[0-9])|sp|fp|ip|acc|mb"));

    return reg;
}

mpc_parser_t *_c16parser_hex()
{
    mpc_parser_t *hex = mpc_new("hex");
    mpc_define(hex, mpc_re("[0-9a-fA-F]"));

    return hex;
}

mpc_parser_t *_c16parser_hexLiteral(c16parser_node **anchor)
{
    mpc_parser_t *hexLiteral = mpc_new("hexLiteral");
    mpc_parser_t *hex = _c16parser_hex();
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "hexLiteral : '$'<hex>+ ;",
        hex, hexLiteral, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    //_c16parser_addTempParser(anchor, hex);

    return hexLiteral;
}

mpc_parser_t *_c16parser_address(c16parser_node **anchor)
{
    mpc_parser_t *address = mpc_new("address");
    mpc_parser_t *hex = _c16parser_hex();
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "address : '&'<hex>+ ;",
        hex, address, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, hex);

    return address;
}

mpc_parser_t *_c16parser_validIdentifier()
{
    mpc_parser_t *ident = mpc_new("ident");
    mpc_define(ident, mpc_re("[a-zA-Z_][a-zA-Z0-9_]*"));

    return ident;
}

mpc_parser_t *_c16parser_variable(c16parser_node **anchor)
{
    mpc_parser_t *ident = _c16parser_validIdentifier();
    mpc_parser_t *variable = mpc_new("variable");
    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
        "variable : '!'<ident>",
        ident, variable, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    }

    _c16parser_addTempParser(anchor, ident);

    return variable;
}

mpc_parser_t *_c16parser_operator()
{
    mpc_parser_t *operator = mpc_new("operator");
    mpc_define(operator, mpc_re("[+\\-*]"));

    return operator;
}