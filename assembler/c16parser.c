#include <c16parser.h>

mpc_result_t *c16parser_parse(const char *input)
{
    // Main Parsers
    mpc_parser_t *c16parser = mpc_new("c16parser");
    mpc_parser_t *data = mpc_new("data");
    mpc_parser_t *data8 = mpc_new("data8");
    mpc_parser_t *data16 = mpc_new("data16");
    mpc_parser_t *constant = mpc_new("constant");
    mpc_parser_t *structure = mpc_new("structure");
    mpc_parser_t *instruction = mpc_new("instruction");
    mpc_parser_t *label = mpc_new("label");

    // helpers
    mpc_parser_t *isExport = mpc_new("isExport");
    mpc_parser_t *ident = mpc_new("ident");
    mpc_parser_t *hex = mpc_new("hex");
    mpc_parser_t *hexLiteral = mpc_new("hexLiteral");
    mpc_parser_t *commaSepHexLits = mpc_new("commaSepHexLits");
    mpc_parser_t *keyValuePair = mpc_new("keyValuePair");
    mpc_parser_t *commaSepKeyValuePairs = mpc_new("commaSepKeyValuePairs");

    mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
                               "isExport : '+' ;"
                               "ident : /[a-zA-Z_][a-zA-Z0-9_]*/ ;"
                               "hex : /[0-9a-fA-F]/ ;"
                               "hexLiteral : '$'<hex>+ ;"
                               "commaSepHexLits : <hexLiteral>/\\s*/(','<hexLiteral>)* ;"
                               "keyValuePair : <ident>/\\s*/':'/\\s*/<hexLiteral> ;"
                               "commaSepKeyValuePairs : <keyValuePair>/\\s*/(','/\\s*/<keyValuePair>)*/\\s*/ ;"
                               "data : <ident>/\\s*/'='/\\s*/'{'/\\s*/<commaSepHexLits>/\\s*/'}'/\\s*/ ;"
                               "data8 : <isExport>?\"data8\"/\\s+/<data> ;"
                               "data16 : <isExport>?\"data16\"/\\s+/<data> ;"
                               "constant : <isExport>?\"constant\" <ident>/\\s*/'='/\\s*/<hexLiteral> ;"
                               "structure : <isExport>?\"structure\" <ident>/\\s*/'{'/\\s*/<commaSepKeyValuePairs>/\\s*/'}'/\\s*/ ;"
                               "instruction : /\\s*/\"instr\"/\\s*/ ;"
                               "label : <ident>':'/\\s*/ ;"
                               "c16parser : /^/(<data8>|<data16>|<constant>|<structure>|<instruction>|<label>)*/$/ ;",
                               isExport, ident, hex, hexLiteral, commaSepHexLits, keyValuePair, commaSepKeyValuePairs,
                               data, data8, data16, constant, structure, instruction, label, c16parser, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        return NULL;
    }

    mpc_result_t *result = malloc(sizeof(mpc_result_t));
    if (mpc_parse("<stdin>", input, c16parser, result))
    {
        mpc_ast_print(result->output);
        mpc_cleanup(14, isExport, ident, hex, hexLiteral, commaSepHexLits, keyValuePair, commaSepKeyValuePairs,
                    data, data8, data16, constant, structure, instruction, label, c16parser);
        return result;
    }
    else
    {
        mpc_err_print(result->error);
        mpc_err_delete(result->error);
        return NULL;
    }

    mpc_cleanup(14, isExport, ident, hex, hexLiteral, commaSepHexLits, keyValuePair, commaSepKeyValuePairs,
                data, data8, data16, constant, structure, instruction, label, c16parser);

    return NULL;
}