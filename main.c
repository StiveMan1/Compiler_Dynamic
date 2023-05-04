#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_semantic.h"
#include "printer.c"

void print_error_log(struct error_st* error, struct la_parser* F_parser){
    if (!error->present) return;
    printf("%s\n", error->type->data);
    if (error->message->size > 0)
        printf("%s\n", error->message->data);
    printf("Line %zu: \n", error->line_num + 1);
    for (size_t i = error->line_pos; i < F_parser->str_size; i++) {
        if (F_parser->data[i] == '\n') break;
        printf("%c", F_parser->data[i]);
    }
    printf("\n");
    for (size_t i = error->line_pos; i < error->pos; i++) printf(" ");
    printf("^\n");
    exit(-1);
}

void interpretation(struct object_st *expr_obj, struct error_st *);

int main() {
    struct error_st *error = error_new();
    struct object_st *expr_obj = object_new();
    struct la_parser *F_parser = la_parser_new();
    struct ast_parser *T_parser = ast_parser_new();

    // Reading input
    la_parser_set_file(F_parser, "text.txt");
    // Tokenize
    tokenize(F_parser);
    la_parser_get_error(F_parser, error);
    // Print Tokenize Result
    print_error_log(error, F_parser);

    object_set_type(expr_obj, NODE_TYPE);
    ast_parser_set_list(T_parser, F_parser);
    // AST Analyze
    token_analyzer(T_parser, expr_obj->data);
    ast_parser_get_error(T_parser, error);
    // Print AST Analyze Result
    print_error_log(error, F_parser);

    semantic_scan(expr_obj, error);
    print_error_log(error, F_parser);

    clock_t begin = clock();

    interpretation(expr_obj,error);
    print_error_log(error, F_parser);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time : %f\t", time_spent);

    ast_parser_free(T_parser);
    la_parser_free(F_parser);
    object_free(expr_obj);
    error_free(error);
    printf("Memory : %lld", mem_ctx.filled);

}
