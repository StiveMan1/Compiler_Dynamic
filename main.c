#include <stdio.h>
#include "struct.h"
#include "lexical_analysis.h"

#define PRINT_PREF for(int _i=0;_i<size;_i++)printf("%c",prefix[_i]);
#ifdef WIN32
#define PRINT_NEXT(expr) if(expr){printf("\t%c- ",195);prefix[size + 1] = '|';}else{printf("\t%c- ",192);prefix[size + 1] = ' ';}prefix[size] = '\t';
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
void print_obj(const struct object_st *res, int size) {
    printf("object : (%d)\n", res->counter);
    PRINT_PREF
    PRINT_NEXT(0)
    if (res->type == NONE_TYPE) printf("None\n");
    else if (res->type == OBJECT_TYPE) return print_obj(res->data, size + 2);
    else if (res->type == STRING_TYPE) return print_str(res->data);
    else if (res->type == INTEGER_TYPE) return print_int(res->data);
    else if (res->type == BOOL_TYPE) return print_bool(res->data);
    else if (res->type == REAL_TYPE) return print_real(res->data);
    else if (res->type == ARRAY_TYPE) return print_array(res->data, size + 2);
    else if (res->type == TUPLE_TYPE) return print_tuple(res->data, size + 2);
    else if (res->type == TOKEN_TYPE) return print_token(res->data, size + 2);
}

int main() {
    struct la_parser *F_parser = la_parser_new();
    la_parser_set_file(F_parser, "text.txt");
    tokenize(F_parser);
    print_array(F_parser->list, 0);
    //            an_parser_set_list(T_parser, F_parser);
    la_parser_free(F_parser);


}

// Сделать сериализацию обектов
// У обектов будет две переменые _encode _decode
// работать так же как с обычными обектами

