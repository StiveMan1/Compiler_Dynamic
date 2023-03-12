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
        token_set_pos(token, parser);
        parser->position += 2;
        return;
    }
    if (la_keyword_cmp(parser, "in", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IN;
        token_set_pos(token, parser);
        parser->position += 2;
        return;
    }
    if (la_keyword_cmp(parser, "is", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IS;
        token_set_pos(token, parser);
        parser->position += 2;
        return;
    }
    if (la_keyword_cmp(parser, "or", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_OR;
        token_set_pos(token, parser);
        parser->position += 2;
        return;
    }

    // Keywords length 3
    if (la_keyword_cmp(parser, "and", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_AND;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "end", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_END;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "for", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FOR;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "int", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_INT;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "not", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_NOT;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "var", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_VAR;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }
    if (la_keyword_cmp(parser, "xor", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_XOR;
        token_set_pos(token, parser);
        parser->position += 3;
        return;
    }

    // Keywords length 4
    if (la_keyword_cmp(parser, "bool", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_BOOL;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "else", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_ELSE;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "func", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FUNC;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "loop", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_LOOP;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "real", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_REAL;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "then", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_THEN;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }
    if (la_keyword_cmp(parser, "true", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_TRUE;
        token_set_pos(token, parser);
        parser->position += 4;
        return;
    }


    if (la_keyword_cmp(parser, "empty", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_EMPTY;
        token_set_pos(token, parser);
        parser->position += 5;
        return;
    }
    if (la_keyword_cmp(parser, "false", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FALSE;
        token_set_pos(token, parser);
        parser->position += 5;
        return;
    }
    if (la_keyword_cmp(parser, "print", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_PRINT;
        token_set_pos(token, parser);
        parser->position += 5;
        return;
    }
    if (la_keyword_cmp(parser, "while", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_WHILE;
        token_set_pos(token, parser);
        parser->position += 5;
        return;
    }

    if (la_keyword_cmp(parser, "return", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_RETURN;
        token_set_pos(token, parser);
        parser->position += 6;
        return;
    }
    if (la_keyword_cmp(parser, "string", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_STRING;
        token_set_pos(token, parser);
        parser->position += 6;
        return;
    }

}