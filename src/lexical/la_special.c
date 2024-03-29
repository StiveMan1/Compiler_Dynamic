#include <stdio.h>
#include "lexical_analysis.h"

// Special chars with length 1
short Special_OneChar(char c1) {
    switch (c1) {
        // Special Chars OP
        case '%':
            return Special_MOD;
        case '&':
            return Special_AND;
        case '*':
            return Special_MUL;
        case '+':
            return Special_ADD;
        case '-':
            return Special_SUB;
        case '/':
            return Special_DIV;
        case '^':
            return Special_XOR;
        case '|':
            return Special_OR;

            // Special Chars Brackets
        case '(':
            return Special_LSB;
        case ')':
            return Special_RSB;
        case '[':
            return Special_LSQB;
        case ']':
            return Special_RSQB;
        case '{':
            return Special_LCB;
        case '}':
            return Special_RCB;

            // Special Chars Compare
        case '<':
            return Special_LESS;
        case '>':
            return Special_GREATER;
        case '=':
            return Special_EQ_EQ;

        case ',':
            return Special_COMMA;
        case '.':
            return Special_DOT;
        case ';':
            return Special_SEMI;

        default:
            break;
    }
    return Special_None;
}

// Special chars with length 2
short Special_TwoChar(char c1, char c2) {
    switch (c1) {
        case '*':
            if (c2 == '/') return Special_COM_END;
            break;
        case '/':
            if (c2 == '/') return Special_COM_LINE;
            if (c2 == '*') return Special_COM_STR;
            if (c2 == '=') return Special_EQ_NOT;
            break;
        case '<':
            if (c2 == '<') return Special_LSHIFT;
            if (c2 == '=') return Special_EQ_LESS;
            break;
        case '>':
            if (c2 == '>') return Special_RSHIFT;
            if (c2 == '=') return Special_EQ_GREATER;
            break;
        case ':':
            if (c2 == '=') return Special_EQ;
            break;
        case '=':
            if (c2 == '>') return Special_ARROW;
            break;
        default:
            break;
    }
    return Special_None;
}

// Parsing operations, scopes, comments, delimiters, etc
void la_special(struct token_st *token, struct la_parser *parser) {
    // If 2-symbol length...
    short result = Special_TwoChar(parser->data[parser->position], parser->data[parser->position + 1]);
    if (result != Special_None) {
        token->type = TokenType_Special;
        token->subtype = result;
        token_set_pos(token, parser);
        parser->position += 2;
        return;
    }

    // 1-symbol length...
    result = Special_OneChar(parser->data[parser->position]);
    if (result != Special_None) {
        token->type = TokenType_Special;
        token->subtype = result;
        token_set_pos(token, parser);
        parser->position += 1;
    }
}