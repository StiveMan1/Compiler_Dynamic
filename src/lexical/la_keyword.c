#include "lexical_analysis.h"

int la_keyword_cmp(struct la_parser *parser, const char *data, size_t size) {
    if (parser->str_size - parser->position < size) return 0;
    if (memcmp(&parser->data[parser->position], data, size) != 0) return 0;
    if (parser->str_size - parser->position >= size + 1 && IdentifierChar(parser->data[parser->position + size]))
        return 0;
    return 1;
}

void la_keyword(struct token_st *token, struct la_parser *parser) {
    // Keywords length 2
    if (la_keyword_cmp(parser, "if", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IF;
        parser->position += 2;
        return;
    }
    if (la_keyword_cmp(parser, "in", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IN;
        parser->position += 2;
        return;
    }
    if (la_keyword_cmp(parser, "is", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IS;
        parser->position += 2;
        return;
    }
    if (la_keyword_cmp(parser, "or", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_OR;
        parser->position += 2;
        return;
    }

    // Keywords length 3
    if (la_keyword_cmp(parser, "and", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_AND;
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "end", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_END;
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "for", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FOR;
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "int", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_INT;
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "not", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_NOT;
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "var", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_VAR;
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "xor", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_XOR;
        parser->position += 3;
        return;
    }

    // Keywords length 4
    if (la_keyword_cmp(parser, "bool", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_BOOL;
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "else", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_ELSE;
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "func", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FUNC;
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "loop", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_LOOP;
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "real", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_REAL;
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "then", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_THEN;
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "true", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_TRUE;
        parser->position += 4;
        return;
    }


    if (la_keyword_cmp(parser, "empty", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_EMPTY;
        parser->position += 5;
        return;
    }
    if (la_keyword_cmp(parser, "false", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FALSE;
        parser->position += 5;
        return;
    }
    if (la_keyword_cmp(parser, "print", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_PRINT;
        parser->position += 5;
        return;
    }
    if (la_keyword_cmp(parser, "while", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_WHILE;
        parser->position += 5;
        return;
    }

    if (la_keyword_cmp(parser, "return", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_RETURN;
        parser->position += 6;
        return;
    }
    if (la_keyword_cmp(parser, "string", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_STRING;
        parser->position += 6;
        return;
    }

}