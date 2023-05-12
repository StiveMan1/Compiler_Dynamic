#include <stdio.h>
#include <time.h>
#include "struct.h"
#include "ast_semantic.h"
#include "printer.c"

#define PRINT_ERROR print_error_log(error, F_parser); if (error->present) exit(1);

void interpretation(struct imp_parser *parser, int stream);

int main() {
    struct error_st *error = error_new();
    struct la_parser *F_parser = la_parser_new();
    struct ast_parser *T_parser = ast_parser_new();
    struct imp_parser *I_parser = imp_parser_new();

    // Reading input
    la_parser_set_file(F_parser, "text.txt");
    // Tokenize
    tokenize(F_parser);
    la_parser_get_error(F_parser, error);
    // Print Tokenize Result
    PRINT_ERROR

    ast_parser_set_list(T_parser, F_parser);
    // AST Analyze
    token_analyzer(T_parser, T_parser->tree);
    ast_parser_get_error(T_parser, error);
    // Print AST Analyze Result
    PRINT_ERROR

    semantic_scan(T_parser);
    ast_parser_get_error(T_parser, error);
    PRINT_ERROR

    imp_parser_set_tree(I_parser, T_parser);
    clock_t begin = clock();

    interpretation(I_parser, 1);
    imp_parser_get_error(I_parser, error);
    PRINT_ERROR

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time : %f\t", time_spent);

    la_parser_free(F_parser);
    ast_parser_free(T_parser);
    imp_parser_free(I_parser);
    error_free(error);
    printf("Memory : %lld", mem_ctx.filled);

}
