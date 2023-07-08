#include <c16parser.h>


int main(int argc, char** argv)
{
    char* input = "constant  ad =$ff \n +structure rectangle { x:$00, y:$00, width:$ff, height:$ff }";

    // parse the input
    mpc_result_t* result = c16parser_parse(input);
}