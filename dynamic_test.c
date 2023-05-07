#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
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

void check_error(struct error_st *error, struct error_st *expected_error, char* type){
    int is_error_null = error == NULL || error_is_null(error);
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

void check_interp_out(struct string_st *interp_out, struct string_st *expected_interp_out){
    if (string_cmp(interp_out, expected_interp_out) != 0)
        exit(1);
    if (string_is_null(interp_out))
        exit(2);
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

    interp_out = string_new();
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

    ast_parser_free(T_parser);
    la_parser_free(F_parser);

    // Interpretation

    // https://www.rozmichelle.com/pipes-forks-dups/
    int fd[2]; // an array that will hold two file descriptors
    if (pipe(fd) == -1) { // populates fds with two file descriptors
        fprintf(stderr, "Pipe Failed");
        exit(-1);
    }
    pid_t pid = fork(); // create child process that is a clone of the parent

    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    // Child process
    else if (pid == 0) {
        // dup2(fd[0], STDIN_FILENO);  // fds[0] (the read end of pipe) donates its data to file descriptor 0
        close(fd[0]);   // file descriptor no longer needed in child since stdin is a copy

        char buf[128];
        /*int bytes_to_read, total_read_bytes, read_bytes;
        bytes_to_read = 128;
        total_read_bytes = 0;

        while ((read_bytes = read(0, buf + total_read_bytes, bytes_to_read) != 0)) {
            if (read_bytes < 0) return -1;
            total_read_bytes += read_bytes;
        }
        buf[total_read_bytes] = '\0';*/

        /*read(fd[0], buf, 128);

        struct string_st* interp_out = string_new();
        string_set_str(interp_out, buf, strlen(buf));

        check_interp_out(interp_out, expected_interp_out);*/

        close(fd[1]);
    }
    // Parent process
    else {
        close(fd[0]); // file descriptor unused in parent

        interpretation(expr_obj, error, fd[1]);

        // send EOF so child can continue (child blocks until all input has been processed):
        close(fd[1]);

        check_error(error, expected_error, INTERPRETER_ERROR);

        int status;
        pid_t wpid = waitpid(pid, &status, 0); // wait for child to finish before exiting
        printf("%d", status);
        return wpid == pid && WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}