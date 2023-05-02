#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_semantic.h"
#include "printer.c"

void interpretation(struct object_st *expr_obj);

int main() {
//    struct object_st *expr_obj = object_new();
    struct la_parser *F_parser = la_parser_new();
//    struct ast_parser *T_parser = ast_parser_new();


    // Reading input
    la_parser_set_file(F_parser, "text.txt");
    // Tokenize
    tokenize(F_parser);
    // Print Tokenize Result
    if (F_parser->error_obj->present) {
        printf("Tokenize Error : ");
        printf("%s\n", F_parser->error_obj->type->data);
        printf("%s\n", F_parser->error_obj->message->data);
        printf("%d\n", F_parser->error_obj->pos);
        printf("%d\n", F_parser->error_obj->line_num);
        printf("%d\n", F_parser->error_obj->line_pos);
//        for (int i = 0; i < F_parser->error_obj->message->size; i++) printf("%c", F_parser->error_obj->message->data[i]);
//        printf("\nLine %zu: \n", F_parser->current_line + 1);
//        for (size_t i = F_parser->line_pos; i < F_parser->str_size; i++) {
//            if (F_parser->data[i] == '\n') break;
//            printf("%c", F_parser->data[i]);
//        }
//        printf("\n");
//        for (size_t i = F_parser->line_pos; i < F_parser->position; i++) printf(" ");
//        printf("^\n");
//        exit(-1);
    }


//    object_set_type(expr_obj, NODE_TYPE);
//    ast_parser_set_list(T_parser, F_parser);
//    // AST Analyze
//    int ast_result = token_analyzer(T_parser, expr_obj->data);
//    // Print AST Analyze Result
//    if (ast_result != SN_Status_Success) {
//        if(ast_result == SN_Status_EOF) {
//            printf("Unexpected EOF\n");
//        } else {
//            printf("Syntax Error : ");
//            struct token_st *token = T_parser->list->data[T_parser->error_pos]->data;
//            printf("Line %zu: \n", token->line_num + 1);
//            for (size_t i = token->line_pos; i < F_parser->str_size; i++) {
//                if (F_parser->data[i] == '\n') break;
//                printf("%c", F_parser->data[i]);
//            }
//            printf("\n");
//            // TODO : errors with unclosed brackets are shown on the last token (can be on
//            //  a different line and not understandable). Consider showing the error on the
//            //  start bracket.
//            for (size_t i = token->line_pos; i < token->pos; i++) printf(" ");
//            printf("^\n");
//        }
//        exit(-1);
//    }
//
//
//    ast_result = semantic_scan(expr_obj);
//    if(ast_result != 0) {
//        printf("Semantic Error : \n");
//        switch (ast_result) {
//            case -SemanticError_Ident:
//                printf("Identifier not initialized\n");
//                break;
//            case -SemanticError_Return:
//                printf("Return Statement not in Function Scopes\n");
//                break;
//        }
//        exit(-1);
//    }

    // print_obj(expr_obj, 0);

//    clock_t begin = clock();
//
//    interpretation(expr_obj);
//
//    clock_t end = clock();
//    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//    printf("Time : %f\n", time_spent);

//    ast_parser_free(T_parser);
    la_parser_free(F_parser);

}
