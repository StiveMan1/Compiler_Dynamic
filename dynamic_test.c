#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_semantic.h"
#include "printer.c"

void interpretation(struct object_st *expr_obj, struct error_st *, int stream);

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

void check_error(struct error_st *error, struct error_st *expected_error, char* type, struct la_parser* F_parser){
    int is_error_null = error == NULL || error_is_null(error);
    if (!is_error_null) {
        print_error_log(error, F_parser);
    }
    if (expected_error == NULL){
        if (is_error_null) return;
        else exit(1);
    }
    if (is_error_null){
        if (type_to_step(type) < type_to_step(expected_error->type->data)) return;
        else exit(1);
    }
    if (error_cmp(error, expected_error) == 0) exit(0); // Finishing the execution successfully
    exit(1);
}

void read_error( char* error_file, struct error_st *expected_error){
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

void read_output(char* result_file, struct string_st *interp_out){
    FILE *fp = fopen(result_file, "r");
    if (fp == NULL) exit(-1);
    fseek(fp, 0, SEEK_END);
    size_t str_size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    char *data = Malloc(str_size);
    memset(data, 0, str_size);
    char c;
    size_t i;
    for (i = 0; i < str_size && (c = (char)getc(fp)) != EOF; i++) {
        data[i] = c;
    }
    str_size = i;
    fclose(fp);

    string_set_str(interp_out, data, str_size);
}

// Function that tests execution of all interpreter modules
int main(int argc, char *argv[]) {
    if (argc == 0)
        return -1;

    char* path = argv[1];

    char* code_file = concat(path, "/code.txt");
    char* error_file = concat(path, "/error.txt");
    char* result_file = concat(path, "/out.txt");

    struct error_st *error = NULL;
    struct error_st *expected_error = NULL;
    struct string_st *expected_interp_out = string_new();

    struct la_parser *F_parser = la_parser_new();
    struct ast_parser *T_parser = ast_parser_new();
    struct object_st *expr_obj = object_new();

    la_parser_set_file(F_parser, code_file);

    // If error file present, then check if there will be an error
    if (access(error_file, F_OK) == 0) read_error(error_file, expected_error);
    // Otherwise, check output
    else read_output(result_file, expected_interp_out);

    // Lexical
    tokenize(F_parser);
    check_error(error, expected_error, LEXICAL_ANALYSIS_ERROR, F_parser);

    // AST parser
    object_set_type(expr_obj, NODE_TYPE);
    ast_parser_set_list(T_parser, F_parser);
    token_analyzer(T_parser, expr_obj->data);
    check_error(error, expected_error, SYNTAX_ANALYSIS_ERROR, F_parser);

    error = error_new();

    // Semantic
    semantic_scan(expr_obj, error);
    check_error(error, expected_error, SEMANTIC_ANALYSIS_ERROR, F_parser);

    // Interpretation

    int stdout_bk = dup(fileno(stdout)); // fd for stdout backup
    int fd[2]; // an array that will hold two file descriptors
    _pipe(fd, 0, 0);

    dup2(fd[1], fileno(stdout));

    fflush(stdout);//flushall();
    interpretation(expr_obj, error, fd[1]);
    check_error(error, expected_error, INTERPRETER_ERROR, F_parser);
    close(fd[1]);
    dup2(stdout_bk, fileno(stdout));//restore

    ast_parser_free(T_parser);
    la_parser_free(F_parser);

    char ch;
    int index = 0;
    while(read(fd[0], &ch, 1) > 0) {
        if (expected_interp_out->size == index || expected_interp_out->data[index] != ch)
            exit(1);
        index++;
    }
    if (expected_interp_out->size != index) exit(1);
}