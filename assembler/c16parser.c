#include <c16parser.h>

mpc_parser_t *_c16parser_build(c16parser_node *anchor)
{
    mpc_parser_t *c16parser = mpc_new("c16parser");
    mpc_parser_t *data8 = _c16parser_data8(anchor);
    mpc_parser_t *data16 = _c16parser_data16(anchor);
    mpc_parser_t *constant = _c16parser_constant();
    mpc_parser_t *structure = _c16parser_structure();
    mpc_parser_t *instruction = _c16parser_instruction();
    mpc_parser_t *label = _c16parser_label();

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
    mpc_parser_t *c16parser = _c16parser_build(anchor);
    mpc_err_t *err = mpc_parse("input", input, c16parser, r);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
    } else {
        mpc_ast_print(r->output);
    }
    mpc_delete(c16parser);
    _c16parser_freeTempParsers(anchor);
}


// Parsers

mpc_parser_t *_c16parser_data16(c16parser_node *anchor)
{
    return _c16parser_dataParser("16", anchor);
}

mpc_parser_t *_c16parser_data8(c16parser_node *anchor)
{
    return _c16parser_dataParser("8", anchor);
}

mpc_parser_t *_c16parser_dataParser(char *size, c16parser_node *anchor)
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
    mpc_parser_t *hex = _c16parser_hex();
    mpc_parser_t *values = mpc_new("values"); // comma separated hex values
    mpca_lang(MPCA_LANG_DEFAULT,
        "values : <hex> (',' <hex>)* ;",
        hex, values);

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
    _c16parser_addTempParser(anchor, hex);
    _c16parser_addTempParser(anchor, values);
    free(name);

    return data;
}