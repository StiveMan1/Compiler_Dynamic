#include "lexical_analysis.h"

#define CharInt_bin(c) ((c) >= '0' && (c) <= '1')
#define CharInt_oct(c) ((c) >= '0' && (c) <= '7')
#define CharInt_dec(c) ((c) >= '0' && (c) <= '9')
#define CharInt_hex(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))

#define GetChar {pos++;if (pos == parser->str_size) return;c = parser->data[pos];}

void la_integer(struct token_st *token, struct la_parser *parser) {
    size_t pos = parser->position;
    if (CharInt_dec(parser->data[pos])) {
        char c = parser->data[pos];
        if (c == '0') {
            GetChar
            if (c == 'b' || c == 'B') {
                GetChar
                while (c == '_') GetChar
                size_t zero_end = pos;
                while (CharInt_bin(c)) GetChar
                if (IdentifierStart(c)) return;

                token->type = TokenType_Int;
                token->subtype = IntType_bin;
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                parser->position = pos;
            } else if (c == 'o' || c == 'O') {
                GetChar
                while (c == '_') GetChar
                size_t zero_end = pos;
                while (CharInt_oct(c)) GetChar
                if (IdentifierStart(c)) return;

                token->type = TokenType_Int;
                token->subtype = IntType_oct;
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                parser->position = pos;
            } else if (c == 'x' || c == 'X') {
                GetChar
                while (c == '_') GetChar
                size_t zero_end = pos;
                while (CharInt_hex(c)) GetChar
                if (IdentifierStart(c)) return;

                token->type = TokenType_Int;
                token->subtype = IntType_hex;
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                parser->position = pos;
            } else {
                while (c == '0') GetChar
                size_t zero_end = pos;
                while (CharInt_dec(c)) GetChar

                if (c == '.') {
                    GetChar
                    while (CharInt_dec(c)) GetChar
                    token->subtype = IntType_float;
                } else {
                    token->subtype = IntType_dec;
                }
                if (IdentifierStart(c)) return;

                token->type = TokenType_Int;
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                parser->position = pos;
            }
        } else {
            while (CharInt_dec(c)) GetChar
            if (c == '.') {
                GetChar
                while (CharInt_dec(c)) GetChar
                token->subtype = IntType_float;
            } else {
                token->subtype = IntType_dec;
            }
            if (IdentifierStart(c)) return;

            token->type = TokenType_Int;
            token_resize(token, pos - parser->position);
            memcpy(token->data, &parser->data[parser->position], token->size);
            parser->position = pos;
        }
    }
}