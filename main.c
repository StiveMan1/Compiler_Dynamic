#include <stdio.h>
#include "struct.h"
#include "lexical_analysis.h"
#include "ast_analyze.h"

#define PRINT_PREF for(int _i=0;_i<size;_i++)printf("%c",prefix[_i]);
//#define PRINT_NEXT(expr) if(expr){printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';
#ifdef WIN32
    #ifdef DEBUG
    #define PRINT_NEXT(expr) if(expr){printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';
    #else
    #define PRINT_NEXT(expr) if(expr){printf("\t%c- ",195);prefix[size + 1] = '|';}else{printf("\t%c- ",192);prefix[size + 1] = ' ';}prefix[size] = '\t';
    #endif
#else
#define PRINT_NEXT(expr) if(expr){printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';
#endif

char prefix[100];


void print_str(const struct string_st *res) {
    printf("string (%zu): ", res->size);
    for (int i = 0; i < res->size; i++) printf("%c", res->data[i]);
    printf("\n");
}

void print_int(const struct integer_st *res) {
    printf("integer : %ld\n", res->data);
}

void print_bool(const struct bool_st *res) {
    printf("bool : %hhd\n", res->data);
}

void print_real(const struct real_st *res) {
    printf("real : %f\n", res->data);
}

void print_array(const struct array_st *res, int size) {
    printf("array (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_obj(res->data[i], size + 2);
    }
}

void print_tuple(const struct tuple_st *res, int size) {
    printf("array (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_obj(res->data[i], size + 2);
    }
}

void print_token(const struct token_st *res, int size) {
    printf("Token : ");
    switch (res->type) {
        case TokenType_None:
            printf("TokenType_None ");
            break;
        case TokenType_KeyWords:
            printf("TokenType_KeyWords ");
            break;
        case TokenType_Identifier:
            printf("TokenType_Identifier ");
            break;
        case TokenType_String:
            printf("TokenType_String ");
            break;
        case TokenType_Int:
            printf("TokenType_Int ");
            break;
        case TokenType_Special:
            printf("TokenType_Special ");
            break;
    }
    if (res->type == TokenType_Int) {
        switch (res->subtype) {
            case IntType_bin:
                printf("IntType_bin ");
                break;
            case IntType_oct:
                printf("IntType_oct ");
                break;
            case IntType_dec:
                printf("IntType_dec ");
                break;
            case IntType_hex:
                printf("IntType_hex ");
                break;
            case IntType_float:
                printf("IntType_float ");
                break;
        }
    }
    if (res->type == TokenType_KeyWords) {
        switch (res->subtype) {
            case KeyWord_IF:
                printf("KeyWord_IF ");
                break;
            case KeyWord_IN:
                printf("KeyWord_IN ");
                break;
            case KeyWord_IS:
                printf("KeyWord_IS ");
                break;
            case KeyWord_OR:
                printf("KeyWord_OR ");
                break;

            case KeyWord_AND:
                printf("KeyWord_AND ");
                break;
            case KeyWord_END:
                printf("KeyWord_END ");
                break;
            case KeyWord_FOR:
                printf("KeyWord_FOR ");
                break;
            case KeyWord_INT:
                printf("KeyWord_INT ");
                break;
            case KeyWord_NOT:
                printf("KeyWord_NOT ");
                break;
            case KeyWord_VAR:
                printf("KeyWord_VAR ");
                break;
            case KeyWord_XOR:
                printf("KeyWord_XOR ");
                break;

            case KeyWord_BOOL:
                printf("KeyWord_BOOL ");
                break;
            case KeyWord_ELSE:
                printf("KeyWord_ELSE ");
                break;
            case KeyWord_FUNC:
                printf("KeyWord_FUNC ");
                break;
            case KeyWord_LOOP:
                printf("KeyWord_LOOP ");
                break;
            case KeyWord_REAL:
                printf("KeyWord_REAL ");
                break;
            case KeyWord_THEN:
                printf("KeyWord_THEN ");
                break;
            case KeyWord_TRUE:
                printf("KeyWord_TRUE ");
                break;

            case KeyWord_EMPTY:
                printf("KeyWord_EMPTY ");
                break;
            case KeyWord_FALSE:
                printf("KeyWord_FALSE ");
                break;
            case KeyWord_PRINT:
                printf("KeyWord_PRINT ");
                break;
            case KeyWord_WHILE:
                printf("KeyWord_WHILE ");
                break;
        }
    }
    printf("\n");
    if (res->type == TokenType_Int || res->type == TokenType_Identifier || res->type == TokenType_String) {
        PRINT_PREF
        PRINT_NEXT(0)
        for (int i = 0; i < res->size; i++) printf("%c", res->data[i]);
        printf("\n");
    } else if (res->type == TokenType_Special) {
        PRINT_PREF
        PRINT_NEXT(0)
        switch (res->subtype) {
            case Special_MOD:
                printf("%c", '%');
                break;
            case Special_AND:
                printf("&");
                break;
            case Special_MUL:
                printf("*");
                break;
            case Special_ADD:
                printf("+");
                break;
            case Special_SUB:
                printf("-");
                break;
            case Special_DIV:
                printf("/");
                break;
            case Special_XOR:
                printf("^");
                break;
            case Special_OR:
                printf("|");
                break;
            case Special_LSHIFT:
                printf("<<");
                break;
            case Special_RSHIFT:
                printf(">>");
                break;

            case Special_LSB:
                printf("(");
                break;
            case Special_RSB:
                printf(")");
                break;
            case Special_LSQB:
                printf("[");
                break;
            case Special_RSQB:
                printf("]");
                break;
            case Special_LCB:
                printf("{");
                break;
            case Special_RCB:
                printf("}");
                break;

            case Special_LESS:
                printf("<");
                break;
            case Special_GREATER:
                printf(">");
                break;
            case Special_EQ_LESS:
                printf("<=");
                break;
            case Special_EQ_GREATER:
                printf(">=");
                break;
            case Special_EQ_NOT:
                printf("/=");
                break;
            case Special_EQ_EQ:
                printf("=");
                break;

            case Special_COMMA:
                printf(",");
                break;
            case Special_DOT:
                printf(".");
                break;
            case Special_SEMI:
                printf(";");
                break;
            case Special_EQ:
                printf(":=");
                break;
        }
        printf("\n");
    }
}

void print_node(const struct node_st *res, int size) {
    printf("Node : ");
    switch (res->main_type) {
        case MainType_None:
            printf("MainType_None ");
            break;
        case MainType_Expr:
            printf("MainType_Expr ");
            break;
        case MainType_Oper:
            printf("MainType_Oper ");
            break;
        case MainType_Stmt:
            printf("MainType_Stmt ");
            break;
    }
    if (res->main_type == MainType_Expr) {
        switch (res->type) {
            case PrimType_Tuple:
                printf("PrimType_Tuple ");
                break;
            case PrimType_List:
                printf("PrimType_List ");
                break;
            case PrimType_Ident_new:
                printf("PrimType_Ident_new ");
                break;
            case PrimType_Ident_get:
                printf("PrimType_Ident_get ");
                break;
            case PrimType_Literal:
                printf("PrimType_Literal ");
                break;
            case PrimType_Attrib:
                printf("PrimType_Attrib ");
                break;
            case PrimType_Subscript:
                printf("PrimType_Subscript ");
                break;
            case PrimType_Call:
                printf("PrimType_Call ");
                break;
        }
    }
    if (res->main_type == MainType_Oper) {
        switch (res->type) {
            case ExprType_U:
                printf("ExprType_U ");
                break;
            case ExprType_M:
                printf("ExprType_M ");
                break;
            case ExprType_A:
                printf("ExprType_A ");
                break;
            case ExprType_Shift:
                printf("ExprType_Shift ");
                break;
            case ExprType_And:
                printf("ExprType_And ");
                break;
            case ExprType_Xor:
                printf("ExprType_Xor ");
                break;
            case ExprType_Or:
                printf("ExprType_Or ");
                break;
            case ExprType_Comp:
                printf("ExprType_Comp ");
                break;
            case ExprType_NotTest:
                printf("ExprType_NotTest ");
                break;
            case ExprType_AndTest:
                printf("ExprType_AndTest ");
                break;
            case ExprType_OrTest:
                printf("ExprType_OrTest ");
                break;
        }
    }
    if (res->main_type == MainType_Stmt) {
        switch (res->type) {
            case StmtType_If:
                printf("StmtType_If ");
                break;
            case StmtType_While:
                printf("StmtType_While ");
                break;
            case StmtType_DoWhile:
                printf("StmtType_DoWhile ");
                break;
            case StmtType_For:
                printf("StmtType_For ");
                break;
            case StmtType_ForHeader:
                printf("StmtType_ForHeader ");
                break;
            case StmtType_Try:
                printf("StmtType_Try ");
                break;
            case StmtType_Params:
                printf("StmtType_Params ");
                break;
            case StmtType_Func:
                printf("StmtType_Func ");
                break;
            case StmtType_Class:
                printf("StmtType_Class ");
                break;
            case StmtType_Annot:
                printf("StmtType_Annot ");
                break;
            case StmtType_Assign:
                printf("StmtType_Assign ");
                break;
            case StmtType_Return:
                printf("StmtType_Return ");
                break;
            case StmtType_Break:
                printf("StmtType_Break ");
                break;
            case StmtType_Continue:
                printf("StmtType_Continue ");
                break;
            case StmtType_Import:
                printf("StmtType_Import ");
                break;
            case StmtType_List:
                printf("StmtType_List ");
                break;
            case StmtType_Extends:
                printf("StmtType_Extends ");
                break;
            case StmtType_Print:
                printf("StmtType_Print ");
                break;
        }
    }
    printf("\n");
    if (res->data != NULL) {
        PRINT_PREF
        PRINT_NEXT(!array_is_null(res->tokens) || !array_is_null(res->next))
        print_obj(res->data, size + 2);
    }
    if (!array_is_null(res->tokens)) {
        PRINT_PREF
        PRINT_NEXT(!array_is_null(res->next))
        print_array(res->tokens, size + 2);
    }
    if (!array_is_null(res->next)) {
        PRINT_PREF
        PRINT_NEXT(0)
        print_array(res->next, size + 2);
    }
}

void print_obj(const struct object_st *res, int size) {
//    printf("object : (%d)\n", res->counter);
//    PRINT_PREF
//    PRINT_NEXT(0)
    if (res->type == NONE_TYPE) printf("None\n");
    else if (res->type == OBJECT_TYPE) return print_obj(res->data, size);
    else if (res->type == STRING_TYPE) return print_str(res->data);
    else if (res->type == INTEGER_TYPE) return print_int(res->data);
    else if (res->type == BOOL_TYPE) return print_bool(res->data);
    else if (res->type == REAL_TYPE) return print_real(res->data);
    else if (res->type == ARRAY_TYPE) return print_array(res->data, size);
    else if (res->type == TUPLE_TYPE) return print_tuple(res->data, size);
    else if (res->type == TOKEN_TYPE) return print_token(res->data, size);
    else if (res->type == NODE_TYPE) return print_node(res->data, size);
}

int main() {
    struct object_st *expr_obj = object_new();
    struct la_parser *F_parser = la_parser_new();
    struct ast_parser *T_parser = ast_parser_new();


    // Reading input
    la_parser_set_file(F_parser, "text.txt");
    // Tokenize
    tokenize(F_parser);
    // Print Tokenize Result
    if (string_is_null(F_parser->error_msg)) {
//        print_array(F_parser->list, 0);
        // AST data init
        object_set_type(expr_obj, NODE_TYPE);
        ast_parser_set_list(T_parser, F_parser);
        // AST Analyze
        char ast_result = token_analyzer(T_parser, expr_obj->data);
        // Print AST Analyze Result
        if (ast_result != SN_Status_Success) {
            if(ast_result == SN_Status_EOF) {
                printf("Unexpected EOF\n");
            } else {
                printf("Syntax Error\n");
                struct token_st *token = T_parser->list->data[T_parser->error_pos]->data;
                printf("Line %zu: \n", token->line_num + 1);
                for (size_t i = token->line_pos; i < F_parser->str_size; i++) {
                    if (F_parser->data[i] == '\n') break;
                    printf("%c", F_parser->data[i]);
                }
                printf("\n");
                // TODO : errors with unclosed brackets are shown on the last token (can be on
                //  a different line and not understandable). Consider showing the error on the
                //  start bracket.
                for (size_t i = token->line_pos; i < token->pos; i++) printf(" ");
                printf("^\n");
            }
        } else {
            print_obj(expr_obj, 0);
        }
    } else {
        printf("Error : ");
        for (int i = 0; i < F_parser->error_msg->size; i++) printf("%c", F_parser->error_msg->data[i]);
        printf("\nLine %zu: \n", F_parser->current_line + 1);
        for (size_t i = F_parser->line_pos; i < F_parser->str_size; i++) {
            if (F_parser->data[i] == '\n') break;
            printf("%c", F_parser->data[i]);
        }
        printf("\n");
        for (size_t i = F_parser->line_pos; i < F_parser->position; i++) printf(" ");
        printf("^\n");
    }


    ast_parser_free(T_parser);
    la_parser_free(F_parser);

}
