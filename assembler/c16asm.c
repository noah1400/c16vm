#include <c16parser.h>


int main(void)
{
    char *input = "$fff";
    
    c16parser_node *anchor = NULL;
    mpc_result_t *r = malloc(sizeof(mpc_result_t));
    mpc_parser_t *test = mpc_new("test");
    mpc_parser_t *parser = _c16parser_hexLiteral(&anchor);
    mpc_define(test, parser);

    if (mpc_parse("test", input, test, r))
    {
        printf("Parsed: %s\n", r->output);
        mpc_ast_print(r->output);
        mpc_ast_delete(r->output);
    }
    else
    {
        printf("Failed to parse\n");
        mpc_err_print(r->error);
        mpc_err_delete(r->error);
    }
}