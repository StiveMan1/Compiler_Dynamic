#include "lexical_analysis.h"

#define CharInt_bin(c) ((c) >= '0' && (c) <= '1')
#define CharInt_oct(c) ((c) >= '0' && (c) <= '7')
#define CharInt_dec(c) ((c) >= '0' && (c) <= '9')
#define CharInt_hex(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))

#define set_error_lexical(message) error_fill_in(parser->error_obj, LEXICAL_ANALYSIS_ERROR, message, parser->position, parser->current_line, parser->line_pos);
#define ErrInt {set_error_lexical("Error while parsing integer") return;}

#define GetChar {if (++pos == parser->str_size) ErrInt c = parser->data[pos];}

void la_integer(struct token_st *token, struct la_parser *parser) {
    size_t pos = parser->position;
    // Checking subtype 
    if (CharInt_dec(parser->data[pos])) {
        char c = parser->data[pos];
        if (c == '0') {
            GetChar
            // Binary
            if (c == 'b' || c == 'B') {
                GetChar
                while (c == '_') GetChar
                size_t zero_end = pos;
                while (CharInt_bin(c)) GetChar
                if (IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->subtype = IntType_bin;
                // Allocating space for token
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                token_set_pos(token, parser);
                parser->position = pos;
            } else if (c == 'o' || c == 'O') {
                GetChar
                while (c == '_') GetChar
                size_t zero_end = pos;
                while (CharInt_oct(c)) GetChar
                if (IdentifierStart(c)) ErrInt
                
                // Oct.
                token->type = TokenType_Int;
                token->subtype = IntType_oct;
                // Allocating space for token
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                token_set_pos(token, parser);
                parser->position = pos;
            } else if (c == 'x' || c == 'X') {
                GetChar
                while (c == '_') GetChar
                size_t zero_end = pos;
                while (CharInt_hex(c)) GetChar
                if (IdentifierStart(c)) ErrInt
                
                // Hexidal
                token->type = TokenType_Int;
                token->subtype = IntType_hex;
                // Allocating space for token
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                token_set_pos(token, parser);
                parser->position = pos;
            } else {
                while (c == '0') GetChar
                size_t zero_end = pos;
                while (CharInt_dec(c)) GetChar

                // Float
                if (c == '.') {
                    GetChar
                    while (CharInt_dec(c)) GetChar
                    token->subtype = IntType_float;
                } else {
                    // Decimal
                    token->subtype = IntType_dec;
                }
                if (IdentifierStart(c)) ErrInt

                // Integer
                token->type = TokenType_Int;
                // Allocating space for token
                token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                token_set_pos(token, parser);
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
            if (IdentifierStart(c)) ErrInt

            token->type = TokenType_Int;
            // Allocating space for token
            token_resize(token, pos - parser->position);
            memcpy(token->data, &parser->data[parser->position], token->size);
            token_set_pos(token, parser);
            parser->position = pos;
        }
    }
}

#define set_error_lexical_int(message) error_fill_in(error, LEXICAL_ANALYSIS_ERROR, message, 0, 0, 0);
#define set_error_lexical_int(message) error_fill_in(error, LEXICAL_ANALYSIS_ERROR, message, 0, 0, 0);

// Get integer
int get_integer(const struct string_st *str, struct error_st *error) {
    size_t pos = 0;
    if (CharInt_dec(str->data[pos])) {
        char c = str->data[pos];
        if (c == '0') {
            if (++pos == str->size) return 0;
            c = str->data[pos];
            if (c == 'b' || c == 'B') {
                if (++pos == str->size) return 0;
                c = str->data[pos];
                while (c == '_') {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_bin(c)) {
                    if (++pos == str->size)
                        return strtol(str->data + zero_end, NULL, 2);
                    c = str->data[pos];
                }
                if (IdentifierStart(c)) {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return strtol(str->data + zero_end, NULL, 2);
            } else if (c == 'o' || c == 'O') {
                if (++pos == str->size) return 0;
                c = str->data[pos];
                while (c == '_') {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_oct(c)) {
                    if (++pos == str->size)
                        return strtol(str->data + zero_end, NULL, 8);
                    c = str->data[pos];
                }
                if (IdentifierStart(c)) {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return strtol(str->data + zero_end, NULL, 8);
            } else if (c == 'x' || c == 'X') {
                if (++pos == str->size) return 0;
                c = str->data[pos];
                while (c == '_') {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_oct(c)) {
                    if (++pos == str->size)
                        return strtol(str->data + zero_end, NULL, 16);
                    c = str->data[pos];
                }
                if (IdentifierStart(c)) {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return strtol(str->data + zero_end, NULL, 16);
            } else {
                while (c == '0')  {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_dec(c))  {
                    if (++pos == str->size)
                        return strtol(str->data + zero_end, NULL, 10);
                    c = str->data[pos];
                }

                if (c == '.') {
                    if (++pos == str->size)
                        return strtol(str->data + zero_end, NULL, 10);
                    c = str->data[pos];
                    while (CharInt_dec(c))  {
                        if (++pos == str->size)
                            return (int) strtof(str->data + zero_end, NULL);
                        c = str->data[pos];
                    }
                }
                if (IdentifierStart(c))  {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return strtol(str->data + zero_end, NULL, 10);
            }
        } else {
            while (CharInt_dec(c))  {
                if (++pos == str->size)
                    return strtol(str->data, NULL, 10);
                c = str->data[pos];
            }

            if (c == '.') {
                if (++pos == str->size)
                    return strtol(str->data, NULL, 10);
                c = str->data[pos];
                while (CharInt_dec(c))  {
                    if (++pos == str->size)
                        return (int) strtof(str->data, NULL);
                    c = str->data[pos];
                }
            }
            if (IdentifierStart(c)) {
                set_error_lexical_int("Error while parsing integer")
                return 0;
            }
            return strtol(str->data, NULL, 10);
        }
    }
    return 0;
}
// Get float
float get_float(const struct string_st *str, struct error_st *error) {
    size_t pos = 0;
    if (CharInt_dec(str->data[pos])) {
        char c = str->data[pos];
        if (c == '0') {
            if (++pos == str->size) return 0;
            c = str->data[pos];
            if (c == 'b' || c == 'B') {
                if (++pos == str->size) return 0;
                c = str->data[pos];
                while (c == '_') {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_bin(c)) {
                    if (++pos == str->size)
                        return (float) strtol(str->data + zero_end, NULL, 2);
                    c = str->data[pos];
                }
                if (IdentifierStart(c)) {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return (float) strtol(str->data + zero_end, NULL, 2);
            } else if (c == 'o' || c == 'O') {
                if (++pos == str->size) return 0;
                c = str->data[pos];
                while (c == '_') {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_oct(c)) {
                    if (++pos == str->size)
                        return (float) strtol(str->data + zero_end, NULL, 8);
                    c = str->data[pos];
                }
                if (IdentifierStart(c)) {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return (float) strtol(str->data + zero_end, NULL, 8);
            } else if (c == 'x' || c == 'X') {
                if (++pos == str->size) return 0;
                c = str->data[pos];
                while (c == '_') {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_oct(c)) {
                    if (++pos == str->size)
                        return (float) strtol(str->data + zero_end, NULL, 16);
                    c = str->data[pos];
                }
                if (IdentifierStart(c)) {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return (float) strtol(str->data + zero_end, NULL, 16);
            } else {
                while (c == '0')  {
                    if (++pos == str->size) return 0;
                    c = str->data[pos];
                }
                size_t zero_end = pos;
                while (CharInt_dec(c))  {
                    if (++pos == str->size)
                        return (float) strtol(str->data + zero_end, NULL, 10);
                    c = str->data[pos];
                }

                if (c == '.') {
                    if (++pos == str->size)
                        return (float) strtol(str->data + zero_end, NULL, 10);
                    c = str->data[pos];
                    while (CharInt_dec(c))  {
                        if (++pos == str->size)
                            return strtof(str->data + zero_end, NULL);
                        c = str->data[pos];
                    }
                }
                if (IdentifierStart(c))  {
                    set_error_lexical_int("Error while parsing integer")
                    return 0;
                }
                return (float) strtol(str->data + zero_end, NULL, 10);
            }
        } else {
            while (CharInt_dec(c))  {
                if (++pos == str->size)
                    return (float) strtol(str->data, NULL, 10);
                c = str->data[pos];
            }

            if (c == '.') {
                if (++pos == str->size)
                    return (float) strtol(str->data, NULL, 10);
                c = str->data[pos];
                while (CharInt_dec(c))  {
                    if (++pos == str->size)
                        return strtof(str->data, NULL);
                    c = str->data[pos];
                }
            }
            if (IdentifierStart(c)) {
                set_error_lexical_int("Error while parsing integer")
                return 0;
            }
            return (float) strtol(str->data, NULL, 10);
        }
    }
    return 0;
}