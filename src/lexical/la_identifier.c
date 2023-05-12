#include "lexical_analysis.h"

// Identifier token type
void la_identifier(struct token_st *token, struct la_parser *parser) {
    if (IdentifierStart(parser->data[parser->position])) {
        size_t pos = parser->position;
        for (; pos < parser->str_size; pos++) {
            if (!IdentifierChar(parser->data[pos])) break;
        }
        token->type = TokenType_Identifier;
        token_resize(token, pos - parser->position);
        memcpy(token->data, &parser->data[parser->position], token->size);
        token_set_pos(token, parser);
        parser->position = pos;
    }
}