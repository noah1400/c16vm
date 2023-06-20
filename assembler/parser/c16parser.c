#include "c16parser.h"

mpc_parser_t* c16parser_getStrUpperOrLower() {
    mpc_parser_t* strUpper = mpc_new("strUpper");
    mpc_parser_t* strLower = mpc_new("strLower");
    mpc_parser_t* strUpperOrLower = mpc_new("strUpperOrLower");

    mpca_lang(MPCA_LANG_DEFAULT,
        " strUpper: /[A-Z]+/ ; "
        " strLower: /[a-z]+/ ; "
        " strUpperOrLower: <strUpper> | <strLower> ; ",
        strUpper, strLower, strUpperOrLower, NULL);

    return strUpperOrLower;
}

mpc_parser_t* c16parser_getHexDigitParser()
{
    mpc_parser_t* hexDigit = mpc_new("hexDigit");
    mpca_lang(MPCA_LANG_DEFAULT,
        " hexDigit: /[0-9A-Fa-f]/ ; ",
        hexDigit, NULL);
    return hexDigit; 
}

mpc_parser_t* c16parser_getHexDigitLiteralParser(mpc_parser_t* hexDigit)
{
    mpc_parser_t* hexDigitLiteral = mpc_and(
        2,
        mpc_char('$'),
        hexDigit,
        free,
        free
    ); // Q: Will hexDigit be freed? 
}