#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_semantic.h"
#include "printer.c"

void interpretation(struct object_st *expr_obj);

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
    if (error->present) {
        printf("Tokenize Error : ");
        printf("%s\n", error->type->data);
        printf("%s\n", error->message->data);
        printf("%d\n", error->pos);
        printf("%d\n", error->line_num);
        printf("%d\n", error->line_pos);
        exit(-1);
    }


    object_set_type(expr_obj, NODE_TYPE);
    ast_parser_set_list(T_parser, F_parser);
    // AST Analyze
    token_analyzer(T_parser, expr_obj->data);
    ast_parser_get_error(T_parser, error);
    // Print AST Analyze Result
    if (error->present) {
        printf("Tokenize Error : ");
        printf("%s\n", error->type->data);
        printf("%s\n", error->message->data);
        printf("%d\n", error->pos);
        printf("%d\n", error->line_num);
        printf("%d\n", error->line_pos);
        exit(-1);
    }


    semantic_scan(expr_obj, error);
    if (error->present) {
        printf("Tokenize Error : ");
        printf("%s\n", error->type->data);
        printf("%s\n", error->message->data);
        printf("%d\n", error->pos);
        printf("%d\n", error->line_num);
        printf("%d\n", error->line_pos);
        exit(-1);
    }

    clock_t begin = clock();

    interpretation(expr_obj);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time : %f\n", time_spent);

    ast_parser_free(T_parser);
    la_parser_free(F_parser);

}
