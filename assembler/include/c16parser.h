#ifndef _c16parser_h_
#define _c16parser_h_
#include <mpc.h>
#include <string.h>

typedef struct _c16parser_node {
    mpc_parser_t *parser;
    struct _c16parser_node *next;
} c16parser_node;

/*
*  Stores a parser in a list of parsers.
*  The list is used to free the parsers when the program ends.
*/
void _c16parser_addTempParser(c16parser_node *anchor, mpc_parser_t *parser);

/*
*  Frees all the parsers in the list.
*/
void _c16parser_freeTempParsers(c16parser_node *anchor);

/*
*  Runs all parsers specified in this function.
*  Only function that should be called from outside this file.
*
*  Returns the result of the last parser. (Needs to be freed)
*/
mpc_result_t *c16parser_run(char *input);

// returns a many parser that matches any of the parsers specified in this function
// named "c16parser"
mpc_parser_t *_c16parser_build(c16parser_node *anchor);

// Parsers:

// All Parsers will be named _c16parser_<name of parser>
// If a parser uses other_parsers, they will be added to the list of temp_parsers and freed by _c16parser_freeTempParsers

// data
mpc_parser_t *_c16parser_dataParser(char *size, c16parser_node *anchor);
mpc_parser_t *_c16parser_data16(c16parser_node *anchor);
mpc_parser_t *_c16parser_data8(c16parser_node *anchor);

// constant
mpc_parser_t *_c16parser_constant();

// structure
mpc_parser_t *_c16parser_structure();

// instructions
// Iterate over all intructions uses meta data out of ../include/meta.h
mpc_parser_t *_c16parser_instruction();

// label
mpc_parser_t *_c16parser_label();

// The main parsers that will be run are:
// _c16parser_data16
// _c16parser_data8
// _c16parser_constant
// _c16parser_structure
// _c16parser_instruction
// _c16parser_label

// Parsers that are used by other parsers
mpc_parser_t *_c16parser_register();
mpc_parser_t *_c16parser_hex();
mpc_parser_t *_c16parser_hexLiteral();
mpc_parser_t *_c16parser_address();
mpc_parser_t *_c16parser_validIdentifier();
mpc_parser_t *_c16parser_variable();
mpc_parser_t *_c16parser_operator();

#endif // _c16parser_h_