#include <dirent.h>
#include <assert.h>
#include <stdio.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_semantic.h"
#include "printer.c"

void interpretation(struct object_st *expr_obj, struct error_st *);

char* concat(char* s1, char* s2){
    char* name_with_extension;
    name_with_extension = Malloc(strlen(s1) + 1 + strlen(s2));
    strcpy(name_with_extension, s1);
    strcat(name_with_extension, s2);
    return name_with_extension;
}

int type_to_step(char* type){
    if (strcmp(type, LEXICAL_ANALYSIS_ERROR) != 0) return 1;
    else if (strcmp(type, SYNTAX_ANALYSIS_ERROR) != 0) return 3;
    else if (strcmp(type, SEMANTIC_ANALYSIS_ERROR) != 0) return 2;
    return 1;
}

void check_error(struct error_st *error, struct error_st *expected_error, char* type){
    if (expected_error == NULL){
        if (error == NULL) return;
        else exit(1);
    }
    if (error == NULL){
        if (type_to_step(type) < type_to_step(expected_error->type->data)) return;
        else exit(1);
    }
    if (error_cmp(error, expected_error) == 0) exit(0); // Finishing the execution successfully
    exit(1);
}

// Function that tests execution of all interpreter modules
int main(int argc, char *argv[]) {
    if (argc == 0)
        return -1;

    char* path = argv[1];

    char* code_file = concat(path, "code.txt");
    char* result_file = concat(path, "result.txt");
    char* error_file = concat(path, "error.txt");

    struct error_st *error = NULL;
    struct error_st *expected_error = NULL;

    struct la_parser *F_parser = la_parser_new();
    struct ast_parser *T_parser = ast_parser_new();
    struct object_st *expr_obj = object_new();

    la_parser_set_file(F_parser, code_file);

    // If result file present, then check output of interpretator
    if (access(result_file, F_OK) == 0) {

    } // Otherwise, check the error
    else {
        struct la_parser *error_parser = la_parser_new();
        struct token_st *token = NULL;
        la_parser_set_file(error_parser, error_file);
        tokenize(error_parser);
        expected_error = error_new();
        token = error_parser->list->data[2]->data;
        string_set_str(expected_error->type, token->data, token->size);
        token = error_parser->list->data[5]->data;
        string_set_str(expected_error->message, token->data, token->size);
        token = error_parser->list->data[8]->data;
        expected_error->pos = strtol(token->data, NULL, 10);
        token = error_parser->list->data[11]->data;
        expected_error->line_num = strtol(token->data, NULL, 10);
        token = error_parser->list->data[14]->data;
        expected_error->line_pos = strtol(token->data, NULL, 10);
    }

    // Lexical
    tokenize(F_parser);
    check_error(error, expected_error, LEXICAL_ANALYSIS_ERROR);

    // AST parser
    object_set_type(expr_obj, NODE_TYPE);
    ast_parser_set_list(T_parser, F_parser);
    token_analyzer(T_parser, expr_obj->data);
    check_error(error, expected_error, SYNTAX_ANALYSIS_ERROR);

    error = error_new();

    // Semantic
    semantic_scan(expr_obj, error);
    check_error(error, expected_error, SEMANTIC_ANALYSIS_ERROR);

    // Interpretation
    interpretation(expr_obj, error);
    check_error(error, expected_error, INTERPRETER_ERROR);

    // if (strcmp())

    ast_parser_free(T_parser);
    la_parser_free(F_parser);
}