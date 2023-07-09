#ifndef _c16parser_h_
#define _c16parser_h_
#include <mpc.h>
#include <string.h>


void c16parser_interpretAs(mpc_parser_t **parser, mpc_parser_t **ident);
void c16parser_ident(mpc_parser_t **parser);

mpc_result_t* c16parser_parse(const char* input);

#endif // _c16parser_h_