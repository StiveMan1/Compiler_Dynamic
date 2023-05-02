#include <dirent.h>
#include <stdio.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_semantic.h"
#include "printer.c"

struct error {
    int present;
    char *type;
    char *message;
    int line;
    int line_pos;
};

void interpretation(struct object_st *expr_obj);

void lexical_analysis(struct error* error, struct la_parser *F_parser){
    // Tokenize
    tokenize(F_parser);
    // Print Tokenize Result
    if (!string_is_null(F_parser->error_msg)) {
        error->present = 1;
        error->type = "Tokenize Error";
        error->message = F_parser->error_msg->data;
        error->line = F_parser->current_line + 1;
        error->line_pos = F_parser->line_pos;
    }
}

void syntactical_analysis(struct error* error, struct ast_parser *T_parser, struct object_st *expr_obj, struct la_parser *F_parser){
    object_set_type(expr_obj, NODE_TYPE);
    ast_parser_set_list(T_parser, F_parser);
    // AST Analyze
    int ast_result = token_analyzer(T_parser, expr_obj->data);
    // Print AST Analyze Result
    if (ast_result != SN_Status_Success) {
        if(ast_result == SN_Status_EOF) {
            error->present = 1;
            error->type = "Syntax Error";
            error->message = "Unexpected EOF";
        } else {
            struct token_st *token = T_parser->list->data[T_parser->error_pos]->data;
            error->present = 1;
            error->type = "Tokenize Error";
            // TODO : Create error message for ast_parser
            // error->message = T_parser->error_msg->data;
            error->line = token->line_num + 1;
            error->line_pos = token->line_pos;
        }
    }
}

void semantic_analysis(struct error* error, struct object_st *expr_obj){
    int ast_result = semantic_scan(expr_obj);
    if(ast_result != 0) {
        error->present = 1;
        error->type = "Semantic Error";
        switch (ast_result) {
            case -SemanticError_Ident:
                error->message = "Identifier not initialized";
                break;
            case -SemanticError_Return:
                error->message = "Return Statement not in Function Scopes";
                break;
        }
    }
}

void perform_interpretation(struct error* error, struct object_st *expr_obj){
    interpretation(expr_obj);
    // TODO : check errors for interpretation
}

// Function that tests execution of all interpreter modules
int main() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

    struct la_parser *F_parser = la_parser_new();
    struct ast_parser *T_parser = ast_parser_new();
    struct object_st *expr_obj = object_new();
    la_parser_set_file(F_parser, "text.txt"); // Reading input

    struct error* error = malloc(sizeof(struct error));

    lexical_analysis(error, F_parser);
    syntactical_analysis(error, T_parser, expr_obj, F_parser);
    semantic_analysis(error, expr_obj);
    perform_interpretation(error, expr_obj);

    ast_parser_free(T_parser);
    la_parser_free(F_parser);
}